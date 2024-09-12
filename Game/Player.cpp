#include "Player.h"

#include <numbers>

#include "Input/input.h"
#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"
#include "Engine/Math/MathUtils.h"
#include "Game/GameProperty.h"
#include "Engine/Graphics/ImGuiManager.h"
#include "Engine/File/JsonHelper.h"
#include "Map.h"

void Player::Initialize(Map* map) {
	SetName("Player");
	model_.SetModel(AssetManager::GetInstance()->FindModel("player"));
	collider_ = std::make_shared<BoxCollider>();
	collider_->SetGameObject(this);
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::Player);
	collider_->SetCollisionMask(CollisionAttribute::PlayerBullet | CollisionAttribute::EnemyBullet | CollisionAttribute::Block);
	collider_->SetIsActive(true);

	mapCollider_ = std::make_shared<MapCollider>();
	mapCollider_->SetMode(MapCollider::Break);
	mapCollider_->SetSize({ colliderSize_.x, colliderSize_.y });
	mapCollider_->SetPosition(transform.translate.GetXY());
	mapCollider_->SetRotate(transform.rotate.EulerAngle().z);
	mapCollider_->SetIsActive(true);
	map->AddCollider(mapCollider_);

	Reset();
}

void Player::Reset() {
	invincibleTime_ = 0.0f;
	JSON_OPEN("Resources/Data/Player/player.json");
	JSON_LOAD(speed_);
	JSON_LOAD(initializePosition_);
	JSON_LOAD(directionSpeed_);
	JSON_LOAD(gravity_);
	JSON_LOAD(resistance_);
	JSON_LOAD(maxSpeed_);
	JSON_CLOSE();

	currentVector_ = { 0.0f,-1.0f,0.0f };
	velocity_ = currentVector_ * speed_;
	transform.translate = initializePosition_;
	transform.rotate = Quaternion::MakeLookRotation(-currentVector_, Vector3::forward);
	UpdateTransform();
}

void Player::SetGameClearPosPosition(float distance) {

	transform.translate.z = distance;
}

void Player::AddGravity(const Vector3& vector) {
	velocity_ += vector * gravity_;
}

