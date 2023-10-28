#pragma once

namespace DXH
{
inline uint32_t GetCBByteSize(uint32_t byteSize)
{
	return (byteSize + int(0xff)) & ~int(0xff);
}
}