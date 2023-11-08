#include "include/basic-particles.hlsli"

struct VertexInput
{
    float3 PosL : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VertexOutput
{
    float4 PosH : SV_POSITION;
    nointerpolation float4 Color : COLOR;
};

VertexOutput VS(VertexInput vIn, uint instanceID : SV_InstanceID)
{
    VertexOutput vOut = (VertexOutput)0.0f;

    ParticleData instData = gParticlesData[instanceID];

    float4 posW = mul(float4(vIn.PosL, 1.0f), gWorld);
    posW = mul(posW, instData.World);

    vOut.PosH = mul(posW, gViewProj);
    
    vOut.Color = lerp(instData.BeginColor, instData.EndColor, instData.TTLRatio);

    return vOut;
}

float4 PS(VertexOutput pIn) : SV_TARGET
{
    clip(pIn.Color.a - 0.3f);
    return pIn.Color;
}