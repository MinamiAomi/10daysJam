#define USE_INSTANCING
#include "GeometryPass.hlsli"

// 法線マップ、MetalliccRoughnessマップ不使用

struct PSInput {
    float4 svPosition : SV_POSITION;
    float3 worldPosition : POSITION0;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
    #ifdef USE_INSTANCING
    uint instanceId : SV_INSTANCEID;
#endif
};

struct PSOutput {
    float4 albedo : SV_TARGET0;
    float2 metallicRoughness : SV_TARGET1;
    float4 normal : SV_TARGET2;
};

PSOutput main(PSInput input) {

    PSOutput output;

#ifndef USE_INSTANCING
    Instance instance = g_Instance;
#else
    Instance instance = g_Instances[input.instanceId + g_InstanceOffset.offset];
#endif
    
    float3 albedo = g_BindlessTextures[g_Material.albedoMapIndex].Sample(g_Sampler, input.texcoord).xyz;
    albedo *= g_Material.albedo;
    output.albedo.xyz = albedo;
    output.albedo.w = 1.0f;
    
    float2 metallicRoughness = float2(g_Material.metallic, g_Material.roughness);
    output.metallicRoughness = metallicRoughness;
    
    float3 normal = normalize(input.normal);
    output.normal.xyz = (normal + 1.0f) * 0.5f;
    output.normal.w = 1.0f;
  
    return output;
}