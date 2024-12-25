#include "Ry-Engine_PCH.h"
#include "D3D12SwapChain.h"

namespace Ry_Engine
{
	D3D12SwapChain::~D3D12SwapChain()
	{
		Release();
	}

	void D3D12SwapChain::Init(ID3D12Device* pDevice, IDXGIFactory2* pFactory, ID3D12CommandQueue* pCommandQueue, const HWND hwnd, const UINT width, const UINT height)
	{
		m_Width = width;
		m_Height = height;

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.NumDescriptors = 2;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;

		DX_CALL(pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_RTVHeap.GetAddressOf())));

		m_HeapIncrement = pDevice->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

		DXGI_SWAP_CHAIN_DESC1 description = {};
		description.Width = width;
		description.Height = height;
		description.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		description.Stereo = false;
		description.SampleDesc = { 1,0 };
		description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		description.BufferCount = 2;
		description.Scaling = DXGI_SCALING_NONE;
		description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		description.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		DX_CALL(pFactory->CreateSwapChainForHwnd(pCommandQueue, hwnd, &description, nullptr, nullptr, GetAddressOf()));
		m_BufferCount = 2;
		m_Device = pDevice;
		CreateBuffers();
		std::cout << "[D3D12SwapChain] Created Renderer SwapChain\n";
	}

	void D3D12SwapChain::CreateBuffers()
	{
		if (!Get()) 
		{
			std::cout << "[D3D12SwapChain] Error: Tried to create buffers for uninitialized SwapChain\n";
			return;
		}
		for (int i = 0; i < m_BufferCount; i++) {
			m_RenderTargets[i].Reset();
			DX_CALL(Get()->GetBuffer(i, IID_PPV_ARGS(m_RenderTargets[i].GetAddressOf())), "Error creating buffer for swapchain idx: " << i);
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += (size_t)m_HeapIncrement * i;
			m_Device->CreateRenderTargetView(m_RenderTargets[i].Get(), 0, cpuHandle);
		}
	}

	void D3D12SwapChain::DropBuffers()
	{
		for (int i = 0; i < m_BufferCount; i++) {
			m_RenderTargets[i].Reset();
		}
	}

	void D3D12SwapChain::Present()
	{
		Get()->Present(0, 0); //DXGI_PRESENT_ALLOW_TEARING

		m_CurrentBuffer = (m_CurrentBuffer + 1) % m_BufferCount;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE D3D12SwapChain::GetCurrentRTVHandle()
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
		cpuHandle.ptr += (size_t)m_CurrentBuffer * m_HeapIncrement;
		return cpuHandle;
	}

	void D3D12SwapChain::Release()
	{
		if (Get()) 
		{
			DropBuffers();
			m_RTVHeap.Reset();
			Reset();
		}
	}
}