#pragma once

#include "D3D12Core.h"

namespace Ry_Engine
{
	class D3D12CommandQueue : public Microsoft::WRL::ComPtr<ID3D12CommandQueue>
	{
	public:
		D3D12CommandQueue() = default;
		~D3D12CommandQueue();

		void Init(ID3D12Device* pDevice);
		void Release();

		inline ID3D12Fence* GetFence() { return m_Fence.Get(); }
		inline UINT64 GetCurrentFenceValue() { return m_CurrentFenceValue; }
		void ExecuteCommandList(ID3D12CommandList* pCommandList);

		void FlushQueue();
	private:
		Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
		UINT64 m_CurrentFenceValue = 0;
	};
}