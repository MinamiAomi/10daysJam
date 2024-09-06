#include "Player.h"

#include "Input/input.h"
#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"
#include "Engine/Math/MathUtils.h"
#include "Game/GameProperty.h"

void Player::Initialize() {
	SetName("Player");
	model_.SetModel(AssetManager::GetInstance()->FindModel("player"));
	collider_ = std::make_shared<BoxCollider>();
	collider_->SetGameObject(this->shared_from_this());
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::Player);
	collider_->SetCollisionMask(CollisionAttribute::PlayerBullet | CollisionAttribute::Block);
	collider_->SetIsActive(true);

	bulletManager_ = std::make_shared<PlayerBulletManager>();
	bulletManager_->Initialize();

	Reset();
}

void Player::Reset() {
	fireTime_ = 0.0f;

	canJump_ = true;
	velocity_ = Vector3::zero;
	transform.translate.y = initializePosition_;

	UpdateTransform();

	bulletManager_->Reset();
}

void Player::UpdateTransform() {
	transform.UpdateMatrix();
	// 怪しい
	collider_->SetSize({ 2.0f,4.0f,2.0f });
	collider_->SetCenter(transform.worldMatrix.GetTranslate());
	collider_->SetOrientation(transform.worldMatrix.GetRotate());
	model_.SetWorldMatrix(transform.worldMatrix);
}

void Player::OnCollision(const CollisionInfo& collisionInfo) {
	if (collisionInfo.gameObject->GetName() == "Block") {
		// 着地
		if (Vector3::Dot(collisionInfo.normal, Vector3(0.0f, 1.0f, 0.0f)) >= 0.8f) {
			canJump_ = true;
			velocity_.y = 0.0f;
		}
		transform.translate += collisionInfo.depth * collisionInfo.normal;
		UpdateTransform();
	}
}


void Player::Update() {
	auto input = Input::GetInstance();
	auto gamepad = input->GetXInputState();
	// Move
	{
		Vector3 move;
		velocity_.x = 0.0f;
		if (std::abs(gamepad.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.x = gamepad.Gamepad.sThumbLX / 255.0f; }
		if (std::abs(gamepad.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.z = gamepad.Gamepad.sThumbLY / 255.0f; }
		if (input->IsKeyPressed(DIK_A)) { move.x = -1.0f; }
		if (input->IsKeyPressed(DIK_D)) { move.x = 1.0f; }
		float acceleration = 0.0f;
		if (input->IsKeyTrigger(DIK_W) && canJump_) {
			acceleration = 0.8f;
			canJump_ = false;
		}
		if (move != Vector3::zero) {
			move = move.Normalized() * 0.4f;
			velocity_.x += move.x;
		}
		velocity_.y += acceleration;
		velocity_.y -= 0.03f;
		velocity_.y = (std::max)(velocity_.y, -1.0f);
		transform.translate += velocity_;
		// プレイヤー反転
		if (transform.translate.x > GameProperty::GameStageSize.x) {
			transform.translate.x = -GameProperty::GameStageSize.x;
		}
		else if (transform.translate.x < -GameProperty::GameStageSize.x) {
			transform.translate.x = GameProperty::GameStageSize.x;
		}
	}

	// Bullet
	{
		// インターバルカウント
		if (fireTime_ > 0.0f) {
			fireTime_ -= 1.0f;
		}
		bool inputFlag = input->IsKeyPressed(DIK_SPACE) && input->IsGamepadButtonPressed(GamepadButton::B);
		// 弾発射
		if (inputFlag &&
			fireTime_ <= 0.0f) {
			bulletManager_->FireBullet(transform.translate, { 0.0f,-0.5f,0.0f });
			fireTime_ = fireInterval_;
		}
		bulletManager_->Update();
	}
	UpdateTransform();

}