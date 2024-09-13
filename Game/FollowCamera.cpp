#include "FollowCamera.h"

#include "Engine/Graphics/ImGuiManager.h"
#include "Engine/File/JsonHelper.h"

void FollowCamera::Initialize() {
	Reset();
}

void FollowCamera::Update() {
	transform_.translate = { offset_.x,offset_.y + player_->transform.worldMatrix.GetTranslate().y ,offset_.z };
	transform_.UpdateMatrix();
	camera_->SetPosition(transform_.translate);
	camera_->UpdateMatrices();
#ifdef _DEBUG
	Debug();
#endif // _DEBUG
}

void FollowCamera::Reset() {
	JSON_OPEN("Resources/Data/FollowCamera/followCamera.json");
	JSON_LOAD(offset_);
	JSON_CLOSE();

	transform_.translate = { offset_.x,offset_.y + player_->transform.worldMatrix.GetTranslate().y ,offset_.z };
	transform_.UpdateMatrix();
	camera_->SetPosition(transform_.translate);
	camera_->UpdateMatrices();
}

void FollowCamera::Debug() {
	ImGui::Begin("GameObject");
	if (ImGui::BeginMenu("FollowCamera")) {
		if (ImGui::TreeNode("Property")) {
			ImGui::DragFloat3("offset", &offset_.x, 0.1f);
			if (ImGui::Button("Save")) {
				JSON_OPEN("Resources/Data/FollowCamera/followCamera.json");
				JSON_SAVE(offset_);
				JSON_CLOSE();
			}

			ImGui::TreePop();
		}
		ImGui::EndMenu();
	}
	ImGui::End();
}
