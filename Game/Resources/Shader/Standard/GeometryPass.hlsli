Texture2D<float4> g_BindlessTextures[] : register(t0, space1);
SamplerState g_Sampler : register(s0);

struct Scene {
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float3 cameraPosition;
};
ConstantBuffer<Scene> g_Scene : register(b0);

struct Instance {
    float4x4 worldMatrix;
    float3 albedo;
    float alpha;
    float metallic;
    float roughness;
    uint albedoMapIndex;
    uint pad;
};
struct InstanceOffset {
    uint32_t offset;
};

// GeometryPassInstancingVS
ConstantBuffer<InstanceOffset> g_InstanceOffset : register(b1);
StructuredBuffer<Instance> g_Instances : register(t0);
