#pragma once

#include "../Core/RootSignature.h"
#include "../Core/PipelineState.h"
#include "../Core/CommandContext.h"

#include "Math/MathUtils.h"
#include "Math/Camera.h"

#include "Voronoi.h"

class SkyRenderer {
public:
   static float y_;
   static const int transitionHeight = 50;

    void Initialize(DXGI_FORMAT rtvFormat);
    void Render(CommandContext& commandContext, const Camera& camera, Matrix4x4 worldMatrix);
    void DebugParam();

    static void SetZero() { y_ = 0.0f; }

private:
    RootSignature rootSignature_;
    PipelineState pipelineState_;

    Voronoi voronoi_;
    Vector3 color_ = {1.0f,1.0f,1.0f};
    Vector3 toColor_ = { 0.0f,0.0f,0.0f };

    Vector3 topColor_;
    Vector3 bottomColor_;

    float speed_ = 0.01f;

    int switchNum_ = 0;

};