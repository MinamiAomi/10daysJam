#include "Enemy.h"

#include "Framework/AssetManager.h"
#include "CollisionAttribute.h"

void Enemy::Initialize(const Vector3& position) {
	SetName("Enemy");
	model_.SetModel(AssetManager::GetInstance()->FindModel("enemy"));
	collider_ = std::make_shared<SphereCollider>();
	collider_->SetGameObject(this->shared_from_this());
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::Enemy);
	collider_->SetCollisionMask(CollisionAttribute::Player | CollisionAttribute::PlayerBullet | CollisionAttribute::Block);
	collider_->SetIsActive(true);
	isAlive_ = true;
	velocity_.y = 0.2f;
	transform.translate = position;
	UpdateTransform();
}

void Enemy::Update() {
	transform.translate += velocity_;
	UpdateTransform();
}

void Enemy::OnCollision(const CollisionInfo& info) {
	if (info.gameObject->GetName() == "Bullet"||
		info.gameObject->GetName() == "Player" || 
		info.gameObject->GetName() == "Block" ) {
		isAlive_ = false;
	}
}

void Enemy::UpdateTransform() {
	transform.UpdateMatrix();
	collider_->SetRadius(2.0f);
	collider_->SetCenter(transform.worldMatrix.GetTranslate());
	model_.SetWorldMatrix(transform.worldMatrix);
}
