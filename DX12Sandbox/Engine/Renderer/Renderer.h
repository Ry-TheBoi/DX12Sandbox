#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <exception>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Ry_Engine
{
	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(HWND window, uint32_t width, uint32_t height);
		~Renderer();

		bool Init();
		bool Release();

		//Resources
		void InitResources();
	public:
		//DX12
		void InitDX12API(HWND window);
		void CreateSwapchain(uint32_t width, uint32_t height);
		void LogInfo();
		void DestroyDX12API();

		void Resize(uint32_t width, uint32_t height);

		inline void ThrowIfFailedFunc(HRESULT hr, std::string debugName);
	public:
		static Renderer* Get();
	private:
		HWND* m_Window;

		//Devices
		ID3D12Device* m_Device = nullptr;
		ID3D12DebugDevice* m_DebugDevice = nullptr;
		D3D_FEATURE_LEVEL m_FeatureLevel;

		//Factory
		IDXGIFactory4* m_Factory = nullptr;
		ID3D12Debug1* m_DebugController = nullptr;

		//Adapter
		IDXGIAdapter1* m_Adapter = nullptr;

		//Command Queue
		ID3D12CommandQueue* m_CommandQueue = nullptr;

		//Command Allocator
		ID3D12CommandAllocator* m_CommandAllocator = nullptr;

		//Fencing
		ID3D12Fence* m_Fence = nullptr;

		//Swapchain
		uint32_t m_Width = 1280;
		uint32_t m_Height = 720;

		IDXGISwapChain3* m_Swapchain = nullptr;
		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT m_SurfaceSize;

		//Current frame handles
		static const UINT m_BackbufferCount = 2;
		UINT m_CurrentBuffer;

		//Descriptor heaps (objs that handle memory allocation required for storing descriptions of objs that shaders need to reference)
		ID3D12DescriptorHeap* m_RenderTargetViewHeap = nullptr;
		ID3D12Resource* m_RenderTargets[m_BackbufferCount];
		UINT m_RTVDescriptorSize;

		//Root signatures: (objs that define what type of resources can be accessed by the shader (i.e constant / uniform buffers, samplers, textures, etc))
		ID3D12RootSignature* m_RootSignature = nullptr;

		//Syncs
		UINT m_FrameIndex;
	};
}

