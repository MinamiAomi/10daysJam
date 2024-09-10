#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"
#include "BulletManager.h"
#include "MapCollider.h"

class Map;

class Player : public GameObject {
public:
	void Initialize(Map* map);
	void Update();

	void Reset();
	void SetBulletManager(std::shared_ptr<BulletManager> bulletManager) { bulletManager_ = bulletManager; }

	void AddGravity(const Vector3& vector);
private:
	void Move();
	void UpdateRotate(const Vector3& vector);
	void FireBullet();
	void UpdateInvincible();
	void UpdateTransform();
	void OnCollision(const CollisionInfo& collisionInfo);
	void Debug();
	ModelInstance model_;
	std::shared_ptr<BoxCollider> collider_;
	std::shared_ptr<MapCollider> mapCollider_;

	Vector3 initializePosition_;
	Vector3 currentVector_;
	Vector3 velocity_;
	float speed_;
	// 方向
	float directionSpeed_;
	// 重力
	float gravity_;

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