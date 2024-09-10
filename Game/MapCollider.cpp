#include "MapCollider.h"

#include "Map.h"

MapCollider::~MapCollider() {
    if (map_) {
        map_->RemoveCollider(shared_from_this());
    }
}
