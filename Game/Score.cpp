#include "Score.h"

#include <algorithm>

#include "Framework/AssetManager.h"

#include "Engine/Graphics/ImGuiManager.h"
#include "File/JsonHelper.h"

#include "Player.h"

void Score::Initialize() {
	isFirst_ = true;
	score_ = 0;

	onePlace_.transform_.SetParent(&timerTransform_);
	onePlace_.Initialize("onePlace", 9);
	tenPlace_.transform_.SetParent(&timerTransform_);
	tenPlace_.Initialize("tenPlace", 0);

	blockOnePlace_.transform_.SetParent(&blockTransform_);
	blockOnePlace_.Initialize("blockOnePlace", 0);
	blockTenPlace_.transform_.SetParent(&blockTransform_);
	blockTenPlace_.Initialize("blockTenPlace", 0);
	blockHundredPlace_.transform_.SetParent(&blockTransform_);
	blockHundredPlace_.Initialize("blockHundredPlace", 0);
	blockThousandPlace_.transform_.SetParent(&blockTransform_);
	blockThousandPlace_.Initialize("blockThousandPlace", 0);

	depthOnePlace_.transform_.SetParent(&depthTransform_);
	depthOnePlace_.Initialize("depthOnePlace", 0);
	depthTenPlace_.transform_.SetParent(&depthTransform_);
	depthTenPlace_.Initialize("depthTenPlace", 0);
	depthHundredPlace_.transform_.SetParent(&depthTransform_);
	depthHundredPlace_.Initialize("depthHundredPlace", 0);
	depthThousandPlace_.transform_.SetParent(&depthTransform_);
	depthThousandPlace_.Initialize("depthThousandPlace", 0);

	depthOnePlace_.transform_.SetParent(&depthTransform_);
	depthOnePlace_.Initialize("depthOnePlace", 0);
	depthTenPlace_.transform_.SetParent(&depthTransform_);
	depthTenPlace_.Initialize("depthTenPlace", 0);
	depthHundredPlace_.transform_.SetParent(&depthTransform_);
	depthHundredPlace_.Initialize("depthHundredPlace", 0);
	depthThousandPlace_.transform_.SetParent(&depthTransform_);
	depthThousandPlace_.Initialize("depthThousandPlace", 0);

	scoreOnePlace_.transform_.SetParent(&scoreTransform_);
	scoreOnePlace_.Initialize("scoreOnePlace", 0);
	scoreTenPlace_.transform_.SetParent(&scoreTransform_);
	scoreTenPlace_.Initialize("scoreTenPlace", 0);
	scoreHundredPlace_.transform_.SetParent(&scoreTransform_);
	scoreHundredPlace_.Initialize("scoreHundredPlace", 0);
	scoreThousandPlace_.transform_.SetParent(&scoreTransform_);
	scoreThousandPlace_.Initialize("scoreThousandPlace", 0);
	scoreTenThousandPlace_.transform_.SetParent(&scoreTransform_);
	scoreTenThousandPlace_.Initialize("scoreTenThousandPlace", 0);

	Reset();
}

