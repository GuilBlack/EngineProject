float inverseLerp(float v, float minValue, float maxValue) 
{
  return (v - minValue) / (maxValue - minValue);
}

float remap(float v, float inMin, float inMax, float outMin, float outMax) {
  float t = inverseLerp(v, inMin, inMax);
  return lerp(outMin, outMax, t);
}

float3 toSRGB(float3 value) {
  float3 lt = float3(value.x < 0.0031308, value.y < 0.0031308, value.z < 0.0031308);
  float3 v1 = value * 12.92;
  float3 v2 = pow(value, float3(0.41666, 0.41666, 0.41666)) * 1.055 - float3(0.055, 0.055, 0.055);

  return lerp(v2, v1, lt);
}