void Player::Move() {
	auto input = Input::GetInstance();
	auto gamepad = input->GetXInputState();
	// Move
	{
		Vector3 move{};
		Vector3 directionAcceleration{};
		// ゲームパッドのスティックで移動を処理
		if (std::abs(gamepad.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.x = gamepad.Gamepad.sThumbLX / 32767.0f; }
		if (std::abs(gamepad.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.y = gamepad.Gamepad.sThumbLY / 32767.0f; }

		// Aキーが押されている間、加速を適用
		if (input->IsKeyPressed(DIK_A)) {
			directionAcceleration += Vector3(currentVector_.y, -currentVector_.x, currentVector_.z) * directionSpeed_;
			if (velocity_.Length() != 0.0f) {
				UpdateRotate(velocity_.Normalized());
			}
			else {
				UpdateRotate((currentVector_ + directionAcceleration).Normalized());
			}
		}

		// Dキーが押されている間、加速を適用
		if (input->IsKeyPressed(DIK_D)) {
			directionAcceleration += Vector3(-currentVector_.y, currentVector_.x, currentVector_.z) * directionSpeed_;
			if (velocity_.Length() != 0.0f) {
				UpdateRotate(velocity_.Normalized());
			}
			else {
				UpdateRotate((currentVector_ + directionAcceleration).Normalized());
			}
		}

		// Wキーで減速
		if (input->IsKeyPressed(DIK_W)) {
			// 速度があったら減速
			if (velocity_.Length() != 0.0f) {
				directionAcceleration += velocity_.Normalized() * -directionSpeed_;
				// 0.0fに近かったら止まる
				if (velocity_.Length() - directionAcceleration.Length() <= 0.01f) {
					directionAcceleration = Vector3::zero;
					velocity_ = Vector3::zero;
				}
			}
		}
		// Sキーで加速
		if (input->IsKeyPressed(DIK_S)) {
			// プレイヤーの方向に加速
			if (currentVector_.Length() != 0.0f) {
				directionAcceleration += currentVector_.Normalized() * directionSpeed_;
			}
		}
		// 速度に加速度を加算
		velocity_ += directionAcceleration;
		// 空気抵抗的な
		velocity_ *= resistance_;
		// プレイヤーの位置を更新
		transform.translate += velocity_;
		// 速度のclamp
		if (velocity_.Length() >= maxSpeed_) {
			velocity_ = velocity_.Normalized() * maxSpeed_;
		}
		// プレイヤーの移動制限
		if (transform.translate.x <= -MapProperty::kSideLimit) {
			transform.translate.x = -MapProperty::kSideLimit;
			// 速度リセット
			velocity_ = Vector3::zero;
		}
		if (transform.translate.x >= MapProperty::kSideLimit) {
			transform.translate.x = MapProperty::kSideLimit;
			// 速度リセット
			velocity_ = Vector3::zero;

		}
	}
}

void Player::UpdateRotate(const Vector3& vector) {
	// 速度ベクトルを正規化して向きを計算
	currentVector_ = (vector).Normalized();
	// 向きを調整
	float dotProduct = Dot(currentVector_, Vector3::down);
	if (dotProduct < 0.999f) {
		//回転クッソ怪しい
		transform.rotate = Quaternion::MakeLookRotation(-currentVector_, Vector3::forward);
	}
}

void Player::UpdateInvincible() {
	if (invincibleTime_ > 0.0f) {
		invincibleTime_ -= 1.0f;
	}
	else {
		model_.SetColor({ 1.0f, 1.0f, 1.0f });
	}
}

void Player::UpdateTransform() {

	transform.UpdateMatrix();
	// 怪しい
	collider_->SetSize(colliderSize_);
	collider_->SetCenter(transform.worldMatrix.GetTranslate());
	collider_->SetOrientation(transform.worldMatrix.GetRotate());
	model_.SetWorldMatrix(transform.worldMatrix);

	mapCollider_->SetPosition(transform.translate.GetXY());
	mapCollider_->SetRotate(transform.rotate.EulerAngle().z);
}

void Player::OnCollision(const CollisionInfo& collisionInfo) {
	if (collisionInfo.gameObject->GetName() == "NormalBlock") {
		transform.translate += collisionInfo.depth * collisionInfo.normal;
		UpdateTransform();
	}
	if (collisionInfo.gameObject->GetName() == "ExplosionBlock") {
		velocity_ += Vector3(collisionInfo.normal.x, collisionInfo.normal.y, 0.0f) * 0.3f;
		UpdateRotate(velocity_.Normalized());
		UpdateTransform();
	}
	if (collisionInfo.gameObject->GetName() == "EnemyBullet") {
		model_.SetColor({ 1.0f, 0.0f, 0.0f });
		invincibleTime_ = invincibleInterval_;
	}
}

void Player::Debug() {
	ImGui::Begin("GameObject");
	if (ImGui::BeginMenu("Player")) {
		ImGui::DragFloat3("velocity", &velocity_.x, 0.1f);
		ImGui::DragFloat3("currentVector", &currentVector_.x, 0.1f);
		if (ImGui::TreeNode("Property")) {
			ImGui::DragFloat3("initializePosition", &initializePosition_.x, 0.1f);
			ImGui::DragFloat("speed", &speed_, 0.01f);
			ImGui::DragFloat("directionSpeed", &directionSpeed_, 0.01f);
			ImGui::DragFloat("gravity", &gravity_, 0.01f);
			ImGui::DragFloat("resistance", &resistance_, 0.01f);
			ImGui::DragFloat("maxSpeed", &maxSpeed_, 0.01f);
			if (ImGui::Button("Save")) {
				JSON_OPEN("Resources/Data/Player/player.json");
				JSON_SAVE(speed_);
				JSON_SAVE(initializePosition_);
				JSON_SAVE(directionSpeed_);
				JSON_SAVE(gravity_);
				JSON_SAVE(resistance_);
				JSON_SAVE(maxSpeed_);
				JSON_CLOSE();
			}

			ImGui::TreePop();
		}
		ImGui::EndMenu();
	}
	ImGui::End();
}


void Player::Update() {
	Move();
	UpdateInvincible();
	UpdateTransform();
#ifdef _DEBUG
	Debug();
#endif // _DEBUG
}