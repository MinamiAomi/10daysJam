#pragma once

#include <array>
#include <stdint.h>
#include <string>

#include "GameObject/GameObject.h"
#include "Graphics/Model.h"

class Player;
class Score {
public:
	void Initialize();
	void Update();

	void Reset();

	void AddScore(int depth);

	bool GetIsClear() { return isClear_; }

	void SetParent(const Transform& parent) {
		timerTransform_.SetParent(&parent);
		scoreTransform_.SetParent(&parent);
	}

	void SetPlayer(const std::shared_ptr<Player>& player) { player_ = player; }
private:
	void Debug();
	void ConversionSeconds();
	void UpdateTimer();
	void UpdateScore();
	std::shared_ptr<Player> player_;

	bool isClear_;
	bool isStart_;

	int score_;
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
		void Reset(const std::string& name,int number);
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
	Vector3 scoreOffset_;
	Transform scoreTransform_;
};