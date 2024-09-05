#include "Player.h"

#include "Input/input.h"
#include "Framework/AssetManager.h"

void Player::Initialize() {
	model_.SetModel(AssetManager::GetInstance()->FindModel("player"));

	bulletManager_ = std::make_shared<PlayerBulletManager>();
	bulletManager_->Initialize();
	fireTime_ = 0.0f;

	//walk_ = AssetManager::GetInstance()->FindAnimation("human_walk");
	//skeleton_ = std::make_shared<Skeleton>();
	//skeleton_->Create(model_.GetModel());
	//model_.SetSkeleton(skeleton_);
	//skeleton_->ApplyAnimation(walk_->GetAnimation("Walk"), 0.0f);
	//skeleton_->Update();

	//hammer_.SetModel(AssetManager::GetInstance()->FindModel("hammer"));
}

void Player::Update() {
	auto input = Input::GetInstance();
	auto gamepad = input->GetXInputState();
	// Move
	{
		Vector3 move;
		if (std::abs(gamepad.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.x = gamepad.Gamepad.sThumbLX / 255.0f; }
		if (std::abs(gamepad.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { move.z = gamepad.Gamepad.sThumbLY / 255.0f; }
		if (input->IsKeyPressed(DIK_A)) { move.x = -1.0f; }
		if (input->IsKeyPressed(DIK_D)) { move.x = 1.0f; }
		//if (input->IsKeyPressed(DIK_W)) { move.y = 1.0f; }
		//if (input->IsKeyPressed(DIK_S)) { move.y = -1.0f; }
		if (move != Vector3::zero) {
			move = move.Normalized() * 0.4f;

			transform_.translate += move;
			Quaternion targetRotate = Quaternion::MakeLookRotation(move) * Quaternion::MakeForYAxis(Math::Pi);
			//transform_.rotate = Quaternion::Slerp(0.2f, transform_.rotate, targetRotate);

			//animationTime_ += 1.0f / 60.0f;
			//animationTime_ = std::fmod(animationTime_, 1.0f);
			//skeleton_->ApplyAnimation(walk_->GetAnimation("Walk"), animationTime_);
			//skeleton_->Update();
		}
		//skeleton_->DebugDraw(model_.GetWorldMatrix());
	}

	// Bullet
	{
		// インターバルカウント
		if (fireTime_ > 0.0f) {
			fireTime_ -= 1.0f;
		}
		// 弾発射
		if (input->IsKeyPressed(DIK_SPACE) &&
			fireTime_ <= 0.0f) {
			bulletManager_->FireBullet(transform_.translate, { 0.0f,-0.5f,0.0f });
			fireTime_ = fireInterval_;
		}
		bulletManager_->Update();
	}
	transform_.UpdateMatrix();
	model_.SetWorldMatrix(transform_.worldMatrix);
	//auto& joint = skeleton_->GetJoint("mixamorig:RightHandIndex4");
	//hammer_.SetWorldMatrix(Matrix4x4::MakeScaling(Vector3(50.0f)) * Matrix4x4::MakeRotationXYZ(Vector3{ 0.0f, -60.0f, 210.0f } *Math::ToRadian) * joint.skeletonSpaceMatrix * transform_.worldMatrix);

}
