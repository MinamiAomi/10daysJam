#include "MapBomb.h"

void MapBomb::OnInitialize() {}

void MapBomb::OnUpdate() {}

void MapBomb::OnBreak() {}

void MapBomb::OnSwitchingCulling() {
    model_.SetIsActive(!model_.IsActive());
}

