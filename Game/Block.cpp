#include "Block.h"

#include "Framework/AssetManager.h"
#include "CollisionAttribute.h"
#include "Game/GameProperty.h"

void Block::Initialize(const Vector3& position) {
	SetName("Block");
	model_.SetModel(AssetManager::GetInstance()->FindModel("block"));
	collider_ = std::make_shared<BoxCollider>();
	collider_->SetGameObject(this->shared_from_this());
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
	collider_->SetCollisionAttribute(CollisionAttribute::Block);
	collider_->SetCollisionMask(CollisionAttribute::Player | CollisionAttribute::PlayerBullet | CollisionAttribute::Enemy);
	collider_->SetIsActive(true);

	transform.translate = position;
	isAlive_ = true;
	UpdateTransform();
}

void Block::Update() {

	transform.translate += velocity_;

	// 落下リミット
	if (transform.translate.y <= -GameProperty::GameStageSize.y) {
		transform.translate.y = GameProperty::GameStageSize.y;
	}
	// 上から降ってくる
	UpdateTransform();
}

void Block::OnCollision(const CollisionInfo& info) {
	if (info.gameObject->GetName() == "Bullet") {
		velocity_ = { 0.0f,-0.3f,0.0f };
	}
	if (info.gameObject->GetName() == "Enemy") {
		isAlive_ = false;
	}
	if (info.gameObject->GetName() == "Block") {
		// ブロックに上もしくは下に触れたら
		if (Vector3::Dot(info.normal, Vector3::up) >= 0.8f ||
			Vector3::Dot(info.normal, Vector3::down) >= 0.8f) {
			velocity_ = { 0.0f,0.0f,0.0f };
		}
	}
}

void Block::UpdateTransform() {
	transform.UpdateMatrix();
	collider_->SetSize({ 2.0f,2.0f,2.0f });
	collider_->SetCenter(transform.worldMatrix.GetTranslate());
	collider_->SetOrientation(transform.worldMatrix.GetRotate());
	model_.SetWorldMatrix(transform.worldMatrix);
}