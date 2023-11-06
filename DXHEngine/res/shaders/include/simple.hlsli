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
}
