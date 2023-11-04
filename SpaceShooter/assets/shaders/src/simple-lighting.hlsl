#include "../../../../DXHEngine/res/shaders/include/simple-phong.hlsli"

struct VertexInput 
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
};

struct VertexOutput 
{
    float4 PosW : SV_POSITION;
    float3 NormalW : NORMAL;
};

VertexOutput VS(VertexInput vIn)
{
    VertexOutput vOut;
    float4 posW = mul(float4(vIn.PosL, 1.0f), gWorld);

    vOut.PosW = mul(posW, gViewProj);
    vOut.NormalW = vIn.NormalL;

    return vOut;
}

float4 PS(VertexOutput pIn) : SV_Target
{
    
    return float4(pIn.NormalW, 1.0f);
}
