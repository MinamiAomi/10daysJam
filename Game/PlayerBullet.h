#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"

class PlayerBullet : public GameObject {
public:
	void Initialize(const Vector3& position, const Vector3& velocity);
	void Update();
	bool GetIsAlive() { return isAlive_; }
private:
	void UpdateTransform();
	void OnCollision(const CollisionInfo& info);
	Vector3 velocity_;
	ModelInstance model_;
	bool isAlive_;
	// 一回目の地面の判定
	bool isOnce_;

	std::shared_ptr<SphereCollider> collider_;
};