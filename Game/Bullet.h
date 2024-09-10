#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"

class Bullet : public GameObject {
public:
	virtual void Initialize(const Vector3& position, const Vector3& velocity) = 0;
	virtual void Update() = 0;
	virtual bool GetIsAlive() { return isAlive_; }
protected:
	virtual void UpdateTransform() = 0;
	virtual void OnCollision(const CollisionInfo& info) = 0;
	ModelInstance model_;
	std::shared_ptr<SphereCollider> collider_;
	Vector3 velocity_;
	bool isAlive_;
};

class PlayerBullet : public Bullet {
public:
	void Initialize(const Vector3& position, const Vector3& velocity) override;
	void Update() override;
private:
	void UpdateTransform() override;
	void OnCollision(const CollisionInfo& info) override;
};


class EnemyBullet : public Bullet {
public:
	void Initialize(const Vector3& position, const Vector3& velocity) override;
	void Update() override;
private:
	void UpdateTransform() override;
	void OnCollision(const CollisionInfo& info) override;
};
