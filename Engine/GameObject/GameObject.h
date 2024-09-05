#pragma once

#include <typeindex>
#include <typeinfo>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Math/Transform.h"

class GameObject :
    public std::enable_shared_from_this<GameObject> {
    friend class GameObjectManager;
public:
    virtual ~GameObject() {}

    void UpdateTransformHierarchy();

    void SetParent(const std::shared_ptr<GameObject>& gameObject);
    bool HasParent() const { return !parent_.expired(); }
    const std::weak_ptr<GameObject>& GetParent() const { return parent_; }
    const std::vector<std::weak_ptr<GameObject>>& GetChildren() const { return children_; }

    void SetName(const std::string& name) { name_ = name; }
    const std::string& GetName() const { return name_; }

    void SetIsActive(bool isActive) { isActive_ = isActive; }
    bool IsActive() const { return isActive_; }

    Transform transform;

private:
    void AddChild(const std::shared_ptr<GameObject>& gameObject);
    void RemoveChild(const std::shared_ptr<GameObject>& gameObject);

    // 親
    std::weak_ptr<GameObject> parent_;
    // 子
    std::vector<std::weak_ptr<GameObject>> children_;
    // 名前
    std::string name_;
    bool isActive_;
};