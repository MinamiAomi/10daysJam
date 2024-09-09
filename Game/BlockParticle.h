#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"
class BlockParticle : public GameObject{
public:
	void Initialize();
	void Emit(const Vector3& position);
	void Update();

	bool GetIsAlive() { return isAlive_; };
private:
	void UpdateTransform();


	bool isAlive_ = false;
	ModelInstance model_;
	std::shared_ptr<SphereCollider> collider_;
};