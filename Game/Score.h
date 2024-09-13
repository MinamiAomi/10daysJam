#pragma once

#include <array>
#include <stdint.h>
#include <string>

#include "Math/Random.h"
#include "GameObject/GameObject.h"
#include "Graphics/Model.h"

class Player;
class Score {
public:
	enum State {
		OutGame,
		InGame,
		Result,
	};

	void Initialize();
	void Update();

	void InitializeInGame();
	void FinalizeInGame();

	void InitializeResultGame();
	void FinalizeResultGameGame();

	void Reset();

	void AddScore(int depth);

	bool GetIsClear() { return isClear_; }

	void SetParent(const Transform& parent) {
		timerTransform_.SetParent(&parent);
		blockTransform_.SetParent(&parent);
		mTransform_.SetParent(&blockTransform_);
		depthTransform_.SetParent(&parent);
		if (state_ == Result) {
			scoreTransform_.SetParent(&parent);
		}
		else {
			scoreTransform_.SetParent(nullptr);
		}
	}

	void SetPlayer(const std::shared_ptr<Player>& player) { player_ = player; }

	float GetEasingTime() { return resultEasingTime_; }
	const State& GetState() { return state_; }
private:
	void Debug();
	void ConversionSeconds();
	void UpdateTimer();
	void UpdateScore();

	std::shared_ptr<Player> player_;
	State state_;
	Random::RandomNumberGenerator rnd_;

	// スコアを最初に見せないため
	bool isFirst_;

	bool isClear_;

	int score_;
	int preScore_;
	int blockCount_;
	int preBlockCount_;
	int depthCount_;
	int preDepthCount_;

	// 位
	struct NumPlace {
		void Initialize(const std::string& modelName, int number);
		void Update();
		void UpdateTranslate();
		void ActiveModel(int num);
		void Reset(const std::string& name, int number);
		void Debug(const std::string& name);

		std::array<ModelInstance, 10> numberModel_;
		//float easingTime_;
		//float transformFrame_;
		//Quaternion startRotate_;
		//Quaternion endRotate_;

		Vector3 offset_;
		Transform transform_;

	};

	int limitTime_;
	int time_;
	int second_;
	int preSecond_;

	NumPlace onePlace_;
	NumPlace tenPlace_;
	Vector3 timerOffset_;
	Transform timerTransform_;

	NumPlace blockOnePlace_;
	NumPlace blockTenPlace_;
	NumPlace blockHundredPlace_;
	NumPlace blockThousandPlace_;
	Vector3 blockOffset_;
	Vector3 blockScoreOffset_;
	Transform blockTransform_;

	NumPlace depthOnePlace_;
	NumPlace depthTenPlace_;
	NumPlace depthHundredPlace_;
	NumPlace depthThousandPlace_;
	Vector3 depthOffset_;
	Vector3 depthScoreOffset_;
	Transform depthTransform_;

	NumPlace scoreOnePlace_;
	NumPlace scoreTenPlace_;
	NumPlace scoreHundredPlace_;
	NumPlace scoreThousandPlace_;
	NumPlace scoreTenThousandPlace_;
	Vector3 scoreOffset_;
	Transform scoreTransform_;

	ModelInstance m_;
	Vector3 mOffset_;
	Transform mTransform_;


	float resultEasingTime_;
	float resultTransitionFrame_;
};