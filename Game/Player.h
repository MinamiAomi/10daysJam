#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"
#include "BulletManager.h"

class Player : public GameObject {
public:
	void Initialize();
	void Update();

	void Reset();
	void SetBulletManager(std::shared_ptr<BulletManager> bulletManager) { bulletManager_ = bulletManager; }
private:
	void FireBullet();
	void UpdateTransform();
	void OnCollision(const CollisionInfo& collisionInfo);
	ModelInstance model_;
	std::shared_ptr<BoxCollider> collider_;

	bool canJump_;
	float initializePosition_ = 4.0f;
	Vector3 velocity_;

#pragma region Bullet
	std::shared_ptr<BulletManager> bulletManager_;
	// インターバル
	float fireInterval_ = 30.0f;
	float fireTime_ = 0.0f;
#pragma endregion 
};