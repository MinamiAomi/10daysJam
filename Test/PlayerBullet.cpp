#include "PlayerBullet.h"

#include "Input/input.h"
#include "Framework/AssetManager.h"

const float PlayerBullet::kLimitLine = -50.0f;

void PlayerBullet::Initialize(const Vector3& position,const Vector3& velocity) {
	model_.SetModel(AssetManager::GetInstance()->FindModel("player"));
    transform_.translate = position;
    velocity_ = velocity;
    isAlive_ = true;
    isOnce_ = false;
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
    model_.SetWorldMatrix(transform_.worldMatrix);
}
