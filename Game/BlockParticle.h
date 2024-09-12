#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"
class Map;
class BlockParticle : public GameObject{
public:

	static const int kTime_ = 80;

	void Initialize();
	void Emit(const Vector3& position, const Vector3& velocity, const Math::Sphere& sphere, int index);
	void Update();

	void Reset();

	bool GetIsAlive() { return isAlive_; };
	void SetMap(const Map* map) { map_ = map; }
private:
	void UpdateTransform();

	Vector3 velocity_;

	bool isAlive_ = false;
	bool isGround_ = false;
	int time_ = 0;
	int index_ = 0;
	ModelInstance model_;
	std::shared_ptr<Material> material_;
	Math::Sphere collSphere_;
	const Map* map_ = nullptr;
};