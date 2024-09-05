#include "PlayerBullet.h"

#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"


const float PlayerBullet::kLimitLine = -30.0f;

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity) {
	SetName("Bullet");
	model_.SetModel(AssetManager::GetInstance()->FindModel("playerBullet"));
	transform.translate = position;
	velocity_ = velocity;
	isAlive_ = true;
	isOnce_ = false;

	collider_ = std::make_shared<SphereCollider>();
	collider_->SetGameObject(this->shared_from_this());
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::PlayerBullet);
	collider_->SetCollisionMask(CollisionAttribute::Enemy);
	collider_->SetIsActive(true);

	transform.translate = position;

	UpdateTransform();
}

void PlayerBullet::Update() {
	if (transform.translate.y <= 0.0f &&
		isOnce_) {
		isAlive_ = false;
	}
	// 下に着いたら
	if (transform.translate.y <= kLimitLine) {
		// 反転
		transform.translate.y = kLimitLine * -1.0f;
		collider_->SetCollisionMask(CollisionAttribute::Enemy | CollisionAttribute::Block | CollisionAttribute::Player);
		isOnce_ = true;
	}
	transform.translate += velocity_;
	UpdateTransform();
}

void PlayerBullet::UpdateTransform() {
	transform.UpdateMatrix();
	collider_->SetRadius(1.0f);
	collider_->SetCenter(transform.translate);
	model_.SetWorldMatrix(transform.worldMatrix);
}

void PlayerBullet::OnCollision(const CollisionInfo& info) {
	if (info.gameObject->GetName() == "Block") {
		if (isOnce_) {
			isAlive_ = false;
		}
	}
	if (info.gameObject->GetName() == "Player") {
			isAlive_ = false;
	}
	if (info.gameObject->GetName() == "Enemy") {
		isAlive_ = false;
	}
}