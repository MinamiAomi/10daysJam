#include "BlockParticle.h"

#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"
#include "GameProperty.h"

void BlockParticle::Initialize() {
	SetName("PlayerBullet");
	model_.SetModel(AssetManager::GetInstance()->FindModel("playerBullet"));
}

void BlockParticle::Emit(const Vector3& position) {
	isAlive_ = true;
	SetIsActive(isAlive_);
	transform.translate = position;
	UpdateTransform();
}

void BlockParticle::Update() {
	SetIsActive(isAlive_);
	UpdateTransform();
}

void BlockParticle::UpdateTransform() {
	transform.UpdateMatrix();
	collider_->SetRadius(1.0f);
	collider_->SetCenter(transform.translate);
	model_.SetWorldMatrix(transform.worldMatrix);
}