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

struct VertexInput 
{
	float3 PosL : POSITION;
	float4 Col : COLOR;
};

struct VertexOutput 
{
	float4 PosH : SV_POSITION;
	float4 Col : COLOR;
};

VertexOutput VS(VertexInput vIn)
{
	VertexOutput vOut;
	float4 posW = mul(float4(vIn.PosL, 1.0f), gWorld);

	vOut.PosH = mul(posW, gViewProj);
	vOut.Col = vIn.Col;

	return vOut;
}

float4 PS(VertexOutput pIn) : SV_Target
{
	return pIn.Col;
}