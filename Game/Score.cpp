#include "Score.h"

#include <algorithm>

#include "Framework/AssetManager.h"

#include "Engine/Graphics/ImGuiManager.h"
#include "File/JsonHelper.h"

void Score::Initialize() {
	onePlace_.transform_.SetParent(&timerTransform_);
	onePlace_.Initialize("onePlace", Quaternion::identity);
	tenPlace_.transform_.SetParent(&timerTransform_);
	tenPlace_.Initialize("tenPlace", Quaternion::MakeForZAxis(36.0f * (second_ / 10.0f - 10.0f) * Math::ToRadian));
	Reset();
}

void Score::Update() {
	time_++;
	if (time_ > limitTime_) {
		isClear_ = true;
	}
	score_ = blockCount_ * depthCount_;
	ConversionSeconds();
	UpdateTimer();
	timerTransform_.translate = timerOffset_;
	timerTransform_.UpdateMatrix();
#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

void Score::Reset() {
	JSON_OPEN("Resources/Data/Score/score.json");
	JSON_LOAD(limitTime_);
	JSON_LOAD(timerOffset_);
	JSON_CLOSE();

	score_ = 0;
	time_ = 0;
	isClear_ = false;
	blockCount_ = 0;
	depthCount_ = 0;
	ConversionSeconds();
	preSecond_ = second_;
	onePlace_.transform_.SetParent(&timerTransform_);
	onePlace_.Reset("onePlace", Quaternion::identity);
	tenPlace_.transform_.SetParent(&timerTransform_);
	tenPlace_.Initialize("tenPlace", Quaternion::MakeForZAxis(36.0f * (second_ / 10.0f - 10.0f) * Math::ToRadian));
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
		ImGui::DragInt("second", &second_);
		if (ImGui::TreeNode("Property")) {
			ImGui::DragInt("limitTime_", &limitTime_);
			ImGui::DragFloat3("timerOffset_", &timerOffset_.x);
			if (ImGui::Button("Save")) {
				JSON_OPEN("Resources/Data/Score/score.json");
				JSON_SAVE(limitTime_);
				JSON_SAVE(timerOffset_);
				JSON_CLOSE();
			}

			ImGui::TreePop();
		}
		ImGui::EndMenu();
	}
	ImGui::End();
	onePlace_.Debug("onePlace");
	tenPlace_.Debug("tenPlace");
}

void Score::ConversionSeconds() {
	second_ = (limitTime_ - time_) / 60;
}

void Score::UpdateTimer() {
	// 秒数が前回と違ったら
	if (preSecond_ != second_) {
		onePlace_.startRotate_ = onePlace_.transform_.rotate;
		onePlace_.endRotate_ = onePlace_.transform_.rotate * Quaternion::MakeForZAxis(-36.0f * Math::ToRadian);
		onePlace_.easingTime_ = 0.0f;
		// 十の桁が動いたら
		if ((preSecond_ / 10) != (second_ / 10)) {
			tenPlace_.startRotate_ = tenPlace_.transform_.rotate;
			tenPlace_.endRotate_ = tenPlace_.transform_.rotate * Quaternion::MakeForZAxis(-36.0f * Math::ToRadian);
			tenPlace_.easingTime_ = 0.0f;
		}
	}
	// 一の位
	onePlace_.Update();
	tenPlace_.Update();
	preSecond_ = second_;
}

void Score::NumPlace::Initialize(const std::string& modelName, const Quaternion& rotate) {
	model_.SetModel(AssetManager::GetInstance()->FindModel(modelName));
	Reset(modelName, rotate);
}

void Score::NumPlace::Update() {
	easingTime_ += 1.0f / transformFrame_;
	easingTime_ = std::clamp(easingTime_, 0.0f, 1.0f);
	transform_.rotate = Quaternion::Slerp(easingTime_, startRotate_, endRotate_);
	transform_.UpdateMatrix();
	model_.SetWorldMatrix(transform_.worldMatrix);
}

void Score::NumPlace::Reset(const std::string& name, const Quaternion& rotate) {
	JSON_OPEN("Resources/Data/Score/score.json");
	JSON_LOAD_BY_NAME(name + "TransformFrame_", transformFrame_);
	JSON_CLOSE();
	easingTime_ = 0.0f;
	startRotate_ = rotate;
	endRotate_ = rotate;
	transform_.translate = Vector3::zero;
	transform_.rotate = rotate;
	isUpdate_ = false;
}

void Score::NumPlace::Debug(const std::string& name) {
	if (ImGui::TreeNode(name.c_str())) {

		ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
		ImGui::DragFloat("easingTime", &easingTime_);
		if (ImGui::TreeNode("Property")) {
			ImGui::DragFloat("transformFrame", &transformFrame_);
			if (ImGui::Button("Save")) {
				JSON_OPEN("Resources/Data/Score/score.json");
				JSON_SAVE_BY_NAME(name + "TransformFrame_", transformFrame_);
				JSON_CLOSE();
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
