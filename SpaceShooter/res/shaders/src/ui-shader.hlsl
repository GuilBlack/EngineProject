#include "../../../../DXHEngine/res/shaders/include/texture-lighting.hlsli"

struct VertexInput
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 TexC : TEXCOORD;
};

struct VertexOutput
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 TexC : TEXCOORD;
};


VertexOutput VS(VertexInput vIn)
{
    VertexOutput vOut;
    float4 posW = mul(float4(vIn.PosL, 1.0f), gWorld);
    vOut.PosW = posW.xyz;

    vOut.PosH = mul(posW, gViewProj);
    vOut.NormalW = mul(vIn.NormalL, (float3x3) gWorld);
    vOut.TexC = vIn.TexC;

    return vOut;
}

float4 PS(VertexOutput pIn) : SV_Target
{
    return float4(1.f, 1.f, 1.f, 1.f);
}