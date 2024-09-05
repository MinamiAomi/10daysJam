
#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"

#include "Engine/Collision/Collider.h"

class Enemy :public GameObject {
public:
	void Initialize(const Vector3& position);
	void Update();
	
	bool GetIsAlive() { return isAlive_; }
private:
	void OnCollision(const CollisionInfo& info);
	void UpdateTransform();
	std::shared_ptr<SphereCollider> collider_;
	bool isAlive_;
	ModelInstance model_;
	Vector3 velocity_;
};
