#include "GameScene.h"

#include "Framework/Engine.h"
#include "Input/Input.h"
#include "Graphics/RenderManager.h"
#include "Framework/AssetManager.h"
#include "Graphics/Core/TextureLoader.h"

#include "Engine/Collision/CollisionManager.h"

void GameScene::OnInitialize() {

	camera_ = std::make_shared<Camera>();
	camera_->SetPosition({ 0.0f, -10.0f, -80.0f });
	camera_->SetRotate(Quaternion::MakeLookRotation({ 0.0f, 0.0f, 1.0f }));
	camera_->UpdateMatrices();
	RenderManager::GetInstance()->SetCamera(camera_);

	sunLight_ = std::make_shared<DirectionalLight>();
	sunLight_->direction = Vector3(0.0f, -1.0f, 0.0f).Normalized();
	RenderManager::GetInstance()->SetSunLight(sunLight_);

	blockParticles_ = std::make_shared<BlockParticles>();
	blockParticles_->Initialize(player_.get());

	bulletManager_ = std::make_shared<BulletManager>();
	bulletManager_->Initialize();

	blockManager_ = std::make_shared<BlockManager>();
	blockManager_->Initialize(blockParticles_.get());

	enemyManager_ = std::make_shared<EnemyManager>();
	enemyManager_->Initialize();
	enemyManager_->SetBulletManager(bulletManager_);

	player_ = std::make_shared<Player>();
	player_->Initialize();
	player_->SetBulletManager(bulletManager_);

	followCamera_ = std::make_shared<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetPlayer(player_);
	followCamera_->SetCamera(camera_);
	//map_ = std::make_shared<Map>();
	//map_->SetPlayer(player_);
	//map_->Initialize();
}

void GameScene::OnUpdate() {

	Input* input = Input::GetInstance();

	if (input->IsKeyTrigger(DIK_P)) {
		blockParticles_->Emit({ 0.0f,5.0f,0.0f });
	}

	blockParticles_->Update();

	blockManager_->Update();
	enemyManager_->Update();
	player_->Update();
	followCamera_->Update();
	bulletManager_->Update();

	CollisionManager::GetInstance()->CheckCollision();


	//auto mouseMoveX = input->GetMouseMoveX();
	//auto mouseMoveY = input->GetMouseMoveY();
	//auto wheel = input->GetMouseWheel();

	//Quaternion rotate = camera_->GetRotate();
	//Vector3 position = camera_->GetPosition();

	//Vector3 diffPosition;

	//if (input->IsMousePressed(1)) {
	//	constexpr float rotSpeed = Math::ToRadian * 0.1f;
	//	euler_.x += rotSpeed * static_cast<float>(mouseMoveY);
	//	euler_.y += rotSpeed * static_cast<float>(mouseMoveX);
	//}
	//else if (input->IsMousePressed(2)) {
	//	Vector3 cameraX = rotate.GetRight() * (-static_cast<float>(mouseMoveX) * 0.01f);
	//	Vector3 cameraY = rotate.GetUp() * (static_cast<float>(mouseMoveY) * 0.01f);
	//	diffPosition += cameraX + cameraY;
	//}
	//else if (wheel != 0) {
	//	Vector3 cameraZ = rotate.GetForward() * (static_cast<float>(wheel / 120) * 0.5f);
	//	diffPosition += cameraZ;
	//}

	//{
	//	auto BoolInt = [](bool x) {
	//		return x ? 1 : 0;
	//		};

	//	int xRotate = BoolInt(input->IsKeyPressed(DIK_DOWN)) - BoolInt(input->IsKeyPressed(DIK_UP));
	//	int yRotate = BoolInt(input->IsKeyPressed(DIK_RIGHT)) - BoolInt(input->IsKeyPressed(DIK_LEFT));

	//	constexpr float rotSpeed = Math::ToRadian * 1.0f;
	//	euler_.x += rotSpeed * static_cast<float>(xRotate);
	//	euler_.y += rotSpeed * static_cast<float>(yRotate);


	//	/*  int xMove = BoolInt(input->IsKeyPressed(DIK_D)) - BoolInt(input->IsKeyPressed(DIK_A));
	//	  Vector3 cameraX = rotate.GetRight() * (float)xMove * 0.5f;
	//	  diffPosition += cameraX;

	//	  int yMove = BoolInt(input->IsKeyPressed(DIK_SPACE)) - BoolInt(input->IsKeyPressed(DIK_LSHIFT));
	//	  Vector3 cameraY = rotate.GetUp() * (float)yMove * 0.5f;
	//	  diffPosition += cameraY;

	//	  int zMove = BoolInt(input->IsKeyPressed(DIK_W)) - BoolInt(input->IsKeyPressed(DIK_S));
	//	  Vector3 cameraZ = rotate.GetForward() * (float)zMove * 0.5f;
	//	  diffPosition += cameraZ;*/
	//}
	if (input->IsKeyTrigger(DIK_R)) {
		bulletManager_->Reset();
		//blockManager_->Reset();
		enemyManager_->Reset();
		player_->Reset();
	}


	//camera_->SetPosition(position + diffPosition);
	//camera_->SetRotate(Quaternion::MakeFromEulerAngle(euler_));
	//camera_->UpdateMatrices();

}

void GameScene::OnFinalize() {

}
