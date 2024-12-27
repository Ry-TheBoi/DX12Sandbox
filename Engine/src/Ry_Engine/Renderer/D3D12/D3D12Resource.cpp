#include "Ry-Engine_PCH.h"
#include "D3D12Resource.h"

namespace Ry_Engine
{
	D3D12Resource::~D3D12Resource()
	{
		Release();
	}

	void D3D12Resource::Initialize(ID3D12Device* pDevice, const unsigned int numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState)
	{
		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = heapType;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProp.CreationNodeMask = 0;
		heapProp.VisibleNodeMask = 0;
		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = numBytes;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc = { 1,0 };
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		DX_CALL(pDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, initialState, 0, IID_PPV_ARGS(GetAddressOf())));
		std::cout << "[D3D12Resource]: Created Memory Resource!\n";
	}

	void D3D12Resource::Release()
	{
		if (m_Memory && Get()) {
			Get()->Unmap(0, 0);
		}

		if (Get()) {
			Reset();
		}
	}

	void* D3D12Resource::GetCPUMemory()
	{
		if (!m_Memory) {
			Get()->Map(0, 0, &m_Memory);
		}
		return m_Memory;
	}
}