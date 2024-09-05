#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"

#include "Engine/Collision/Collider.h"

class Block {
public:
	void Initialize(const Vector3& position);
	void Update();
private:
	void OnCollision(const CollisionInfo& info);
	void UpdateTransform();
	std::shared_ptr<BoxCollider> collider_;
	bool isAlive_;
	Transform transform_;
	ModelInstance model_;
};