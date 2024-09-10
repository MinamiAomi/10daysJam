#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"
class BlockParticle : public GameObject{
public:

	static const int kTime_ = 180;

	void Initialize();
	void Emit(const Vector3& position, const Math::Sphere& sphere, int index);
	void Update();

	bool GetIsAlive() { return isAlive_; };
private:
	void UpdateTransform();

	Vector3 velocity_;

	bool isAlive_ = false;
	bool isGround_ = false;
	int time_ = 0;
	int index_ = 0;
	ModelInstance model_;
	Math::Sphere collSphere_;
};