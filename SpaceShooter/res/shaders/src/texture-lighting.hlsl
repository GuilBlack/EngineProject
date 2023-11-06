#include "../../../../DXHEngine/res/shaders/include/texture-lighting.hlsli"
#include "../../../../DXHEngine/res/shaders/include/helpers.hlsli"

struct VertexInput 
{
    float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
    float2 TexC    : TEXCOORD;
};

struct VertexOutput 
{
    float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
	float2 TexC    : TEXCOORD;
};

VertexOutput VS(VertexInput vIn)
{
    VertexOutput vOut;
    float4 posW = mul(float4(vIn.PosL, 1.0f), gWorld);
    vOut.PosW = posW.xyz;

    vOut.PosH = mul(posW, gViewProj);
    vOut.NormalW = mul(vIn.NormalL, (float3x3)gWorld);
    vOut.TexC = vIn.TexC;

    return vOut;
}

float4 PS(VertexOutput pIn) : SV_Target
{
    float4 diffuseAlbedo = gDiffuseTexture.Sample(gsamLinear, pIn.TexC) * gDiffuseAlbedo;
    float3 normal = normalize(pIn.NormalW);
    float3 viewDir = normalize(gEyePosW - pIn.PosW);
    float shininess = round((1.0f - gRoughness) * 256.0f) + ((1.0f - gRoughness) * 256.0f) % 2.0f;

    // Directional Light
    float3 dirLightPos = -normalize(gSunDirection);
    float3 dirLightAngle = max(0.0, dot(dirLightPos, normal));

    float3 diffuse = dirLightAngle * gSunColor * gSunIntensity;

    // Specular Light
    float3 halfway = normalize(viewDir + normalize(dirLightPos));
    float specularCoef = pow(max(dot(normal, halfway), 0.0), 8.0f);

    float3 specularLight = gSunColor * specularCoef;
    
    // Fresnel
    float fresnelCoef = pow(1.0 - max(0.0, dot(viewDir, normal)), 1.0);

    specularLight *= fresnelCoef;

    float3 lighting = float3(gAmbientIntensity, gAmbientIntensity, gAmbientIntensity) + diffuse;

    float3 color = toSRGB(lighting * diffuseAlbedo.rgb) + specularLight;

    return float4(color, diffuseAlbedo.a);
}
