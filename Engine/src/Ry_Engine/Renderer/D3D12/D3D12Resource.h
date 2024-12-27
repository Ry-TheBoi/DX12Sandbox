#pragma once

#include "D3D12Core.h"

namespace Ry_Engine
{
	class D3D12Resource : public Microsoft::WRL::ComPtr<ID3D12Resource>
	{
	public:
		D3D12Resource() = default;
		~D3D12Resource();

		void Initialize(ID3D12Device* pDevice, const unsigned int numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState);

		void Release();
		void* GetCPUMemory();
	private:
		void* m_Memory = nullptr;
	};
}