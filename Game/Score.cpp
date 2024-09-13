#include "Score.h"

#include <algorithm>

#include "Framework/AssetManager.h"

#include "Engine/Graphics/ImGuiManager.h"
#include "File/JsonHelper.h"

#include "Player.h"

void Score::Initialize() {
	onePlace_.transform_.SetParent(&timerTransform_);
	onePlace_.Initialize("onePlace", 9);
	tenPlace_.transform_.SetParent(&timerTransform_);
	tenPlace_.Initialize("tenPlace", 0);
	blockOnePlace_.transform_.SetParent(&scoreTransform_);
	blockOnePlace_.Initialize("blockOnePlace", 0);
	blockTenPlace_.transform_.SetParent(&scoreTransform_);
	blockTenPlace_.Initialize("blockTenPlace", 0);
	blockHundredPlace_.transform_.SetParent(&scoreTransform_);
	blockHundredPlace_.Initialize("blockHundredPlace", 0);
	blockThousandPlace_.transform_.SetParent(&scoreTransform_);
	blockThousandPlace_.Initialize("blockThousandPlace", 0);
	Reset();
}

void Score::Update() {
	if (isStart_) {
		time_++;
		if (time_ > limitTime_) {
			isClear_ = true;
		}
		score_ = blockCount_ * depthCount_;
		ConversionSeconds();
		UpdateScore();
		UpdateTimer();
	}
	else {
		if (player_->transform.translate.y <= 0.0f) {
			isStart_ = true;
		}
	}

	timerTransform_.translate = timerOffset_;
	scoreTransform_.translate = scoreOffset_;

	timerTransform_.UpdateMatrix();
	scoreTransform_.UpdateMatrix();

	onePlace_.UpdateTranslate();
	tenPlace_.UpdateTranslate();

	blockOnePlace_.UpdateTranslate();
	blockTenPlace_.UpdateTranslate();
	blockHundredPlace_.UpdateTranslate();
	blockThousandPlace_.UpdateTranslate();
#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

void Score::Reset() {
	JSON_OPEN("Resources/Data/Score/score.json");
	JSON_LOAD(limitTime_);
	JSON_LOAD(timerOffset_);
	JSON_LOAD(scoreOffset_);
	JSON_CLOSE();

	score_ = 0;
	time_ = 0;
	isClear_ = false;
	isStart_ = false;
	blockCount_ = 0;
	preBlockCount_ = blockCount_;
	depthCount_ = 0;
	preDepthCount_ = depthCount_;
	ConversionSeconds();
	preSecond_ = second_;

	onePlace_.transform_.SetParent(&timerTransform_);
	onePlace_.Reset("onePlace", limitTime_ % 60);
	tenPlace_.transform_.SetParent(&timerTransform_);
	tenPlace_.Reset("tenPlace", limitTime_ / 600);

	blockOnePlace_.Reset("blockOnePlace", 0);
	blockOnePlace_.transform_.SetParent(&scoreTransform_);
	blockTenPlace_.Reset("blockTenPlace", 0);
	blockTenPlace_.transform_.SetParent(&scoreTransform_);
	blockHundredPlace_.Reset("blockHundredPlace", 0);
	blockHundredPlace_.transform_.SetParent(&scoreTransform_);
	blockThousandPlace_.Reset("blockThousandPlace", 0);
	blockThousandPlace_.transform_.SetParent(&scoreTransform_);
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
			ImGui::DragFloat3("scoreOffset_", &scoreOffset_.x);
			if (ImGui::Button("Save")) {
				JSON_OPEN("Resources/Data/Score/score.json");
				JSON_SAVE(limitTime_);
				JSON_SAVE(timerOffset_);
				JSON_SAVE(scoreOffset_);
				JSON_CLOSE();
			}

			ImGui::TreePop();
		}
		ImGui::EndMenu();
	}
	ImGui::End();
	onePlace_.Debug("onePlace");
	tenPlace_.Debug("tenPlace");
	blockOnePlace_.Debug("blockOnePlace");
	blockTenPlace_.Debug("blockTenPlace");
	blockHundredPlace_.Debug("blockHundredPlace");
	blockThousandPlace_.Debug("blockThousandPlace");
}

void Score::ConversionSeconds() {
	second_ = (limitTime_ - time_) / 60;
}

