#pragma once

#include <list>
#include <memory>

#include "CollisionAttribute.h"
#include "Bullet.h"

class BulletManager {
public:
	void Initialize();
	void Update();
	void FireBullet(const Vector3& position, const Vector3& velocity,uint32_t attribute);
	void RemoveBullet(std::shared_ptr<Bullet> bullet);

	void Reset();
private:
	std::list<std::shared_ptr<Bullet>> bullets_;
};