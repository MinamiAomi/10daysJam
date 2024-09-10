#pragma once

#include <list>

#include "Engine/Math/Random.h"

#include "Game/BulletManager.h"
#include "Enemy.h"

class EnemyManager {
public:
	void Initialize();
	void Update();

	void Reset();

	void SetBulletManager(std::shared_ptr<BulletManager> bulletManager) { bulletManager_ = bulletManager; }
private:
	void Create();
	Random::RandomNumberGenerator rnd_;
	std::list<std::shared_ptr<Enemy>> enemies_;      
	std::shared_ptr<BulletManager> bulletManager_;
	int32_t interval_ = 90;
	int32_t time_ = 0;
};