#pragma once
#include "MapTileBase.h"

#include "GameObject/GameObject.h"
#include "Math/Transform.h"
#include "Graphics/Model.h"

#include "Game/BlockParticles.h"

class MapBlock :
    public MapTileBase, GameObject {
public:
    using MapTileBase::MapTileBase;

    void OnInitialize() override;
    void OnUpdate() override;
    void OnBreak() override;

    void SetBlockParticles(std::shared_ptr<BlockParticles> blockParticles) { blockParticles_ = blockParticles; }

private:
    ModelInstance model_;
    std::shared_ptr<BlockParticles> blockParticles_;

};