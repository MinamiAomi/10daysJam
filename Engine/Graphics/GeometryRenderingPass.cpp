#include "GeometryRenderingPass.h"

#include <span>

#include "Core/Helper.h"
#include "Core/Graphics.h"
#include "Core/ShaderManager.h"
#include "Core/CommandContext.h"
#include "Math/Camera.h"
#include "Core/SamplerManager.h"
#include "Model.h"
#include "DefaultTextures.h"
#include "RenderManager.h"

namespace {
    const wchar_t kVertexShader[] = L"Standard/GeometryPassInstancingVS.hlsl";
    const wchar_t kPixelShader[] = L"Standard/GeometryPassInstancingPS.hlsl";
}

void GeometryRenderingPass::Initialize(uint32_t width, uint32_t height) {

    float albedoClearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    albedo_.SetClearColor(albedoClearColor);
    albedo_.Create(L"GeometryRenderingPass Albedo", width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
    float metallicRoughnessClearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    metallicRoughness_.SetClearColor(metallicRoughnessClearColor);
    metallicRoughness_.Create(L"GeometryRenderingPass MetallicRoughness", width, height, DXGI_FORMAT_R16G16_UNORM);
    float normalClearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    normal_.SetClearColor(normalClearColor);
    normal_.Create(L"GeometryRenderingPass Normal", width, height, DXGI_FORMAT_R10G10B10A2_UNORM);
    depth_.Create(L"GeometryRenderingPass Depth", width, height, DXGI_FORMAT_D32_FLOAT);

    RootSignatureDescHelper rootSignatureDesc;
    // Scene
    rootSignatureDesc.AddConstantBufferView(0);
    // InstanceOffset
    rootSignatureDesc.AddConstants(1, 1);
    // Instances
    rootSignatureDesc.AddDescriptorTable().AddSRVDescriptors(1, 0);
    //
    rootSignatureDesc.AddDescriptorTable().AddSRVDescriptors(BINDLESS_RESOURCE_MAX, 0, 1);
    rootSignatureDesc.AddStaticSampler(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
    rootSignatureDesc.AddFlag(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    rootSignature_.Create(L"GeometryRenderingPass RootSignature", rootSignatureDesc);

    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};
        pipelineStateDesc.pRootSignature = rootSignature_;

        D3D12_INPUT_ELEMENT_DESC inputElements[] = {
           { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
           { "NORMAL", 0, DXGI_FORMAT_R10G10B10A2_UNORM, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
           { "TANGENT", 0, DXGI_FORMAT_R10G10B10A2_UNORM, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
           { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };
        D3D12_INPUT_LAYOUT_DESC inputLayout{};
        inputLayout.NumElements = _countof(inputElements);
        inputLayout.pInputElementDescs = inputElements;
        pipelineStateDesc.InputLayout = inputLayout;

        auto shaderManager = ShaderManager::GetInstance();
        auto vs = shaderManager->Compile(kVertexShader, ShaderManager::kVertex);
        auto ps = shaderManager->Compile(kPixelShader, ShaderManager::kPixel);
        pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
        pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());
        pipelineStateDesc.BlendState = Helper::BlendDisable;
        pipelineStateDesc.DepthStencilState = Helper::DepthStateReadWrite;
        pipelineStateDesc.RasterizerState = Helper::RasterizerDefault;
        pipelineStateDesc.NumRenderTargets = 3;
        pipelineStateDesc.RTVFormats[0] = albedo_.GetRTVFormat();
        pipelineStateDesc.RTVFormats[1] = metallicRoughness_.GetRTVFormat();
        pipelineStateDesc.RTVFormats[2] = normal_.GetRTVFormat();
        pipelineStateDesc.DSVFormat = depth_.GetFormat();
        pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        pipelineStateDesc.SampleDesc.Count = 1;

        pipelineState_.Create(L"GeometryRenderingPass PipelineState", pipelineStateDesc);
    }

}

void GeometryRenderingPass::Render(CommandContext& commandContext, const Camera& camera, const ModelSorter& modelSorter) {

    struct SceneData {
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
        Vector3 cameraPosition;
    };

    struct InstanceData {
        Matrix4x4 worldMatrix;
        Vector3 albedo;
        float alpha;
        float metallic;
        float roughness;
        uint32_t albedoMapIndex;
        uint32_t pad;
    };


    uint32_t defaultWhiteTextureIndex = DefaultTexture::White.GetSRV().GetIndex();

    auto SetMaterial = [defaultWhiteTextureIndex](InstanceData& instance, const Material& material) {
        instance.albedo = material.albedo;
        instance.metallic = material.metallic;
        instance.roughness = material.roughness;
        uint32_t albedoMapIndex = defaultWhiteTextureIndex;
        if (material.albedoMap) {
            albedoMapIndex = material.albedoMap->GetSRV().GetIndex();
        }
        instance.albedoMapIndex = albedoMapIndex;
    };

    commandContext.TransitionResource(albedo_, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandContext.TransitionResource(metallicRoughness_, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandContext.TransitionResource(normal_, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandContext.TransitionResource(depth_, D3D12_RESOURCE_STATE_DEPTH_WRITE);

    commandContext.ClearColor(albedo_);
    commandContext.ClearColor(metallicRoughness_);
    commandContext.ClearColor(normal_);
    commandContext.ClearDepth(depth_);

    auto& modelMap = modelSorter.GetModelInstanceMap();
    auto& instanceList = modelSorter.GetDrawModels();
    if (instanceList.empty()) {
        return;
    }
    // Uploadバッファを割り当てる
    size_t allocateBufferSize = sizeof(InstanceData) * instanceList.size();
    auto alloc = commandContext.AllocateDynamicBuffer(LinearAllocatorType::Upload, allocateBufferSize);
    std::span<InstanceData> instancesData = { static_cast<InstanceData*>(alloc.cpu), instanceList.size() };
    uint32_t drawCount = 0;
    // Uploadバッファを埋める
    for (auto& [model, instances] : modelMap) {
        for (auto instance : instances) {
            instancesData[drawCount].worldMatrix = instance->GetWorldMatrix();
            //instancesData[drawCount].worldInverseTransposeMatrix = instancesData[drawCount].worldMatrix.Inverse().Transpose();
            if (auto material = instance->GetMaterial()) {
                SetMaterial(instancesData[drawCount], *material);
            }
            else {
                auto& originalMaterial = model->GetMaterials()[model->GetMeshes()[0].material];
                SetMaterial(instancesData[drawCount], originalMaterial);
            }
            instancesData[drawCount].alpha = instance->GetAlpha();
            ++drawCount;
        }
    }

    // 現状のインスタンシングのバッファじゃ足りないので再生成
    if (allocateBufferSize > instancingBuffer_.GetBufferSize()) {
        instancingBuffer_.Create(L"GeoemtryPass InstancingBuffer", drawCount, sizeof(InstanceData));
    }


    // Uploadバッファからコピー
    commandContext.CopyBufferRegion(instancingBuffer_, 0, alloc.resource, alloc.offset, allocateBufferSize);
    commandContext.TransitionResource(instancingBuffer_, D3D12_RESOURCE_STATE_GENERIC_READ);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvs[] = {
        albedo_.GetRTV(),
        metallicRoughness_.GetRTV(),
        normal_.GetRTV()
    };
    commandContext.SetRenderTargets(_countof(rtvs), rtvs, depth_.GetDSV());
    commandContext.SetViewportAndScissorRect(0, 0, albedo_.GetWidth(), albedo_.GetHeight());
    commandContext.SetRootSignature(rootSignature_);
    commandContext.SetPipelineState(pipelineState_);
    commandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    SceneData sceneData;
    sceneData.viewMatrix = camera.GetViewMatrix();
    sceneData.projectionMatrix = camera.GetProjectionMatrix();
    sceneData.cameraPosition = camera.GetPosition();
    commandContext.SetDynamicConstantBufferView(RootIndex::Scene, sizeof(sceneData), &sceneData);

    commandContext.SetBindlessResource(RootIndex::BindlessTexture);
    commandContext.SetDescriptorTable(RootIndex::Instances, instancingBuffer_.GetSRV());

    uint32_t instanceOffset = 0;
    for (auto& [model, instances] : modelMap) {
        commandContext.SetConstants(RootIndex::InstanceOffset, instanceOffset);

        commandContext.SetVertexBuffer(0, model->GetVertexBuffer().GetVertexBufferView());
        commandContext.SetIndexBuffer(model->GetIndexBuffer().GetIndexBufferView());
        
        for (auto& mesh : model->GetMeshes()) {
            commandContext.DrawIndexedInstanced((UINT)mesh.indexCount, (UINT)instances.size(), mesh.indexOffset, mesh.vertexOffset);
        }
        instanceOffset += static_cast<uint32_t>(instances.size());
    }

    //    auto& instances = modelSorter.GetDrawModels();
    //    for (auto instance : instances) {
    //        auto model = instance->GetModel();
    //
    //        InstanceData instanceData;
    //        instanceData.worldMatrix = /*model->GetRootNode().localMatrix **/ instance->GetWorldMatrix();
    //        instanceData.worldInverseTransposeMatrix = instanceData.worldMatrix.Inverse().Transpose();
    //        commandContext.SetDynamicConstantBufferView(RootIndex::InstanceOrOffset, sizeof(instanceData), &instanceData);
    //
    //        auto instanceMaterial = instance->GetMaterial();
    //
    //        for (auto& mesh : model->GetMeshes()) {
    //            MaterialData materialData = ErrorMaterial();
    //            // インスタンスのマテリアルを優先
    //            if (instanceMaterial) {
    //                SetMaterialData(materialData, *instanceMaterial);
    //            }
    //            // メッシュのマテリアル
    //            else if (mesh.material < model->GetMaterials().size()) {
    //                SetMaterialData(materialData, model->GetMaterials()[mesh.material]);
    //            }
    //            commandContext.SetDynamicConstantBufferView(RootIndex::Material, sizeof(materialData), &materialData);
    //
    //            auto skeleton = instance->GetSkeleton();
    //            auto vbv = model->GetVertexBuffer().GetVertexBufferView();
    //            if (skeleton) {
    //                auto skinCluster = RenderManager::GetInstance()->GetSkinningManager().GetSkinCluster(skeleton.get());
    //                if (skinCluster) {
    //                    vbv = skinCluster->GetSkinnedVertexBuffer().GetVertexBufferView();
    //                }
    //            }
    //            commandContext.SetVertexBuffer(0, vbv);
    //            commandContext.SetIndexBuffer(model->GetIndexBuffer().GetIndexBufferView());
    //            commandContext.DrawIndexed((UINT)mesh.indexCount, mesh.indexOffset, mesh.vertexOffset);
    //        }
    //    }
}
