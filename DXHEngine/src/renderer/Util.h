#pragma once

namespace DXH
{
inline uint32_t GetCBByteSize(uint32_t byteSize)
{
	return (byteSize + int(0xff)) & ~int(0xff);
}

//struct Camera
//{
//	DirectX::XMFLOAT4X4 View = 
//	{
//		1.0f, 0.0f, 0.0f, 0.0f,
//		0.0f, 1.0f, 0.0f, 0.0f,
//		0.0f, 0.0f, 1.0f, 0.0f,
//		0.0, 0.0f, 0.0f, 1.0f
//	};
//	DirectX::XMFLOAT4X4 Proj = 
//	{
//		1.0f, 0.0f, 0.0f, 0.0f,
//		0.0, 1.0f, 0.0f, 0.0f,
//		0.0, 0.0f, 1.0f, 0.0f,
//		0.0, 0.0f, 0.0f, 1.0f
//	};
//};
}