#pragma once

#include "MathUtils.h"

class Camera {
public:
    enum ProjectionType {
        Perspective,
        Orthographic
    };

    union Projection {
        struct Perspective {
            float fovY;
            float aspectRaito;
        } perspective;
        struct Orthographic {
            float width;
            float height;
        } orthographic;
    };

    Camera(ProjectionType projectionType = Perspective);

    void UpdateMatrices();

    void SetPosition(const Vector3& position) { position_ = position, needUpdateing_ = true; }
    void SetRotate(const Quaternion& rotate) { rotate_ = rotate, needUpdateing_ = true; }

    void SetPerspective(float fovY, float aspectRaito, float nearClip, float farClip);
    void SetOrthographic(float width, float height, float nearClip, float farClip);

    float GetNearClip() const { return nearClip_; }
    float GetFarClip() const { return farClip_; }
    const Vector3& GetPosition() const { return position_; }
    const Quaternion& GetRotate() const { return rotate_; }
    const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
    const Projection& GetProjection() const { return projection_; }
    const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
    const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
    const Matrix4x4& GetBillbordMatrix() const { return billbordMatrix_; }

    Vector3 GetForward() const { return rotate_ * Vector3::forward; }
    Vector3 GetRight() const { return rotate_ * Vector3::right; }
    Vector3 GetUp() const { return rotate_ * Vector3::up; }

private:
    Vector3 position_;
    Quaternion rotate_;
    
    ProjectionType projectionType_;
    Projection projection_;
    float nearClip_;
    float farClip_;

    Matrix4x4 viewMatrix_;
    Matrix4x4 projectionMatrix_;
    Matrix4x4 viewProjectionMatrix_;
    Matrix4x4 billbordMatrix_;

    bool needUpdateing_;
};