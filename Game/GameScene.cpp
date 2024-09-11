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

	player_ = std::make_shared<Player>();
	map_ = std::make_shared<Map>();
	
	map_->SetPlayer(player_);
	map_->SetBlockParticles(blockParticles_);
	map_->Initialize();

	player_->Initialize(map_.get());

	followCamera_ = std::make_shared<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetPlayer(player_);
	followCamera_->SetCamera(camera_);
}

void GameScene::OnUpdate() {

	Input* input = Input::GetInstance();

	if (input->IsKeyTrigger(DIK_P)) {
		blockParticles_->Emit({ 0.0f,5.0f,0.0f });
	}


	player_->Update();
	followCamera_->Update();

	map_->CheckCollision();
	CollisionManager::GetInstance()->CheckCollision();

	blockParticles_->Update();

	if (input->IsKeyTrigger(DIK_R)) {
		player_->Reset();
		followCamera_->Reset();
		map_->Initialize();
	}
}

void GameScene::OnFinalize() {

}
