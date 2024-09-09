#include "Bullet.h"

#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"
#include "GameProperty.h"

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity) {
	SetName("PlayerBullet");
	model_.SetModel(AssetManager::GetInstance()->FindModel("playerBullet"));
	transform.translate = position;
	velocity_ = velocity;
	isAlive_ = true;

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
	// 下に着いたら
	if (transform.translate.y <= -GameProperty::GameStageSize.y) {
		// 反転
		transform.translate.y = GameProperty::GameStageSize.y;
		// 当たり判定のマスクを変更
		collider_->SetCollisionMask(CollisionAttribute::Enemy | CollisionAttribute::Block);
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
		isAlive_ = false;
	}
	if (info.gameObject->GetName() == "Player") {
		isAlive_ = false;
	}
	if (info.gameObject->GetName() == "Enemy") {
		isAlive_ = false;
	}
}

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity) {
	SetName("EnemyBullet");
	model_.SetModel(AssetManager::GetInstance()->FindModel("enemyBullet"));
	transform.translate = position;
	velocity_ = velocity;
	isAlive_ = true;

	collider_ = std::make_shared<SphereCollider>();
	collider_->SetGameObject(this->shared_from_this());
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::EnemyBullet);
	collider_->SetCollisionMask(CollisionAttribute::Player);
	collider_->SetIsActive(true);

	transform.translate = position;

	UpdateTransform();
}

void EnemyBullet::Update() {
	if (transform.translate.x <= -GameProperty::GameStageSize.x ||
		transform.translate.x >= GameProperty::GameStageSize.x ||
		transform.translate.y <= -GameProperty::GameStageSize.y ||
		transform.translate.y >= GameProperty::GameStageSize.y) {
		isAlive_ = false;
	}
	transform.translate += velocity_;
	UpdateTransform();
}

void EnemyBullet::UpdateTransform() {
	transform.UpdateMatrix();
	collider_->SetRadius(1.0f);
	collider_->SetCenter(transform.translate);
	model_.SetWorldMatrix(transform.worldMatrix);
}

void EnemyBullet::OnCollision(const CollisionInfo& info) {
	if (info.gameObject->GetName() == "Player") {
		isAlive_ = false;
	}
}
