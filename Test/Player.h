#pragma once

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "PlayerBulletManager.h"

class Player {
public:
	void Initialize();
	void Update();

private:
	Transform transform_;
	ModelInstance model_;
#pragma region Bullet
	std::shared_ptr<PlayerBulletManager> bulletManager_;
	// インターバル
	float fireInterval_ = 30.0f;
	float fireTime_ = 0.0f;
#pragma endregion 
	//ModelInstance hammer_;
	//std::shared_ptr<Skeleton> skeleton_;
	//std::shared_ptr<Animation> walk_;
	//float animationTime_;

};