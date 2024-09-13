#include "GameScene.h"

#include "Framework/Engine.h"
#include "Input/Input.h"
#include "Graphics/RenderManager.h"
#include "Framework/AssetManager.h"
#include "Graphics/Core/TextureLoader.h"

#include "Engine/Collision/CollisionManager.h"
#include "GameProperty.h"
#include "Graphics/App/SkyRenderer.h"

#include "Engine/Graphics/ImGuiManager.h"

void GameScene::OnInitialize() {

	camera_ = std::make_shared<Camera>();
	camera_->SetPosition({ 0.0f, -10.0f, -80.0f });
	camera_->SetRotate(Quaternion::MakeLookRotation({ 0.0f, 0.0f, 1.0f }));
	camera_->UpdateMatrices();
	SkyRenderer::y_ = 0;
	SkyRenderer::switchNum_ = 0;

	RenderManager::GetInstance()->SetCamera(camera_);

	sunLight_ = std::make_shared<DirectionalLight>();
	sunLight_->direction = Vector3(0.0f, -1.0f, 0.0f).Normalized();
	RenderManager::GetInstance()->SetSunLight(sunLight_);

	score_ = std::make_shared<Score>();
	player_ = std::make_shared<Player>();
	map_ = std::make_shared<Map>();

	blockParticles_ = std::make_shared<BlockParticles>();
	blockParticles_->SetMap(map_.get());
	blockParticles_->Initialize();

	map_->SetCamera(camera_);
	map_->SetPlayer(player_);
	map_->SetBlockParticles(blockParticles_);
	map_->SetScore(score_);
	map_->Initialize();

	player_->Initialize(map_.get());

	followCamera_ = std::make_shared<FollowCamera>();
	followCamera_->SetPlayer(player_);
	followCamera_->SetCamera(camera_);
	followCamera_->Initialize();

	score_->SetPlayer(player_);
	score_->SetParent(followCamera_->transform_);
	score_->Initialize();

	gameClearCamera_ = std::make_shared<GameClearCamera>();
	gameClearCamera_->Initialize();
	gameClearCamera_->SetCamera(camera_);

	GameProperty::state_ = GameProperty::kInGame;

	particles_ = std::make_shared<Particles>();
	particles_->SetCamera(camera_.get());
	particles_->SetPlayer(player_.get());
	particles_->Initialize();
}

void GameScene::OnUpdate() {

	Input* input = Input::GetInstance();

	if (input->IsKeyTrigger(DIK_P)) {
		particles_->SetEmitRotate(true);
	}
	else {
		particles_->SetEmitRotate(false);
	}
	particles_->SetEmitPlayer(true);


	switch (GameProperty::state_) {
	case GameProperty::kInGame:
	{
		player_->Update();
		followCamera_->Update();
		map_->Update();
		score_->Update();
		map_->CheckCollision();
		CollisionManager::GetInstance()->CheckCollision();
		blockParticles_->Update();
		particles_->Update();
		// リセット
		if (input->IsKeyTrigger(DIK_R)) {
			player_->Reset();
			followCamera_->Reset();
			map_->Generate();
			score_->Reset();
			SkyRenderer::y_ = 0;
			SkyRenderer::switchNum_ = 0;
		}

		// クリアしたか
		if (score_->GetIsClear()) {
			gameClearCamera_->SetCameraPosition(-(float(map_->GetMapRow()) + MapProperty::kBlockSize * 2.0f));
			player_->SetGameClearPosPosition(gameClearCamera_->GetEndCameraPos().z + 80.0f);
			score_->SetParent(gameClearCamera_->transform_);
			GameProperty::state_ = GameProperty::kResult;
		}
		break;
	case GameProperty::kResult:
	{
		// 集計中
		if (score_->GetState() == Score::State::Result) {
			gameClearCamera_->SetEasingTime(score_->GetEasingTime());
			gameClearCamera_->Update();
			score_->Update();
			map_->Update();
			// 切り替わる瞬間
			if (score_->GetState() != Score::State::Result) {
				player_->Reset();
				followCamera_->Reset();
				map_->Generate();
				score_->SetParent(followCamera_->transform_);
				score_->Reset();
				GameProperty::state_ = GameProperty::kInGame;
				SkyRenderer::y_ = 0;
				SkyRenderer::switchNum_ = 0;
			}
			//player_->Update();
		}
		else {
			GameProperty::state_ = GameProperty::kInGame;
		}
	}
	break;
	default:
		break;
	}
	}


	SkyRenderer::y_ = camera_->GetPosition().y;
}

void GameScene::OnFinalize() {

}
