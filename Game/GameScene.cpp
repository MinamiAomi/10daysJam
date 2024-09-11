#include "GameScene.h"

#include "Framework/Engine.h"
#include "Input/Input.h"
#include "Graphics/RenderManager.h"
#include "Framework/AssetManager.h"
#include "Graphics/Core/TextureLoader.h"

#include "Engine/Collision/CollisionManager.h"
#include "GameProperty.h"

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
	score_ = std::make_shared<Score>();

	player_ = std::make_shared<Player>();
	map_ = std::make_shared<Map>();
	
	map_->SetPlayer(player_);
	map_->SetBlockParticles(blockParticles_);
	map_->SetScore(score_);
	map_->Initialize();

	player_->Initialize(map_.get());
	score_->Initialize();

	followCamera_ = std::make_shared<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetPlayer(player_);
	followCamera_->SetCamera(camera_);

	gameClearCamera_ = std::make_shared<GameClearCamera>();
	gameClearCamera_->Initialize();
	gameClearCamera_->SetCamera(camera_);

	GameProperty::state_ = GameProperty::kInGame;
}

void GameScene::OnUpdate() {

	Input* input = Input::GetInstance();

	if (input->IsKeyTrigger(DIK_P)) {
		blockParticles_->Emit({ 0.0f,5.0f,0.0f });
	}

	switch (GameProperty::state_) {
	case GameProperty::kInGame:
	{
		player_->Update();
		followCamera_->Update();
		score_->Update();
		map_->CheckCollision();
		CollisionManager::GetInstance()->CheckCollision();
		blockParticles_->Update();

		// �N���A�`�F�b�N
		if (score_->GetIsClear()) {
			gameClearCamera_->SetCameraPosition(-(float(map_->GetMapRow()) + MapProperty::kBlockSize * 2.0f));
			player_->SetGameClearPosPosition(gameClearCamera_->GetEndCameraPos().z + 80.0f);
		// リセット
		if (input->IsKeyTrigger(DIK_R)) {
			player_->Reset();
			followCamera_->Reset();
			map_->Generate();
			score_->Reset();
		}
	}
	break;
	case GameProperty::kResult:
	{
		// カメラが動いているとき
		if (!gameClearCamera_->GetIsEasing()) {
			gameClearCamera_->Update();
		}
		else {
			// リセット
			if (input->IsKeyTrigger(DIK_R)) {
				player_->Reset();
				followCamera_->Reset();
				map_->Generate();
				score_->Reset();
				GameProperty::state_ = GameProperty::kInGame;
			}
			player_->Update();
		}
	}
	break;
	default:
		break;
		enemyManager_->Reset();
		player_->Reset();
		followCamera_->Reset();
	}
}

void GameScene::OnFinalize() {

}
