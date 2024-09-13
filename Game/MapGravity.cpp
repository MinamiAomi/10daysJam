#include "MapGravity.h"

#include "Framework/AssetManager.h"

#include "MapProperty.h"
#include "Map.h"

#include "CollisionAttribute.h"
#include "Player.h"
#include "Score.h"

void MapGravity::OnInitialize() {
    transform.SetParent(&map_.transform);
    transform.translate = { MapProperty::kBlockSize * (float)column_ - (float)MapProperty::kMapColumn + MapProperty::kBlockSize * 0.5f, -MapProperty::kBlockSize * (float)row_, 0.0f };
    transform.scale = Vector3::one;
    transform.rotate = Quaternion::identity;
    transform.UpdateMatrix();

    mapModel_.SetModel(AssetManager::GetInstance()->FindModel("block"));
    mapModel_.SetWorldMatrix(transform.worldMatrix);
    gravityModel_.SetModel(AssetManager::GetInstance()->FindModel("gravity"));
    gravityModel_.SetWorldMatrix(transform.worldMatrix);

    isActive_ = true;

    SetName("MapGravity");
    collider_ = std::make_shared<SphereCollider>();
    collider_->SetGameObject(this);
    collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
    collider_->SetCollisionAttribute(CollisionAttribute::Block);
    collider_->SetCollisionMask(CollisionAttribute::Player);
    collider_->SetCenter(transform.worldMatrix.GetTranslate());
    collider_->SetRadius(MapProperty::kGravityRadius);
    collider_->SetIsActive(true);
}

void MapGravity::OnUpdate() {}

void MapGravity::OnBreak() {
    isActive_ = false;
    mapModel_.SetIsActive(false);
    gravityModel_.SetIsActive(false);
    collider_->SetIsActive(false);
    score_->AddScore(int(row_));
}

void MapGravity::OnCollision(const CollisionInfo& collisionInfo) {
    if (collisionInfo.gameObject->GetName() == "Player") {
        player_->AddGravity((transform.worldMatrix.GetTranslate() - player_->transform.worldMatrix.GetTranslate()).Normalize());
    }
}