void Score::UpdateTimer() {
	// 秒数が前回と違ったら
	if (preSecond_ != second_) {
		//onePlace_.startRotate_ = onePlace_.transform_.rotate;
		//onePlace_.endRotate_ = onePlace_.transform_.rotate * Quaternion::MakeForZAxis(-36.0f * Math::ToRadian);
		//onePlace_.easingTime_ = 0.0f;
		onePlace_.ActiveModel(second_ % 10);

		// 十の桁が動いたら
		if ((preSecond_ / 10) != (second_ / 10)) {
			//tenPlace_.startRotate_ = tenPlace_.transform_.rotate;
			//tenPlace_.endRotate_ = tenPlace_.transform_.rotate * Quaternion::MakeForZAxis(-36.0f * Math::ToRadian);
			//tenPlace_.easingTime_ = 0.0f;
			tenPlace_.ActiveModel((second_ / 10) % 10);
		}
	}
	// 一の位
	onePlace_.Update();
	tenPlace_.Update();
	preSecond_ = second_;
}

void Score::UpdateScore() {
	if (preBlockCount_ != blockCount_) {
		//scoreOnePlace_.startRotate_ = scoreOnePlace_.transform_.rotate;
		//scoreOnePlace_.endRotate_ = Quaternion::MakeForZAxis(-36.0f * (blockCount_ % 10) * Math::ToRadian);
		//scoreOnePlace_.easingTime_ = 0.0f;
		blockOnePlace_.ActiveModel(blockCount_ % 10);
		// 十の桁が動いたら
		if ((preBlockCount_ / 10) % 10 != (blockCount_ / 10) % 10) {
			//scoreTenPlace_.startRotate_ = scoreTenPlace_.transform_.rotate;
			//scoreTenPlace_.endRotate_ = Quaternion::MakeForZAxis(-36.0f * ((blockCount_ / 10) % 10) * Math::ToRadian);
			//scoreTenPlace_.easingTime_ = 0.0f;
			blockTenPlace_.ActiveModel((blockCount_ / 10) % 10);
		}
		// 百の位
		if ((preBlockCount_ / 100) % 10 != (blockCount_ / 100) % 10) {
			blockHundredPlace_.ActiveModel((blockCount_ / 100) % 10);
		}
		// 千の位
		if ((preBlockCount_ / 1000) % 10 != (blockCount_ / 1000) % 10) {
			blockThousandPlace_.ActiveModel((blockCount_ / 1000) % 10);
		}
	}

	blockOnePlace_.Update();
	blockTenPlace_.Update();
	blockHundredPlace_.Update();
	blockThousandPlace_.Update();
	preBlockCount_ = blockCount_;
}

void Score::NumPlace::Initialize(const std::string& modelName, int number) {
	for (uint32_t i = 0; i < 10; i++) {
		numberModel_.at(i).SetModel(AssetManager::GetInstance()->FindModel("number_" + std::to_string(i)));
		numberModel_.at(i).SetIsActive(false);
	}
	Reset(modelName, number);
}

void Score::NumPlace::Update() {
	//easingTime_ += 1.0f / transformFrame_;
	//easingTime_ = std::clamp(easingTime_, 0.0f, 1.0f);
	//transform_.rotate = Quaternion::Slerp(easingTime_, startRotate_, endRotate_);
}

void Score::NumPlace::UpdateTranslate() {
	transform_.UpdateMatrix();
	for (auto& model : numberModel_) {
		if (model.IsActive()) {
			model.SetWorldMatrix(transform_.worldMatrix);
		}
	}
}

void Score::NumPlace::ActiveModel(int num) {
	for (int i = 0; auto & model: numberModel_) {
		if (i == num) {
			model.SetIsActive(true);
		}
		else {
			model.SetIsActive(false);
		}
		i++;
	}
}

void Score::NumPlace::Reset(const std::string& name, int number) {
	JSON_OPEN("Resources/Data/Score/score.json");
	//JSON_LOAD_BY_NAME(name + "TransformFrame_", transformFrame_);
	JSON_LOAD_BY_NAME(name + "Offset_", offset_);
	JSON_CLOSE();
	//easingTime_ = 0.0f;
	//startRotate_ = rotate;
	//endRotate_ = rotate;
	//transform_.rotate = rotate;
	for (int i = 0; i < 10; i++) {
		if (i == number) {
			numberModel_.at(i).SetIsActive(true);
		}
		else {
			numberModel_.at(i).SetIsActive(false);
		}
	}


	transform_.translate = offset_;
	UpdateTranslate();
}

void Score::NumPlace::Debug(const std::string& name) {
	if (ImGui::TreeNode(name.c_str())) {

		ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
		//ImGui::DragFloat("easingTime", &easingTime_);
		if (ImGui::TreeNode("Property")) {
			//ImGui::DragFloat("transformFrame", &transformFrame_);
			ImGui::DragFloat3("offset", &offset_.x);
			if (ImGui::Button("Save")) {
				JSON_OPEN("Resources/Data/Score/score.json");
				//JSON_SAVE_BY_NAME(name + "TransformFrame_", transformFrame_);
				JSON_SAVE_BY_NAME(name + "Offset_", offset_);
				JSON_CLOSE();
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
