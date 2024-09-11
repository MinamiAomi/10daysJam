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
    float4x4 worldInverseTransposeMatrix;
};
struct InstanceOffset {
    uint32_t offset;
};
#ifndef USE_INSTANCING
// GeometryPassVS
ConstantBuffer<Instance> g_Instance : register(b1);
#else
// GeometryPassInstancingVS
StructuredBuffer<Instance> g_Instances : register(t1);
ConstantBuffer<InstanceOffset> g_InstanceOffset : register(b1);
#endif

struct Material {
    float3 albedo;
    float metallic;
    float roughness;
    uint albedoMapIndex;
#ifndef USE_INSTANCING
    uint metallicRoughnessMapIndex;
    uint normalMapIndex;
#endif
};
ConstantBuffer<Material> g_Material : register(b2);

#ifdef ENABLE_SKINNING
struct Bone {
    float4x4 boneMatrix;
    float4x4 boneInverseTransposeMatrix;
};
StructuredBuffer<Bone> g_Bones : register(t0);
#endif // ENABLE_SKINNING