#pragma once
#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

class PlayerBullet {
public:
    void Initialize(const Vector3& position, const Vector3& velocity);
    void Update();
    bool GetIsAlive() { return isAlive_; }
private:
    void UpdateTransform();
    static const float kLimitLine;
    Vector3 velocity_;
    Transform transform_;
    ModelInstance model_;
    bool isAlive_;
    // 一回目の地面の判定
    bool isOnce_;
    //ModelInstance hammer_;
    //std::shared_ptr<Skeleton> skeleton_;
    //std::shared_ptr<Animation> walk_;
    //float animationTime_;
};