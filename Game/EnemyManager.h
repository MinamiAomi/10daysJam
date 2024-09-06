#pragma once

#include <list>

#include "Engine/Math/Random.h"

#include "Enemy.h"

class EnemyManager {
public:
	void Initialize();
	void Update();

	void Reset();
private:
	void Create();
	Random::RandomNumberGenerator rnd_;
	std::list<std::shared_ptr<Enemy>> enemies_;      
	int32_t interval_ = 120;
	int32_t time_ = 0;
};