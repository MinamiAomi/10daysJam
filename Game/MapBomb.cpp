#include "MapBomb.h"


#include "Framework/AssetManager.h"

#include "MapProperty.h"
#include "Map.h"

#include "CollisionAttribute.h"
#include "Player.h"

namespace {
    static const uint16_t kExplosionRange = 7;
}

void MapBomb::OnInitialize() {
    transform.SetParent(&map_.transform);
    transform.translate = { MapProperty::kBlockSize * (float)column_ - (float)MapProperty::kMapColumn + MapProperty::kBlockSize * 0.5f, -MapProperty::kBlockSize * (float)row_, 0.0f };
    transform.scale = Vector3(MapProperty::kBombRadius);
    transform.rotate = Quaternion::identity;
    transform.UpdateMatrix();

    model_.SetModel(AssetManager::GetInstance()->FindModel("bom"));
    model_.SetWorldMatrix(transform.worldMatrix);

    SetName("MapBomb");
    collider_ = std::make_shared<SphereCollider>();
    collider_->SetGameObject(this);
    collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });
    collider_->SetCollisionAttribute(CollisionAttribute::Block);
    collider_->SetCollisionMask(CollisionAttribute::Player);
    collider_->SetCenter(transform.worldMatrix.GetTranslate());
    collider_->SetRadius(MapProperty::kBombRadius);
    collider_->SetIsActive(true);

    isActive_ = true;
}

void MapBomb::OnUpdate() {}

void MapBomb::OnBreak() {}

void MapBomb::OnCollision(const CollisionInfo& collisionInfo) {
    if (collisionInfo.gameObject->GetName() == "Player") {
        // 自分を消す
        map_.RemoveTile(row_, column_);

        int32_t topRow = (int32_t)row_ - (int32_t)kExplosionRange;
        int32_t bottomRow = (int32_t)row_ + (int32_t)kExplosionRange;
        int32_t leftColumn = (int32_t)column_ - (int32_t)kExplosionRange;
        int32_t rightColumn = (int32_t)column_ + (int32_t)kExplosionRange;
        map_.MapRangeTile(topRow, leftColumn);
        map_.MapRangeTile(bottomRow, rightColumn);

        Vector2  bombPosition = { (float)column_, (float)row_ };
        for (int32_t row = topRow; row < bottomRow; ++row) {
            for (int32_t column = leftColumn; column < rightColumn; ++column) {
                Vector2 tilePosition = { (float)column, (float)row };
                if ((bombPosition - tilePosition).Length() < (float)kExplosionRange) {
                    map_.RemoveTile((uint16_t)row, (uint16_t)column);
                }
            }
        }

        map_.particles_->EmitBom(transform.worldMatrix.GetTranslate());
        player_->KnockbackBomb((player_->transform.worldMatrix.GetTranslate() - transform.worldMatrix.GetTranslate()).Normalized());
    }
}

