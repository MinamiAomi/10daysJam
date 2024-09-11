#include "Score.h"

#include <algorithm>

#include "Engine/Graphics/ImGuiManager.h"
#include "File/JsonHelper.h"

void Score::Initialize() {
	Reset();
}

void Score::Update() {
	time_++;
	if (time_ > limitTime_) {
		isClear_ = true;
	}
#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

void Score::Reset() {
	JSON_OPEN("Resources/Data/Score/score.json");
	JSON_LOAD(limitTime_);
	JSON_CLOSE();

	score_ = 0;
	time_ = 0;
	isClear_ = false;
	blockCount_ = 0;
	depthCount_ = 0;
}

void Score::AddScore(int depth) {
	blockCount_++;
	depthCount_ = (std::max)(depthCount_, depth);
}

void Score::Debug() {
	ImGui::Begin("GameObject");
	if (ImGui::BeginMenu("Score")) {
		ImGui::DragInt("score", &score_);
		ImGui::DragInt("blockCount", &blockCount_);
		ImGui::DragInt("depthCount", &depthCount_);
		if (ImGui::TreeNode("Property")) {
			ImGui::DragInt("limitTime_", &limitTime_);
			if (ImGui::Button("Save")) {
				JSON_OPEN("Resources/Data/Score/score.json");
				JSON_SAVE(limitTime_);
				JSON_CLOSE();
			}

			ImGui::TreePop();
		}
		ImGui::EndMenu();
	}
	ImGui::End();
}
