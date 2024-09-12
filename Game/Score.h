#pragma once

#include <stdint.h>
#include <string>

#include "GameObject/GameObject.h"
#include "Graphics/Model.h"

class Score {
public:
	void Initialize();
	void Update();

	void Reset();

	void AddScore(int depth);

	bool GetIsClear() { return isClear_; }

	void SetParent(const Transform& parent) { 
		timerTransform_.SetParent(&parent);
	}
private:
	void Debug();
	void ConversionSeconds();
	void UpdateTimer();

	bool isClear_;

	int score_;
	int blockCount_;
	int depthCount_;

	// 位
	struct NumPlace {
		float easingTime_;
		float transformFrame_;

		Quaternion startRotate_;
		Quaternion endRotate_;

		Transform transform_;
		ModelInstance model_;

		// trueアップデート開始
		bool isUpdate_;

		void Initialize(const std::string& modelName,const Quaternion& rotate);
		void Update();
		void Reset(const std::string& name, const Quaternion& rotate);
		void Debug(const std::string& name);
	};

	int limitTime_;
	int time_;
	int second_;
	int preSecond_;

	NumPlace onePlace_;
	NumPlace tenPlace_;
	Vector3 timerOffset_;
	Transform timerTransform_;
};