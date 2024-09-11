#pragma once

#include <stdint.h>

class Score {
public:
	void Initialize();
	void Update();

	void Reset();

	void AddScore(int depth);

	bool GetIsClear() { return isClear_; }
private:
	void Debug();
	void ConversionSeconds();
	int limitTime_;
	int time_;
	int second_;
	bool isClear_;

	int score_;
	int blockCount_;
	int depthCount_;
};