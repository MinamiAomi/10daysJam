
#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"

#include "Engine/Collision/Collider.h"

class Block : public GameObject {
public:
	virtual void Initialize(const Vector3& position) = 0;
	virtual void Update() = 0;
	bool GetIsAlive() { return isAlive_; }
protected:
	virtual void OnCollision(const CollisionInfo& info) = 0;
	virtual void UpdateTransform() = 0;
	std::shared_ptr<BoxCollider> collider_;
	bool isAlive_;
	ModelInstance model_;
};


class NormalBlock :public Block {
public:
	void Initialize(const Vector3& position) override;
	void Update() override;
private:
	void OnCollision(const CollisionInfo& info) override;
	void UpdateTransform()override;
};

class ExplosionBlock :public Block {
public:
	void Initialize(const Vector3& position) override;
	void Update() override;
private:
	void OnCollision(const CollisionInfo& info) override;
	void UpdateTransform()override;
};

class GravityBlock :public Block {
public:
	void Initialize(const Vector3& position) override;
	void Update() override;
private:
	void OnCollision(const CollisionInfo& info) override;
	void UpdateTransform()override;
};