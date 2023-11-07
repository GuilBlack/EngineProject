#include "include/number.hlsli"

struct VertexInput 
{
    float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
    float2 TexC    : TEXCOORD;
};

struct VertexOutput 
{
    float4 PosH    : SV_POSITION;
	float2 TexC    : TEXCOORD;
};

VertexOutput VS(VertexInput vIn)
{
    VertexOutput vOut;
    float4 pos = mul(float4(vIn.PosL / 5, 1.0f), gWorld);
    vOut.PosH = float4(pos.xy, 0.0f, 1.0f);
    vOut.TexC = vIn.TexC;

    return vOut;
}

float4 PS(VertexOutput pIn) : SV_Target
{
    float4 color = gNumberAtlas.Sample(gsamLinear, pIn.TexC);
    clip(color.a - 0.1f);

    return color;
}