void Score::Update() {
	switch (state_) {
	case Score::OutGame:
		if (player_->transform.translate.y <= 0.0f) {
			//FinalizeOutGameGame();
			InitializeInGame();
			state_ = InGame;
		}
		//scoreTransform_.translate = scoreOffset_;

		scoreTransform_.UpdateMatrix();

		scoreOnePlace_.UpdateTranslate();
		scoreTenPlace_.UpdateTranslate();
		scoreHundredPlace_.UpdateTranslate();
		scoreThousandPlace_.UpdateTranslate();
		scoreTenThousandPlace_.UpdateTranslate();

		break;
	case Score::InGame:
		time_++;
		if (time_ > limitTime_) {
			isClear_ = true;
			InitializeResultGame();
			state_ = Result;
		}
		score_ = blockCount_ * depthCount_;
		ConversionSeconds();
		UpdateScore();
		UpdateTimer();

		timerTransform_.translate = timerOffset_;
		blockTransform_.translate = blockOffset_;
		depthTransform_.translate = depthOffset_;

		timerTransform_.UpdateMatrix();
		blockTransform_.UpdateMatrix();
		depthTransform_.UpdateMatrix();

		onePlace_.UpdateTranslate();
		tenPlace_.UpdateTranslate();

		blockOnePlace_.UpdateTranslate();
		blockTenPlace_.UpdateTranslate();
		blockHundredPlace_.UpdateTranslate();
		blockThousandPlace_.UpdateTranslate();

		depthOnePlace_.UpdateTranslate();
		depthTenPlace_.UpdateTranslate();
		depthHundredPlace_.UpdateTranslate();
		depthThousandPlace_.UpdateTranslate();

		break;
	case Result:
	{
		// 1触れ遅らせる
		if (resultEasingTime_ >= 1.0f) {
			blockOnePlace_.ActiveModel(10);
			blockTenPlace_.ActiveModel(10);
			blockHundredPlace_.ActiveModel(10);
			blockThousandPlace_.ActiveModel(10);

			depthOnePlace_.ActiveModel(10);
			depthTenPlace_.ActiveModel(10);
			depthHundredPlace_.ActiveModel(10);
			depthThousandPlace_.ActiveModel(10);

			// スコア
			scoreOnePlace_.ActiveModel(score_ % 10);
			scoreTenPlace_.ActiveModel((score_ / 10) % 10);
			scoreHundredPlace_.ActiveModel((score_ / 100) % 10);
			scoreThousandPlace_.ActiveModel((score_ / 1000) % 10);
			scoreTenThousandPlace_.ActiveModel((score_ / 10000) % 10);

			scoreTransform_.SetParent(nullptr);
			scoreTransform_.UpdateMatrix();
			scoreOnePlace_.UpdateTranslate();
			scoreTenPlace_.UpdateTranslate();
			scoreHundredPlace_.UpdateTranslate();
			scoreThousandPlace_.UpdateTranslate();
			scoreTenThousandPlace_.UpdateTranslate();
			state_ = OutGame;
		}

		resultEasingTime_ += 1.0f / resultTransitionFrame_;
		resultEasingTime_ = std::clamp(resultEasingTime_,0.0f,1.0f);
		blockTransform_.translate = Vector3::Lerp(resultEasingTime_, blockOffset_, blockScoreOffset_);
		depthTransform_.translate = Vector3::Lerp(resultEasingTime_, depthOffset_, depthScoreOffset_);
		scoreTransform_.translate = scoreOffset_;

		scoreOnePlace_.ActiveModel(rnd_.NextIntRange(0, 9));
		scoreTenPlace_.ActiveModel(rnd_.NextIntRange(0, 9));
		scoreHundredPlace_.ActiveModel(rnd_.NextIntRange(0, 9));
		scoreThousandPlace_.ActiveModel(rnd_.NextIntRange(0, 9));
		scoreTenThousandPlace_.ActiveModel(rnd_.NextIntRange(0, 9));

		blockTransform_.UpdateMatrix();
		depthTransform_.UpdateMatrix();
		scoreTransform_.UpdateMatrix();

		blockOnePlace_.UpdateTranslate();
		blockTenPlace_.UpdateTranslate();
		blockHundredPlace_.UpdateTranslate();
		blockThousandPlace_.UpdateTranslate();

		depthOnePlace_.UpdateTranslate();
		depthTenPlace_.UpdateTranslate();
		depthHundredPlace_.UpdateTranslate();
		depthThousandPlace_.UpdateTranslate();

		scoreOnePlace_.UpdateTranslate();
		scoreTenPlace_.UpdateTranslate();
		scoreHundredPlace_.UpdateTranslate();
		scoreThousandPlace_.UpdateTranslate();
		scoreTenThousandPlace_.UpdateTranslate();
	}
	break;
	default:
		break;
	}
#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

void Score::InitializeInGame() {
	onePlace_.transform_.SetParent(&timerTransform_);
	onePlace_.Reset("onePlace", limitTime_ % 10);
	tenPlace_.transform_.SetParent(&timerTransform_);
	tenPlace_.Reset("tenPlace", (limitTime_ / 10) % 10);

	blockOnePlace_.Reset("blockOnePlace", 0);
	blockOnePlace_.transform_.SetParent(&blockTransform_);
	blockTenPlace_.Reset("blockTenPlace", 0);
	blockTenPlace_.transform_.SetParent(&blockTransform_);
	blockHundredPlace_.Reset("blockHundredPlace", 0);
	blockHundredPlace_.transform_.SetParent(&blockTransform_);
	blockThousandPlace_.Reset("blockThousandPlace", 0);
	blockThousandPlace_.transform_.SetParent(&blockTransform_);

	depthOnePlace_.Reset("depthOnePlace", 0);
	depthOnePlace_.transform_.SetParent(&depthTransform_);
	depthTenPlace_.Reset("depthTenPlace", 0);
	depthTenPlace_.transform_.SetParent(&depthTransform_);
	depthHundredPlace_.Reset("depthHundredPlace", 0);
	depthHundredPlace_.transform_.SetParent(&depthTransform_);
	depthThousandPlace_.Reset("depthThousandPlace", 0);
	depthThousandPlace_.transform_.SetParent(&depthTransform_);

	timerTransform_.translate = timerOffset_;
	blockTransform_.translate = blockOffset_;
	depthTransform_.translate = depthOffset_;

	timerTransform_.UpdateMatrix();
	blockTransform_.UpdateMatrix();
	depthTransform_.UpdateMatrix();

	onePlace_.UpdateTranslate();
	tenPlace_.UpdateTranslate();

	blockOnePlace_.UpdateTranslate();
	blockTenPlace_.UpdateTranslate();
	blockHundredPlace_.UpdateTranslate();
	blockThousandPlace_.UpdateTranslate();

	depthOnePlace_.UpdateTranslate();
	depthTenPlace_.UpdateTranslate();
	depthHundredPlace_.UpdateTranslate();
	depthThousandPlace_.UpdateTranslate();
}

void Score::FinalizeInGame() {}

void Score::InitializeResultGame() {
	scoreTransform_.SetParent(blockTransform_.GetParent());
	scoreTransform_.translate = scoreOffset_;
	scoreTransform_.UpdateMatrix();
	scoreOnePlace_.UpdateTranslate();
	scoreTenPlace_.UpdateTranslate();
	scoreHundredPlace_.UpdateTranslate();
	scoreThousandPlace_.UpdateTranslate();
	scoreTenThousandPlace_.UpdateTranslate();
	resultEasingTime_ = 0.0f;
}

void Score::FinalizeResultGameGame() {
	/*scoreOnePlace_.Reset("scoreOnePlace", 10);
	scoreOnePlace_.transform_.SetParent(&scoreTransform_);
	scoreTenPlace_.Reset("scoreTenPlace", 10);
	scoreTenPlace_.transform_.SetParent(&scoreTransform_);
	scoreHundredPlace_.Reset("scoreHundredPlace", 10);
	scoreHundredPlace_.transform_.SetParent(&scoreTransform_);
	scoreThousandPlace_.Reset("scoreThousandPlace", 10);
	scoreThousandPlace_.transform_.SetParent(&scoreTransform_);
	scoreTenThousandPlace_.Reset("scoreTenThousandPlace", 10);
	scoreTenThousandPlace_.transform_.SetParent(&scoreTransform_);*/
}

void Score::Reset() {
	JSON_OPEN("Resources/Data/Score/score.json");
	JSON_LOAD(limitTime_);
	JSON_LOAD(timerOffset_);
	JSON_LOAD(blockOffset_);
	JSON_LOAD(blockScoreOffset_);
	JSON_LOAD(depthOffset_);
	JSON_LOAD(depthScoreOffset_);
	JSON_LOAD(scoreOffset_);
	JSON_LOAD(resultTransitionFrame_);
	JSON_CLOSE();

	state_ = OutGame;

	resultEasingTime_ = 0.0f;
	time_ = 0;
	isClear_ = false;
	blockCount_ = 0;
	preBlockCount_ = blockCount_;
	depthCount_ = 0;
	preDepthCount_ = depthCount_;
	ConversionSeconds();
	preSecond_ = second_;

	onePlace_.transform_.SetParent(&timerTransform_);
	onePlace_.Reset("onePlace", 10);
	tenPlace_.transform_.SetParent(&timerTransform_);
	tenPlace_.Reset("tenPlace", 10);

	blockOnePlace_.Reset("blockOnePlace", 10);
	blockOnePlace_.transform_.SetParent(&blockTransform_);
	blockTenPlace_.Reset("blockTenPlace", 10);
	blockTenPlace_.transform_.SetParent(&blockTransform_);
	blockHundredPlace_.Reset("blockHundredPlace", 10);
	blockHundredPlace_.transform_.SetParent(&blockTransform_);
	blockThousandPlace_.Reset("blockThousandPlace", 10);
	blockThousandPlace_.transform_.SetParent(&blockTransform_);

	depthOnePlace_.Reset("depthOnePlace", 10);
	depthOnePlace_.transform_.SetParent(&depthTransform_);
	depthTenPlace_.Reset("depthTenPlace", 10);
	depthTenPlace_.transform_.SetParent(&depthTransform_);
	depthHundredPlace_.Reset("depthHundredPlace", 10);
	depthHundredPlace_.transform_.SetParent(&depthTransform_);
	depthThousandPlace_.Reset("depthThousandPlace", 10);
	depthThousandPlace_.transform_.SetParent(&depthTransform_);
	if (isFirst_) {
		isFirst_ = false;
		scoreOnePlace_.Reset("scoreOnePlace", 10);
		scoreOnePlace_.transform_.SetParent(&scoreTransform_);
		scoreTenPlace_.Reset("scoreTenPlace", 10);
		scoreTenPlace_.transform_.SetParent(&scoreTransform_);
		scoreHundredPlace_.Reset("scoreHundredPlace", 10);
		scoreHundredPlace_.transform_.SetParent(&scoreTransform_);
		scoreThousandPlace_.Reset("scoreThousandPlace", 10);
		scoreThousandPlace_.transform_.SetParent(&scoreTransform_);
		scoreTenThousandPlace_.Reset("scoreTenThousandPlace", 10);
		scoreTenThousandPlace_.transform_.SetParent(&scoreTransform_);
	}
	else {
		scoreOnePlace_.Reset("scoreOnePlace", score_ % 10);
		scoreOnePlace_.transform_.SetParent(&scoreTransform_);
		scoreTenPlace_.Reset("scoreTenPlace", (score_ / 10) % 10);
		scoreTenPlace_.transform_.SetParent(&scoreTransform_);
		scoreHundredPlace_.Reset("scoreHundredPlace", (score_ / 100) % 10);
		scoreHundredPlace_.transform_.SetParent(&scoreTransform_);
		scoreThousandPlace_.Reset("scoreThousandPlace", (score_ / 1000) % 10);
		scoreThousandPlace_.transform_.SetParent(&scoreTransform_);
		scoreTenThousandPlace_.Reset("scoreTenThousandPlace", (score_ / 10000) % 10);
		scoreTenThousandPlace_.transform_.SetParent(&scoreTransform_);
	}
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
			ImGui::DragFloat3("blockOffset_", &blockOffset_.x);
			ImGui::DragFloat3("depthOffset_", &depthOffset_.x);
			ImGui::DragFloat3("scoreOffset_", &scoreOffset_.x);
			ImGui::DragFloat3("blockScoreOffset_", &blockScoreOffset_.x);
			ImGui::DragFloat3("depthScoreOffset_", &depthScoreOffset_.x);
			ImGui::DragFloat("resultTransitionFrame_", &resultTransitionFrame_, 0.1f);
			if (ImGui::Button("Save")) {
				JSON_OPEN("Resources/Data/Score/score.json");
				JSON_SAVE(limitTime_);
				JSON_SAVE(timerOffset_);
				JSON_SAVE(blockOffset_);
				JSON_SAVE(blockScoreOffset_);
				JSON_SAVE(depthOffset_);
				JSON_SAVE(depthScoreOffset_);
				JSON_SAVE(scoreOffset_);
				JSON_SAVE(resultTransitionFrame_);
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

	depthOnePlace_.Debug("depthOnePlace");
	depthTenPlace_.Debug("depthTenPlace");
	depthHundredPlace_.Debug("depthHundredPlace");
	depthThousandPlace_.Debug("depthThousandPlace");


	scoreOnePlace_.Debug("scoreOnePlace");
	scoreTenPlace_.Debug("scoreTenPlace");
	scoreHundredPlace_.Debug("scoreHundredPlace");
	scoreThousandPlace_.Debug("scoreThousandPlace");
	scoreTenThousandPlace_.Debug("scoreTenThousandPlace");
}

void Score::ConversionSeconds() {
	second_ = (limitTime_ - time_) / 60;
	second_ = std::clamp(second_, 0, limitTime_);
}

void Score::UpdateTimer() {
	// 秒数が前回と違ったら
	if (preSecond_ != second_) {
		onePlace_.ActiveModel(second_ % 10);

		// 十の桁が動いたら
		if ((preSecond_ / 10) != (second_ / 10)) {
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
		blockOnePlace_.ActiveModel(blockCount_ % 10);
		// 十の桁が動いたら
		if ((preBlockCount_ / 10) % 10 != (blockCount_ / 10) % 10) {
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

	if (preDepthCount_ != depthCount_) {
		depthOnePlace_.ActiveModel(depthCount_ % 10);
		// 十の桁が動いたら
		if ((preDepthCount_ / 10) % 10 != (depthCount_ / 10) % 10) {
			depthTenPlace_.ActiveModel((depthCount_ / 10) % 10);
		}
		// 百の位
		if ((preDepthCount_ / 100) % 10 != (depthCount_ / 100) % 10) {
			depthHundredPlace_.ActiveModel((depthCount_ / 100) % 10);
		}
		// 千の位
		if ((preDepthCount_ / 1000) % 10 != (depthCount_ / 1000) % 10) {
			depthThousandPlace_.ActiveModel((depthCount_ / 1000) % 10);
		}
	}


	if (preScore_ != score_ && state_ == OutGame) {
		scoreOnePlace_.ActiveModel(score_ % 10);
		// 十の桁が動いたら
		if ((preScore_ / 10) % 10 != (score_ / 10) % 10) {
			scoreTenPlace_.ActiveModel((score_ / 10) % 10);
		}
		// 百の位
		if ((preScore_ / 100) % 10 != (score_ / 100) % 10) {
			scoreHundredPlace_.ActiveModel((score_ / 100) % 10);
		}
		// 千の位
		if ((preScore_ / 1000) % 10 != (score_ / 1000) % 10) {
			scoreThousandPlace_.ActiveModel((score_ / 1000) % 10);
		}
		if ((preScore_ / 10000) % 10 != (score_ / 10000) % 10) {
			scoreTenThousandPlace_.ActiveModel((score_ / 10000) % 10);
		}
	}

	blockOnePlace_.Update();
	blockTenPlace_.Update();
	blockHundredPlace_.Update();
	blockThousandPlace_.Update();

	depthOnePlace_.Update();
	depthTenPlace_.Update();
	depthHundredPlace_.Update();
	depthThousandPlace_.Update();

	scoreOnePlace_.Update();
	scoreTenPlace_.Update();
	scoreHundredPlace_.Update();
	scoreThousandPlace_.Update();
	scoreTenThousandPlace_.Update();

	preBlockCount_ = blockCount_;
	preDepthCount_ = depthCount_;
	preScore_ = score_;
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
	if (num != 10) {
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
	else {
		for (auto& model : numberModel_) {
			model.SetIsActive(false);
		}
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
