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
    float2 TexC : TEXCOORD;
};


VertexOutput VS(VertexInput vIn)
{
    VertexOutput vOut;
    
    float4 pos = mul(float4(vIn.PosL, 1.0f), gProj); // Transform to world space
    vOut.PosH = float4(pos.xy, 0.f, 1.0f);
    vOut.TexC = vIn.TexC;

    return vOut;
}

float4 PS(VertexOutput pIn) : SV_Target
{
    float4 color = gDiffuseTexture.Sample(gsamLinear, pIn.TexC); // Sample the texture atlas
    clip(color.a - 0.05f); // Clip if alpha is less than 0.05 (transparent)
    return color;
}