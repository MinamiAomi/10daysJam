#include "Player.h"

#include <numbers>

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
	collider_->SetCollisionMask(CollisionAttribute::PlayerBullet | CollisionAttribute::EnemyBullet | CollisionAttribute::Block);
	collider_->SetIsActive(true);

	Reset();
}

void Player::Reset() {
	fireTime_ = 0.0f;
	invincibleTime_ = 0.0f;
	velocity_ = 0.2f;
	currentVector_ = { 0.0f,-1.0f,0.0f };

	transform.translate.y = initializePosition_;

	UpdateTransform();
}

void Player::Move() {
	auto input = Input::GetInstance();
	auto gamepad = input->GetXInputState();
	float AngularVelocity = 0.1f;  // Steering speed
	//float velocity_ = 0.1f;        // Movement speed

	// Move
	{
		Vector3 move{};

		// Handle input for movement direction
		if (std::abs(gamepad.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.x = gamepad.Gamepad.sThumbLX / 32767.0f; }
		if (std::abs(gamepad.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.y = gamepad.Gamepad.sThumbLY / 32767.0f; }
		if (input->IsKeyPressed(DIK_A)) { move.x += -1.0f; }
		if (input->IsKeyPressed(DIK_D)) { move.x += 1.0f; }
		if (input->IsKeyPressed(DIK_W)) { move.y += 1.0f; }
		if (input->IsKeyPressed(DIK_S)) { move.y += -1.0f; }

		if (move != Vector3::zero) {
			//float angle = 0.0f;
			//Vector3 from = { 0.0f, 1.0f, 0.0f };  // Default forward direction
			//Vector3 to = move.Normalized();

			//// Calculate the angle difference between current forward and input direction
			//float dot = Vector3::Dot(to, from);
			//Vector2 Vector2From = { from.x, from.y };
			//Vector2 Vector2To = { to.x, to.y };

			//if (dot >= 1.0f) {
			//	angle = 0.0f;
			//}
			//else if (dot <= -1.0f) {
			//	angle = 180.0f * std::numbers::pi_v<float> / 180.0f;
			//}
			//else if (Vector2::Cross(Vector2To, Vector2From) > 0) {
			//	angle = -std::acosf(dot);  // Turn left
			//}
			//else {
			//	angle = std::acosf(dot);   // Turn right
			//}

			//// Smoothly interpolate to the target angle

			//angle_ = std::lerp(angle_, angle * Math::ToDegree, AngularVelocity);

			currentVector_ = Vector3::Slerp(AngularVelocity, currentVector_, move.Normalized()).Normalized();
			//transform.rotate = Quaternion::MakeFromTwoVector(currentVector_, move.Normalized());
		}

		// Move forward in the direction of the current angle
		//Vector3 forwardDirection = Quaternion::MakeForZAxis(angle_ * Math::ToRadian).GetUp();
		transform.translate += currentVector_.Normalized() * velocity_;

		// Limit player movement within stage bounds
		//transform.translate.x = std::clamp(transform.translate.x, -GameProperty::GameStageSize.x, GameProperty::GameStageSize.x);
	}

}

void Player::FireBullet() {
	auto input = Input::GetInstance();
	auto gamepad = input->GetXInputState();
	// Bullet
	{
		// インターバルカウント
		if (fireTime_ > 0.0f) {
			fireTime_ -= 1.0f;
		}
		// 弾発射
		if (input->IsKeyPressed(DIK_SPACE) &&
			fireTime_ <= 0.0f) {
			bulletManager_->FireBullet(transform.translate, { 0.0f,-0.5f,0.0f }, CollisionAttribute::PlayerBullet);
			fireTime_ = fireInterval_;
		}
		bulletManager_->Update();
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
	collider_->SetSize({ 2.0f,4.0f,2.0f });
	collider_->SetCenter(transform.worldMatrix.GetTranslate());
	collider_->SetOrientation(transform.worldMatrix.GetRotate());
	model_.SetWorldMatrix(transform.worldMatrix);
}

void Player::OnCollision(const CollisionInfo& collisionInfo) {
	if (collisionInfo.gameObject->GetName() == "Block") {
		transform.translate += collisionInfo.depth * collisionInfo.normal;
		UpdateTransform();
	}
	if (collisionInfo.gameObject->GetName() == "EnemyBullet") {
		model_.SetColor({ 1.0f, 0.0f, 0.0f });
		invincibleTime_ = invincibleInterval_;
	}
}


void Player::Update() {
	Move();
	FireBullet();
	UpdateInvincible();
	UpdateTransform();

}