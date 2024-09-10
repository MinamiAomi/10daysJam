#include "Block.h"

#include "Framework/AssetManager.h"
#include "CollisionAttribute.h"

void NormalBlock::Initialize(const Vector3& position) {
	SetName("NormalBlock");
	model_.SetModel(AssetManager::GetInstance()->FindModel("block"));
	collider_ = std::make_shared<BoxCollider>();
	collider_->SetGameObject(this);
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::Block);
	collider_->SetCollisionMask(CollisionAttribute::Player | CollisionAttribute::PlayerBullet);
	collider_->SetIsActive(true);

	transform.translate = position;
	isAlive_ = true;
	UpdateTransform();
}

void NormalBlock::Update() {
	UpdateTransform();
}

void NormalBlock::OnCollision(const CollisionInfo& info) {
	if (info.gameObject->GetName() == "PlayerBullet") {
		isAlive_ = false;
	}
	if (info.gameObject->GetName() == "Player") {
		isAlive_ = false;
	}
}

void NormalBlock::UpdateTransform() {
	transform.UpdateMatrix();
	collider_->SetSize({ 2.0f,2.0f,2.0f });
	collider_->SetCenter(transform.worldMatrix.GetTranslate());
	collider_->SetOrientation(transform.worldMatrix.GetRotate());
	model_.SetWorldMatrix(transform.worldMatrix);
}

void ExplosionBlock::Initialize(const Vector3& position) {
	SetName("ExplosionBlock");
	model_.SetModel(AssetManager::GetInstance()->FindModel("block"));
	collider_ = std::make_shared<BoxCollider>();
	collider_->SetGameObject(this);
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::Block);
	collider_->SetCollisionMask(CollisionAttribute::Player | CollisionAttribute::PlayerBullet);
	collider_->SetIsActive(true);

	transform.translate = position;
	isAlive_ = true;
	UpdateTransform();
}

void ExplosionBlock::Update() {
	UpdateTransform();
}

void ExplosionBlock::OnCollision(const CollisionInfo& info) {
	if (info.gameObject->GetName() == "PlayerBullet") {
		isAlive_ = false;
	}
	if (info.gameObject->GetName() == "Player") {
		isAlive_ = false;
	}
}

void ExplosionBlock::UpdateTransform() {
	transform.UpdateMatrix();
	collider_->SetSize({ 2.0f,2.0f,2.0f });
	collider_->SetCenter(transform.worldMatrix.GetTranslate());
	collider_->SetOrientation(transform.worldMatrix.GetRotate());
	model_.SetWorldMatrix(transform.worldMatrix);
}

void GravityBlock::Initialize(const Vector3& position) {
	position;
}

void GravityBlock::Update() {}

void GravityBlock::OnCollision(const CollisionInfo& info) {
	info;
}

void GravityBlock::UpdateTransform() {}
