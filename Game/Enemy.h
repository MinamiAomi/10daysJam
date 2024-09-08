
#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include <stdint.h>

#include "Math/Transform.h"
#include "Graphics/Model.h"

#include "Engine/Collision/Collider.h"
#include "BulletManager.h"

class Enemy :public GameObject {
public:
	virtual void Initialize(const Vector3& position) = 0;
	virtual void Update() = 0;

	bool GetIsAlive() { return isAlive_; }

	void SetBulletManager(std::shared_ptr<BulletManager> bulletManager) { bulletManager_ = bulletManager; }
protected:
	virtual void OnCollision(const CollisionInfo& info) = 0;
	virtual void UpdateTransform() = 0;
	std::shared_ptr<SphereCollider> collider_;
	std::shared_ptr<BulletManager> bulletManager_;
	ModelInstance model_;
	Vector3 velocity_;
	bool isAlive_;
};

class NormalEnemy : public Enemy {
public:
	void Initialize(const Vector3& position)override;
	void Update()override;
private:
	void FireBullet();
	void OnCollision(const CollisionInfo& info)override;
	void UpdateTransform() override;
	int32_t bulletInterval_ = 60;
	int32_t bulletTime_;
};