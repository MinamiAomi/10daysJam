#include "MapBlock.h"

#include "Framework/AssetManager.h"

#include "MapProperty.h"
#include "Map.h"
#include "Score.h"

void MapBlock::OnInitialize() {

    transform.SetParent(&map_.transform);
    transform.translate = { MapProperty::kBlockSize * (float)column_ - (float)MapProperty::kMapColumn + MapProperty::kBlockSize * 0.5f, -MapProperty::kBlockSize * (float)row_, 0.0f };
    transform.scale = Vector3::one;
    transform.rotate = Quaternion::identity;
    transform.UpdateMatrix();

    model_.SetModel(AssetManager::GetInstance()->FindModel("block"));
    model_.SetWorldMatrix(transform.worldMatrix);

    isActive_ = true;
}

void MapBlock::OnUpdate() {

}

void MapBlock::OnBreak() {
    isActive_ = false;
    model_.SetIsActive(false);
    blockParticles_->Emit(transform.translate);
    score_->AddScore(int(row_));
}

void MapBlock::OnSwitchingCulling() {
    model_.SetIsActive(!model_.IsActive());
}
