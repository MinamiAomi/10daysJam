#include "GameClearCamera.h"

#include "Engine/Graphics/ImGuiManager.h"
#include "File/JsonHelper.h"

void GameClearCamera::Initialize() {
	JSON_OPEN("Resources/Data/GameClearCamera/gameClearCamera.json");
	JSON_LOAD(transitionFrame_);
	JSON_CLOSE();
}

void GameClearCamera::Update() {
	if (time_ < 1.0f) {
		Vector3 position = Vector3::Lerp(time_, startCameraPos_, endCameraPos_);
		camera_->SetPosition(position);
		camera_->UpdateMatrices();
		time_ += 1.0f / transitionFrame_;
	}
	else {
		isEasingCamera_ = true;
	}
#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

void GameClearCamera::SetCameraPosition(float mapRowLength) {
	time_ = 0.0f;
	transitionFrame_ = 90.0f;
	isEasingCamera_ = false;
	startCameraPos_ = camera_->GetPosition();
	mapRowLength;
	//float distance = (mapRowLength * 0.5f) / std::tan(camera_->GetProjection().perspective.fovY * 0.5f);
	endCameraPos_ = { 0.0f,0.0f,camera_->GetPosition().z };
}

void GameClearCamera::Debug() {
	ImGui::Begin("GameObject");
	if (ImGui::BeginMenu("GameClearCamera")) {
		ImGui::DragFloat("time_", &time_, 0.1f);
		if (ImGui::TreeNode("Property")) {
			ImGui::DragFloat("transitionFrame_", &transitionFrame_, 0.1f);
			if (ImGui::Button("Save")) {
				JSON_OPEN("Resources/Data/GameClearCamera/gameClearCamera.json");
				JSON_SAVE(transitionFrame_);
				JSON_CLOSE();
			}

			ImGui::TreePop();
		}
		ImGui::EndMenu();
	}
	ImGui::End();
}
