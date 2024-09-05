#include "Block.h"

#include "Framework/AssetManager.h"
#include "CollisionAttribute.h"

void Block::Initialize(const Vector3& position) {
	model_.SetModel(AssetManager::GetInstance()->FindModel("block"));
	collider_ = std::make_shared<BoxCollider>();
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::Block);
	collider_->SetCollisionMask(CollisionAttribute::Player | CollisionAttribute::PlayerBullet);
	collider_->SetSize({ 1.0f,1.0f,1.0f });
	collider_->SetCenter(position);
	collider_->SetOrientation(Quaternion::identity);
	transform_.translate = position;
	UpdateTransform();
}

void Block::Update() {
	UpdateTransform();
}

void Block::OnCollision(const CollisionInfo& info) {
	info;
}

void Block::UpdateTransform() {
	transform_.UpdateMatrix();
	model_.SetWorldMatrix(transform_.worldMatrix);
}
