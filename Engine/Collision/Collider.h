#pragma once
#include <functional>
#include <memory>
#include <string>

#include "Math/MathUtils.h"
#include "Math/Geometry.h"

class GameObject;
class Collider;
class SphereCollider;
class BoxCollider;

struct CollisionInfo {
    std::shared_ptr<GameObject> gameObject;
    Vector3 normal;
    float depth;
};

struct RayCastInfo {
    std::shared_ptr<GameObject> gameObject;
    float nearest;
};
class Collider {
    friend class CollisionManager;
public:
    using Callback = std::function<void(const CollisionInfo&)>;

    Collider();
    virtual ~Collider();

    virtual bool IsCollision(Collider* collider, CollisionInfo& collisionInfo) = 0;
    virtual bool IsCollision(SphereCollider* collider, CollisionInfo& collisionInfo) = 0;
    virtual bool IsCollision(BoxCollider* collider, CollisionInfo& collisionInfo) = 0;
    virtual bool RayCast(const Vector3& origin, const Vector3& diff, uint32_t mask, RayCastInfo& nearest) = 0;

    void SetGameObject(const std::shared_ptr<GameObject>& gameObject) { gameObject_ = gameObject; }
    void SetCallback(Callback callback) { callback_ = callback; }
    void SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
    void SetCollisionMask(uint32_t mask) { collisionMask_ = mask; }
    void SetIsActive(bool isActive) { isActive_ = isActive; }

    void OnCollision(const CollisionInfo& collisionInfo);

    std::shared_ptr<GameObject> GetGameObject() { return gameObject_.lock(); }
    bool IsActive() const { return isActive_; }

protected:
    bool CanCollision(Collider* other) const;
    bool CanCollision(uint32_t mask) const;

    std::weak_ptr<GameObject> gameObject_;

    Callback callback_;
    uint32_t collisionAttribute_ = 0xFFFFFFFF;
    uint32_t collisionMask_ = 0xFFFFFFFF;
    bool isActive_;
};

class SphereCollider :
    public Collider {
    friend class BoxCollider;
public:
    bool IsCollision(Collider* collider, CollisionInfo& collisionInfo) override;
    bool IsCollision(SphereCollider* collider, CollisionInfo& collisionInfo) override;
    bool IsCollision(BoxCollider* collider, CollisionInfo& collisionInfo) override;
    bool RayCast(const Vector3& origin, const Vector3& diff, uint32_t mask, RayCastInfo& nearest) override;


    void SetCenter(const Vector3& center) { sphere_.center = center; }
    void SetRadius(float radius) { sphere_.radius = radius; }

private:
    Math::Sphere sphere_;
};

class BoxCollider :
    public Collider {
    friend class SphereCollider;
public:
    bool IsCollision(Collider* other, CollisionInfo& collisionInfo) override;
    bool IsCollision(SphereCollider* other, CollisionInfo& collisionInfo) override;
    bool IsCollision(BoxCollider* other, CollisionInfo& collisionInfo) override;
    bool RayCast(const Vector3& origin, const Vector3& diff, uint32_t mask, RayCastInfo& nearest) override;

    void SetCenter(const Vector3& center) { obb_.center = center; }
    void SetOrientation(const Quaternion& orientation) {
        obb_.orientations[0] = orientation.GetRight();
        obb_.orientations[1] = orientation.GetUp();
        obb_.orientations[2] = orientation.GetForward();
    }
    void SetSize(const Vector3& size) { obb_.size = size; }

private:
    Math::OBB obb_;
};