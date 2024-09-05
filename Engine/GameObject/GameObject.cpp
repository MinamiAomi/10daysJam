#include "GameObject.h"

#include "Graphics/ImGuiManager.h"


void GameObject::UpdateTransformHierarchy() {
    transform.UpdateMatrix();

    for (const auto& child : children_) {
        if (auto sp = child.lock()) {
            sp->UpdateTransformHierarchy();
        }
    }
}

void GameObject::SetParent(const std::shared_ptr<GameObject>& gameObject) {
    // 親がもともといる場合
    if (auto currentParent = parent_.lock()) {
        // 子供を削除
        currentParent->RemoveChild(shared_from_this());
    }
    // 新しい親がいる場合
    parent_ = gameObject;
    if (gameObject) {
        gameObject->AddChild(shared_from_this());

    }
}

void GameObject::AddChild(const std::shared_ptr<GameObject>& gameObject) {
    children_.push_back(gameObject);
}

void GameObject::RemoveChild(const std::shared_ptr<GameObject>& gameObject) {
    children_.erase(std::remove_if(children_.begin(), children_.end(), [&](const std::weak_ptr<GameObject>& child) {
        return child.lock() == gameObject;
        }),
        children_.end());
}
