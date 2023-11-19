TextureCube gTexCube : register(t0);
SamplerState gSamLinear : register(s0);

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
};

cbuffer cbPass : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewProj;
    float3 gEyePosW;
    float gNearZ;
    float gFarZ;
    float gTotalTime;
    float2 gRenderTargetSize;
    float gDeltaTime;
    float gAmbientIntensity;
    float gSunIntensity;
    float3 gSunDirection;
    float3 gSunColor;
};
