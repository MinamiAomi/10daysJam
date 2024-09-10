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
	collider_->SetGameObject(this);
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
	angle_ = 270.0f;
	currentVector_ = { 0.0f,1.0f,0.0f };

	transform.translate.y = initializePosition_;

	UpdateTransform();
}

void Player::Move() {
	auto input = Input::GetInstance();
	auto gamepad = input->GetXInputState();
	float AngularVelocity = 2.0f;  // Steering speed
	//float velocity_ = 0.1f;        // Movement speed
	// 
	// Move
	{
		Vector3 move{};

		// Handle input for movement direction
		if (std::abs(gamepad.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.x = gamepad.Gamepad.sThumbLX / 32767.0f; }
		if (std::abs(gamepad.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.y = gamepad.Gamepad.sThumbLY / 32767.0f; }
		if (input->IsKeyPressed(DIK_A)) { angle_ -= AngularVelocity; }
		if (input->IsKeyPressed(DIK_D)) { angle_ += AngularVelocity; }
		float angle = angle_ * Math::ToRadian;
		move = { std::cosf(angle),std::sinf(angle),0.0f };
		/*if (move != Vector3::zero) {

			currentVector_ = Vector3::Slerp(AngularVelocity, currentVector_, move.Normalized()).Normalized();
		}*/

		transform.translate += move * velocity_;
		Quaternion rotate = Quaternion::MakeForXAxis(Math::HalfPi);
		transform.rotate = Quaternion::MakeLookRotation(move) * rotate;

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