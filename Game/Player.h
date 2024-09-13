#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"
#include "MapCollider.h"

class Map;

class Player : public GameObject {
public:
	void Initialize(Map* map);
	void Update();

	void Reset();

	void SetGameClearPosPosition(float distance);

	void AddGravity(const Vector3& vector);
	void ReflectBar(const Vector3& normal);
	void KnockbackBomb(const Vector3& direction);

	const Vector3& GetSize()const { return colliderSize_; }
	const Vector3& GetVelocity() const { return velocity_; }
private:
	void Move();
	void UpdateRotate(const Vector3& vector);
	void UpdateInvincible();
	void UpdateTransform();
	void OnCollision(const CollisionInfo& collisionInfo);
	void Debug();
	ModelInstance model_;
	ModelInstance drillMode_;
	Transform modelTransform_;
	Transform drillTransform_;
	std::shared_ptr<BoxCollider> collider_;
	Vector3 colliderSize_ = { 3.0f,6.0f,3.0f };
	std::shared_ptr<MapCollider> mapCollider_;

	Vector3 initializePosition_;
	Vector3 currentVector_;
	Vector3 velocity_;
	float speed_;
	// 方向
	float directionSpeed_;
	// 重力
	float gravity_;
	// 抵抗
	float resistance_;
	// 最大速度
	float maxSpeed_;

#pragma region Invincible
	// インターバル
	float invincibleTime_;
	float invincibleInterval_ = 30.0f;
#pragma endregion

};