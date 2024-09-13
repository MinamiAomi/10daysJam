#include "GeometryPass.hlsli"

struct VSInput {
    float3 position : POSITION0;
    float4 normal : NORMAL0;
    float4 tangent : TANGENT0;
    float2 texcoord : TEXCOORD0;
    uint instanceId : SV_INSTANCEID;
};

struct VSOutput {
    float4 svPosition : SV_POSITION;
    float3 worldPosition : POSITION0;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
    uint instanceId : SV_INSTANCEID;
};

VSOutput main(VSInput input) {
    VSOutput output;
        
    float4 localPosition = float4(input.position, 1.0f);
    float3 localNormal = input.normal.xyz * 2.0f - 1.0f;
                
    Instance instance = g_Instances[input.instanceId + g_InstanceOffset.offset];
    output.instanceId = input.instanceId;

    float4 worldPosition = mul(localPosition, instance.worldMatrix);
    output.svPosition = mul(worldPosition, mul(g_Scene.viewMatrix, g_Scene.projectionMatrix));
    output.worldPosition = worldPosition.xyz;
    output.normal = mul(localNormal, (float3x3) instance.worldMatrix);
    output.texcoord = input.texcoord;
    
    return output;
}