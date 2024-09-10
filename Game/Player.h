#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"
#include "BulletManager.h"
#include "MapCollider.h"
#include "Map.h"

class Player : public GameObject {
public:
	void Initialize(Map* map);
	void Update();

	void Reset();
	void SetBulletManager(std::shared_ptr<BulletManager> bulletManager) { bulletManager_ = bulletManager; }
private:
	void Move();
	void FireBullet();
	void UpdateInvincible();
	void UpdateTransform();
	void OnCollision(const CollisionInfo& collisionInfo);
	ModelInstance model_;
	std::shared_ptr<BoxCollider> collider_;
	std::shared_ptr<MapCollider> mapCollider_;

	float initializePosition_ = 4.0f;
	float velocity_ = 0.2f;
	float angle_ = 0.0f;
	Vector3 currentVector_;

#pragma region Invincible
	// インターバル
	float invincibleTime_;
	float invincibleInterval_ = 30.0f;
#pragma endregion


#pragma region Bullet
	std::shared_ptr<BulletManager> bulletManager_;
	// インターバル
	float fireInterval_ = 30.0f;
	float fireTime_ = 0.0f;
#pragma endregion 
};