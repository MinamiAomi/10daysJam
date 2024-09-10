#pragma once

#include <memory>

#include "Math/MathUtils.h"

class Map;

// XYの2Dの長方形コライダー
// shared_ptrのみ
class MapCollider :
    public std::enable_shared_from_this<MapCollider> {
    friend class Map;
public:
    enum Mode {
        None,
        Break,

    };

    ~MapCollider();

    void SetPosition(const Vector2& position) { position_ = position; }
    void SetSize(const Vector2& size) { size_ = size; }
    void SetRotate(float rotate) { rotate_ = rotate; }
    void SetMode(Mode mode) { mode_ = mode; }
    void SetIsActive(bool isActive) { isActive_ = isActive; }

private:
    Map* map_ = nullptr;
    Vector2 position_;
    Vector2 size_;
    float rotate_ = 0.0f;
    Mode mode_ = None;
    bool isActive_ = true;

};
