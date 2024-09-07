#include "Enemy.h"

#include "Framework/AssetManager.h"
#include "CollisionAttribute.h"
#include "GameProperty.h"

void NormalEnemy::Initialize(const Vector3& position) {
	SetName("Enemy");
	model_.SetModel(AssetManager::GetInstance()->FindModel("enemy"));
	collider_ = std::make_shared<SphereCollider>();
	collider_->SetGameObject(this->shared_from_this());
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::Enemy);
	collider_->SetCollisionMask(CollisionAttribute::Player | CollisionAttribute::PlayerBullet);
	collider_->SetIsActive(true);
	isAlive_ = true;
	velocity_.y = 0.1f;
	bulletTime_ = bulletInterval_;
	transform.translate = position;
	UpdateTransform();
}

void NormalEnemy::Update() {
	transform.translate += velocity_;
	FireBullet();
	UpdateTransform();
	if (transform.translate.y >= GameProperty::GameStageSize.y) {
		isAlive_ = false;
	}
}

void NormalEnemy::FireBullet() {
	bulletTime_--;
	if (bulletTime_ <= 0) {
		bulletManager_->FireBullet(transform.translate, { 0.0f,0.2f,0.0f }, CollisionAttribute::EnemyBullet);
		bulletTime_ = bulletInterval_;
	}
}

void NormalEnemy::OnCollision(const CollisionInfo& info) {
	if (info.gameObject->GetName() == "PlayerBullet" ||
		info.gameObject->GetName() == "Player") {
		isAlive_ = false;
	}
}

void NormalEnemy::UpdateTransform() {
	transform.UpdateMatrix();
	collider_->SetRadius(2.0f);
	collider_->SetCenter(transform.worldMatrix.GetTranslate());
	model_.SetWorldMatrix(transform.worldMatrix);
}
