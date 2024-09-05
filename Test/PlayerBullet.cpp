#include "PlayerBullet.h"

#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"


const float PlayerBullet::kLimitLine = -30.0f;

void PlayerBullet::Initialize(const Vector3& position,const Vector3& velocity) {
	model_.SetModel(AssetManager::GetInstance()->FindModel("playerBullet"));
    transform_.translate = position;
    velocity_ = velocity;
    isAlive_ = true;
    isOnce_ = false;

    collider_ = std::make_shared<SphereCollider>();
    collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
    collider_->SetCollisionAttribute(CollisionAttribute::PlayerBullet);
    collider_->SetCollisionMask(CollisionAttribute::Player | CollisionAttribute::Block);
    collider_->SetRadius(1.0f);
    collider_->SetCenter(position);
    transform_.translate = position;

    UpdateTransform();
}

void PlayerBullet::Update() {
    if (transform_.translate.y<=0.0f&&
        isOnce_) {
        isAlive_ = false;
    }
    // 下に着いたら
    if (transform_.translate.y <= kLimitLine) {
        // 反転
        transform_.translate.y = kLimitLine * -1.0f;
        isOnce_ = true;
    }
    transform_.translate += velocity_;
    UpdateTransform();
}

void PlayerBullet::UpdateTransform() {
    transform_.UpdateMatrix();
    collider_->SetCenter(transform_.translate);
    model_.SetWorldMatrix(transform_.worldMatrix);
}

void PlayerBullet::OnCollision(const CollisionInfo& info) {
    if (info.gameObject && 
        isOnce_
        ) {

    }
}
