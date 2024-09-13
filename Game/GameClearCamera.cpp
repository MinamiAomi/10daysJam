#include "GameClearCamera.h"

#include "Engine/Graphics/ImGuiManager.h"
#include "File/JsonHelper.h"

void GameClearCamera::Initialize() {

}

void GameClearCamera::Update() {
	transform_.translate = Vector3::Lerp(time_, startCameraPos_, endCameraPos_);
	camera_->SetPosition(transform_.translate);
	camera_->UpdateMatrices();
	transform_.UpdateMatrix();
//#ifdef _DEBUG
//	Debug();
//#endif // _DEBUG

}

void GameClearCamera::SetCameraPosition(float mapRowLength) {
	time_ = 0.0f;
	startCameraPos_ = camera_->GetPosition();
	mapRowLength;
	//float distance = (mapRowLength * 0.5f) / std::tan(camera_->GetProjection().perspective.fovY * 0.5f);
	Vector3 offset_{}, initializePosition_{};
	JSON_OPEN("Resources/Data/FollowCamera/followCamera.json");
	JSON_LOAD(offset_);
	JSON_CLOSE();
	JSON_OPEN("Resources/Data/Player/player.json");
	JSON_LOAD(initializePosition_);
	JSON_CLOSE();
	// 初期位置に
	endCameraPos_ = offset_ + initializePosition_;

	transform_.translate = startCameraPos_;
	transform_.UpdateMatrix();
}

void GameClearCamera::Debug() {
#ifdef _DEBUG


	ImGui::Begin("GameObject");
	if (ImGui::BeginMenu("GameClearCamera")) {
		ImGui::DragFloat("time_", &time_, 0.1f);
		if (ImGui::TreeNode("Property")) {
			//ImGui::DragFloat("transitionFrame_", &transitionFrame_, 0.1f);
			if (ImGui::Button("Save")) {
				/*JSON_OPEN("Resources/Data/GameClearCamera/gameClearCamera.json");
				JSON_SAVE(transitionFrame_);
				JSON_CLOSE();*/
			}

			ImGui::TreePop();
		}
		ImGui::EndMenu();
	}
	ImGui::End();
#endif // _DEBUG
}
