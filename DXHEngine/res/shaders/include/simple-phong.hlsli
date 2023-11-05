cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
};

cbuffer cbMaterial : register(b1)
{
    float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float gRoughness;
};

cbuffer cbPass : register(b2)
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
