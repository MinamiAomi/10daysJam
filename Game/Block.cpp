#include "Block.h"

#include "Framework/AssetManager.h"
#include "CollisionAttribute.h"

void Block::Initialize(const Vector3& position) {
	SetName("Block");
	model_.SetModel(AssetManager::GetInstance()->FindModel("block"));
	collider_ = std::make_shared<BoxCollider>();
	collider_->SetGameObject(this->shared_from_this());
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::Block);
	collider_->SetCollisionMask(CollisionAttribute::Player | CollisionAttribute::PlayerBullet);
	collider_->SetIsActive(true);

	transform.translate = position;
	isAlive_ = true;
	UpdateTransform();
}

void Block::Update() {

	UpdateTransform();
}

void Block::OnCollision(const CollisionInfo& info) {
	if (info.gameObject->GetName() == "PlayerBullet") {
		isAlive_ = false;
	}
}

void Block::UpdateTransform() {
	transform.UpdateMatrix();
	collider_->SetSize({ 2.0f,2.0f,2.0f });
	collider_->SetCenter(transform.worldMatrix.GetTranslate());
	collider_->SetOrientation(transform.worldMatrix.GetRotate());
	model_.SetWorldMatrix(transform.worldMatrix);
}