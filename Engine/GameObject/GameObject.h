#pragma once

#include <string>

#include "Math/Transform.h"

class GameObject {
public:
    virtual ~GameObject() {}

    void SetName(const std::string& name) { name_ = name; }
    const std::string& GetName() const { return name_; }


    Transform transform;

private:

    // 名前
    std::string name_;
};