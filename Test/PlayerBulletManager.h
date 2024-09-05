#pragma once

#include <list>
#include <memory>

#include "PlayerBullet.h"

class PlayerBulletManager {
public:
	void Initialize();
	void Update();
	void FireBullet(const Vector3& position,const Vector3& velocity);
	void RemoveBullet(std::shared_ptr<PlayerBullet> bullet);
private:
	std::list<std::shared_ptr<PlayerBullet>> playerBullets_;
};