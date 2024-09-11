#pragma once

#include <memory>

#include "Math/Camera.h"
#include "Player.h"

class TitleCamera {
public:
	void Initialize();
	void Update();

	void SetCamera(std::shared_ptr<Camera> camera) { camera_ = camera; }

	bool GetIsEasing() { return isEasingCamera_; }
private:
	void Debug();
	std::shared_ptr<Camera> camera_;
	Vector3 offset_;
	Vector3 startCameraPos_;
	Vector3 endCameraPos_;
	float time_;
	float transitionFrame_;
	bool isEasingCamera_;
};