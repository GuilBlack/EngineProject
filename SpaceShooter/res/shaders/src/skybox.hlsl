#include "../../../../DXHEngine/res/shaders/include/skybox.hlsli"
#include "../../../../DXHEngine/res/shaders/include/helpers.hlsli"

struct VertexInput
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 TexC : TEXCOORD;
};

struct VertexOutput
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};

VertexOutput VS(VertexInput vIn)
{
    VertexOutput vOut;
    
    vOut.PosL = vIn.PosL;
    
    float4 worldPos = mul(float4(vIn.PosL, 1.0f), gWorld);
    
    worldPos.xyz += gEyePosW;
    vOut.PosH = mul(worldPos, gViewProj).xyww;
    
    return vOut;
}

float4 PS(VertexOutput pIn) : SV_TARGET
{
    return gTexCube.Sample(gSamLinear, pIn.PosL);
}
