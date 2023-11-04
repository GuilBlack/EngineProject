#include "../../../../DXHEngine/res/shaders/include/simple-phong.hlsli"
#include "../../../../DXHEngine/res/shaders/include/helpers.hlsli"

struct VertexInput 
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
};

struct VertexOutput 
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
};

VertexOutput VS(VertexInput vIn)
{
    VertexOutput vOut;
    float4 posW = mul(float4(vIn.PosL, 1.0f), gWorld);
    vOut.PosW = posW.xyz;

    vOut.PosH = mul(posW, gViewProj);
    vOut.NormalW = mul(vIn.NormalL, (float3x3)gWorld);

    return vOut;
}

float4 PS(VertexOutput pIn) : SV_Target
{
    float3 normal = normalize(pIn.NormalW);
    float4 normalColor = float4(normal / 2 + 0.5f, 1.0f);
    return normalColor;
}
