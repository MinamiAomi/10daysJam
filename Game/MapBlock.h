#pragma once
#include "MapTileBase.h"

#include "GameObject/GameObject.h"
#include "Math/Transform.h"
#include "Graphics/Model.h"

class MapBlock :
    public MapTileBase, GameObject {
public:
    using MapTileBase::MapTileBase;

    void OnInitialize() override;
    void OnUpdate() override;
    void OnBreak() override;

private:
    ModelInstance model_;

};