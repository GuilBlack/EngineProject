#pragma once

namespace DXH
{
struct Texture
{
    Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;

    uint32_t heapIndex = -1;
};
}
