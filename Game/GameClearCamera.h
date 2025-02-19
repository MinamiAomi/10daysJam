#pragma once

#include <memory>

#include "Math/Camera.h"
#include "Player.h"

class GameClearCamera {
public:
	void Initialize();
	void Update();

	void SetCameraPosition(float mapRowLength);

	std::shared_ptr<Camera> GetCamera() { return camera_; }
	void SetCamera(std::shared_ptr<Camera> camera) { camera_ = camera; }
	const Vector3& GetEndCameraPos() { return endCameraPos_; }

	void SetEasingTime(float time) { time_ = time; }
	Transform transform_;
private:
	void Debug();
	std::shared_ptr<Camera> camera_;
	
	Vector3 startCameraPos_;
	Vector3 endCameraPos_;
	float time_;
};