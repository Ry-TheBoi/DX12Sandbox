#pragma once

#include "D3D12Core.h"

namespace Ry_Engine
{
	class D3D12CommandList : public Microsoft::WRL::ComPtr<ID3D12CommandList>
	{
	public:
		D3D12CommandList() = default;
		~D3D12CommandList();

		void Init(ID3D12Device* pDevice);
		void Release();

		void ResetCommandList();
		inline ID3D12GraphicsCommandList* GFXCmd() { return (ID3D12GraphicsCommandList*)Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_Allocator;
	};
}