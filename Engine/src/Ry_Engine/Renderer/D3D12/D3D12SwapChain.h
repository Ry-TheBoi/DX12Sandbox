#pragma once

#include "D3D12Core.h"

namespace Ry_Engine
{
	class D3D12SwapChain : public Microsoft::WRL::ComPtr<IDXGISwapChain1>
	{
	public:
		D3D12SwapChain() = default;
		~D3D12SwapChain();

		void Init(ID3D12Device* pDevice, IDXGIFactory2* pFactory, ID3D12CommandQueue* pCommandQueue, const HWND hwnd, const UINT width, const UINT height);
		void CreateBuffers();
		void DropBuffers();

		void Present();

		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHandle();
		inline ID3D12Resource* GetCurrentRenderTarget() { return m_RenderTargets[m_CurrentBuffer].Get(); }

		void Release();

	private:
		ID3D12Device* m_Device = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_RenderTargets[2]; //2 is the max swapchain buffers for now
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVHeap;

		UINT m_BufferCount = 0;
		UINT m_CurrentBuffer = 0;
		UINT m_Width = 0, m_Height = 0;
		UINT m_HeapIncrement = 0;
	};
}