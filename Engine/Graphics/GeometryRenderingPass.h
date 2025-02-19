#pragma once
#include <memory>

#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Core/RootSignature.h"
#include "Core/PipelineState.h"
#include "Math/Camera.h"
#include "ModelSorter.h"

class CommandContext;

class GeometryRenderingPass {
public:
    struct RootIndex {
        enum Parameters {
            Scene,
            InstanceOffset,
            Instances,
            BindlessTexture,

            NumRootParameters
        };
    };
    
    void Initialize(uint32_t width, uint32_t height);
    void Render(CommandContext& commandContext, const Camera& camera, const ModelSorter& modelSorter);

    ColorBuffer& GetAlbedo() { return albedo_; }
    ColorBuffer& GetMetallicRoughness() { return metallicRoughness_; }
    ColorBuffer& GetNormal() { return normal_; }
    DepthBuffer& GetDepth() { return depth_; }

private:
    ColorBuffer albedo_;
    ColorBuffer metallicRoughness_;
    ColorBuffer normal_;
    DepthBuffer depth_;

    RootSignature rootSignature_;
    PipelineState pipelineState_;
    StructuredBuffer instancingBuffer_;
};