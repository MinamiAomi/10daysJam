#include "MapBar.h"

#include "Framework/AssetManager.h"

#include "MapProperty.h"
#include "Map.h"
#include "Player.h"
#include "CollisionAttribute.h"

namespace {
    static const float kBarReactionFrame = 20.0f;
}

void MapBar::OnInitialize() {
    transform.SetParent(&map_.transform);
    transform.translate = { MapProperty::kBlockSize * (float)column_ - (float)MapProperty::kMapColumn + MapProperty::kBlockSize * 0.5f, -MapProperty::kBlockSize * (float)row_, 0.0f };
    transform.scale = { 5.0f, 1.0f, 1.5f };
    switch (mode_)
    {
    case MapBar::Left:
        transform.rotate = Quaternion::MakeForZAxis(Math::HalfPi * 0.5f);
        break;
    case MapBar::Right:
        transform.rotate = Quaternion::MakeForZAxis(-Math::HalfPi * 0.5f);
        break;
    default:
        break;
    }
    transform.UpdateMatrix();

    SetName("MapBar");
    collider_ = std::make_shared<BoxCollider>();
    collider_->SetGameObject(this);
    collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
    collider_->SetCollisionAttribute(CollisionAttribute::Block);
    collider_->SetCollisionMask(CollisionAttribute::Player);
    collider_->SetCenter(transform.worldMatrix.GetTranslate());
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(Vector3::Scale(transform.scale, { 2.0f, 2.0f, 2.0f }));
    collider_->SetIsActive(true);

    model_.SetModel(AssetManager::GetInstance()->FindModel("bar"));
    model_.SetWorldMatrix(transform.worldMatrix);

    isActive_ = true;
}

void MapBar::OnUpdate() {
    if (animationParameter_ > 0.0f) {
        animationParameter_ -= 1.0f;
        transform.scale.y = Math::Lerp(animationParameter_ / kBarReactionFrame, 1.0f, 1.5f);
        transform.UpdateMatrix();
        model_.SetWorldMatrix(transform.worldMatrix);
        if (animationParameter_ <= 0.0f) {
            animationParameter_ = 0.0f;
        }
    }
}

void MapBar::OnBreak() {}

void MapBar::OnCollision(const CollisionInfo& collisionInfo) {
    if (collisionInfo.gameObject->GetName() == "Player") {
        Vector3 up = transform.rotate.GetUp();
        if (Vector3::Dot(up, player_->transform.worldMatrix.GetTranslate() - transform.worldMatrix.GetTranslate()) < 0.0f) {
            up *= -1.0f;
        }
        player_->ReflectBar(up);
        map_.particles_->EmitBar(transform.worldMatrix.GetTranslate(), up, transform.scale.x * 2.0f);
        animationParameter_ = kBarReactionFrame;
    }
}