#include "GeometryPass.hlsli"

// 法線マップ、MetalliccRoughnessマップ不使用

struct PSInput {
    float4 svPosition : SV_POSITION;
    float3 worldPosition : POSITION0;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
    uint instanceId : SV_INSTANCEID;
};

struct PSOutput {
    float4 albedo : SV_TARGET0;
    float2 metallicRoughness : SV_TARGET1;
    float4 normal : SV_TARGET2;
};

PSOutput main(PSInput input) {

    PSOutput output;

    Instance instance = g_Instances[input.instanceId + g_InstanceOffset.offset];
    
    float3 albedo = g_BindlessTextures[instance.albedoMapIndex].Sample(g_Sampler, input.texcoord).xyz;
    albedo *= instance.albedo;
    output.albedo.xyz = albedo;
    /*output.albedo.w = instance.alpha;
    if (output.albedo.w == 0.0f) {
        discard;
    }*/
    output.albedo.w = 1.0f;
    
    float2 metallicRoughness = float2(instance.metallic, instance.roughness);
    output.metallicRoughness = metallicRoughness;
    
    float3 normal = normalize(input.normal);
    output.normal.xyz = (normal + 1.0f) * 0.5f;
    output.normal.w = 1.0f;
  
    return output;
}