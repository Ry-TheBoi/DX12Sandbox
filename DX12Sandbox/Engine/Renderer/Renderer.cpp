#include "Renderer.h"
#include <iostream>
#include <string>

namespace Ry_Engine
{
	Renderer::Renderer(HWND window, uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{	
		for (size_t i = 0; i < m_BackbufferCount; ++i)
			m_RenderTargets[i] = nullptr;

		InitDX12API(window);
		LogInfo();
		std::cout << "DirectX 12 Initialized! \n";
	}

	Renderer::~Renderer()
	{
		//Clear current swapchains

		if (m_Swapchain != nullptr)
		{
			m_Swapchain->SetFullscreenState(false, nullptr);
			m_Swapchain->Release();
			m_Swapchain = nullptr;
		}

		//TODO (eventually..)
		DestroyDX12API();
	}

	void Renderer::InitDX12API(HWND window)
	{
		m_Window = &window;

		//Factory Creation
		UINT dxgiFactoryFlags = 0;

#if defined (RYAPP_DEBUG)
		ID3D12Debug* debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		ThrowIfFailed(debugController->QueryInterface(IID_PPV_ARGS(&m_DebugController)));
		m_DebugController->EnableDebugLayer();
		m_DebugController->SetEnableGPUBasedValidation(true);

		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

		debugController->Release();
		debugController = nullptr;
#endif

		HRESULT result = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_Factory));


		//Adapter Creation
		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != m_Factory->EnumAdapters1(adapterIndex, &m_Adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			m_Adapter->GetDesc1(&desc);

			//Don't select basic drivers
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			//Check if the adapter has DX12 support
			//Ry: I guess it's good for a game engine to also have DX11 support if older GPUs can't run DX12, unless everyone by now has a GPU that runs DX12
			if (SUCCEEDED(D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), nullptr)))
			{
				break;
			}

			//Otherwise if all else fails, don't use the adapter index
			m_Adapter->Release();
		}


		//Device Creation (finally)
		ThrowIfFailed(D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_Device)));

		//Debug Device
#if defined(RYAPP_DEBUG)
		ThrowIfFailed(m_Device->QueryInterface(&m_DebugDevice));
#endif


		//Command Queues (allowing us to send groups of draw calls "command lists"), this is where the GPU starts to do cool optimization
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		ThrowIfFailed(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)));

		
		//Command Allocator (makes command lists to define what funcs the GPU shall execute)
		ThrowIfFailed(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
		

		//Fencing for GPU Syncronization (lets the engine know when tasks have been done by the GPU, uploads things to GPU VRAM)
		UINT frameIndex;
		HANDLE fenceEvent;
		UINT64 fenceValue;

		ThrowIfFailed(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

		//Swapchains! (finally)
		//Test
		CreateSwapchain(m_Width, m_Height);
	}

	void Renderer::CreateSwapchain(uint32_t width, uint32_t height)
	{
		//Set surface size
		m_SurfaceSize.left = 0;
		m_SurfaceSize.top = 0;
		m_SurfaceSize.right = static_cast<LONG>(width);
		m_SurfaceSize.bottom = static_cast<LONG>(width);

		//Set viewport
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.Width = static_cast<float>(width);
		m_Viewport.Height = static_cast<float>(width);
		m_Viewport.MinDepth = 0.1f;
		m_Viewport.MaxDepth = 1000.0f;

		if (m_Swapchain != nullptr)
		{
			//Create render target attachments
			m_Swapchain->ResizeBuffers(m_BackbufferCount, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		}
		else
		{
			//Create the swapchain since it obviously doesn't exist
			DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
			swapchainDesc.BufferCount = m_BackbufferCount;
			swapchainDesc.Width = width;
			swapchainDesc.Height = height;
			swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapchainDesc.SampleDesc.Count = 1;

			IDXGISwapChain1* newSwapchain;
			ThrowIfFailed(m_Factory->CreateSwapChainForHwnd(m_CommandQueue, *m_Window, &swapchainDesc, nullptr, nullptr, &newSwapchain));

			HRESULT swapchainSupport = newSwapchain->QueryInterface(__uuidof(IDXGISwapChain3), (void**)&newSwapchain);

			if (SUCCEEDED(swapchainSupport))
			{
				m_Swapchain = (IDXGISwapChain3*)newSwapchain;
			}
		}

		m_FrameIndex = m_Swapchain->GetCurrentBackBufferIndex();

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = m_BackbufferCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		ThrowIfFailed(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_renderTargetViewHeap)));

		m_RTVDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		//Make frame resources
		D3D12_CPU_DESCRIPTOR_HANDLE
			rtvHandle(m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UINT n = 0; n < m_BackbufferCount; n++)
		{
			ThrowIfFailed(m_Swapchain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n])));
			m_Device->CreateRenderTargetView(m_RenderTargets[n], nullptr, rtvHandle);
			rtvHandle.ptr += (1 * m_RTVDescriptorSize);
		}
	}

	void Renderer::LogInfo()
	{
		DXGI_ADAPTER_DESC adapterDesc;

		m_Adapter->GetDesc(&adapterDesc);

		char videoCardDesc[128];
		std::string vendor, major, minor, release, build;
		LARGE_INTEGER driverVersion;
		wcstombs_s(NULL, videoCardDesc, 128, adapterDesc.Description, 128);

		if (adapterDesc.VendorId == 0x1002)
			vendor = "AMD";
		else if (adapterDesc.VendorId == 0x10DE)
			vendor = "NVIDIA";
		else if (adapterDesc.VendorId == 0x8086)
			vendor = "Intel";
		else if (adapterDesc.VendorId == 0x1414)
			vendor = "Microsoft";
		else
			vendor = "Unknown!";

		m_Adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);
		major = std::to_string(HIWORD(driverVersion.HighPart));
		minor = std::to_string(LOWORD(driverVersion.HighPart));
		release = std::to_string(HIWORD(driverVersion.LowPart));
		build = std::to_string(LOWORD(driverVersion.LowPart));

		std::cout << "- D3D12 Info:\n";
		std::cout << "[0] GPU Brand: " << vendor.c_str() << std::endl;
		std::cout << "[1] GPU Model: " << videoCardDesc << std::endl;
		std::cout << "[2] DX Version: " << major.c_str() << "." << minor.c_str() << "." << release.c_str() << "." << build.c_str() << std::endl;
	}

	void Renderer::DestroyDX12API()
	{
	}

	void Renderer::Resize(uint32_t width, uint32_t height)
	{
		m_Width = glm::clamp(width, 1u, 0xffffu);
		m_Height = glm::clamp(height, 1u, 0xffffu);
	}

	inline void Renderer::ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw std::exception();
		}
	}

	bool Renderer::Init()
	{
		return true;
	}

	bool Renderer::Release()
	{
		return true;
	}

	Renderer* Renderer::Get()
	{
		static Renderer engine;
		return &engine; // Gets the address of the instance
	}
}