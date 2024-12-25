#include "Ry-Engine_PCH.h"
#include "RendererAPI.h"

#include "Ry_Engine/Renderer/D3D12/D3D12Factory.h"
#include "Ry_Engine/Renderer/D3D12/D3D12Adapter.h"
#include "Ry_Engine/Renderer/D3D12/Debug/D3D12Debug.h"
#include "Ry_Engine/Renderer/D3D12/HLSLShader.h"

#define KBs(x) 1024* (x)

namespace Ry_Engine
{
	struct Vertex {
		glm::vec3 position = { 0.0f,0.0f,0.0f };
		glm::vec4 color = { 0.0f,0.0f,0.0f,1.0f };
	};

	RendererAPI::~RendererAPI()
	{
		ReleaseAll();
	}

	void RendererAPI::Init(HWND hwnd, const UINT width, const UINT height)
	{
		//Set properties
		m_Width = width;
		m_Height = height;

		//Enable debug layer if needed
#ifdef RYAPP_DEBUG
		D3D12Debug::Get().EnableDebug();
#endif

		//Note: all of this would ideally fit in to a abstracted graphics context class if implimented to Ry-Engine

		//Create factory
		D3D12Factory factory;

		//Create Adapter
		D3D12Adapter adapter = factory.GetAdapter();

		//Get description
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wcout << "Selected Device: " << desc.Description << std::endl;

		//Initialize device
		m_Device.Init(adapter.Get());
		m_Device->SetName(L"Ry-Engine GPU Device");

		//Initialize command buffers and queues
		m_CommandQueue.Init(m_Device.Get());
		m_CommandList.Init(m_Device.Get());

		//Initialize Swapchain
		m_SwapChain.Init(m_Device.Get(), factory.Get(), m_CommandQueue.Get(), hwnd, m_Width, m_Height);

		m_DynamicVertexBuffer.Initialize(m_Device.Get(), KBs(16), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);
		m_DynamicVertexBuffer.Get()->SetName(L"Dynamic vertex buffer");
		Vertex vertexData;
		vertexData.position = { 1.0f,5.0f,3.0f };
		vertexData.color = { 0.0f,1.0f,0.0f,1.0f };
		void* destination = nullptr;
		m_DynamicVertexBuffer->Map(0, 0, &destination);
		memcpy(destination, &vertexData, sizeof(Vertex));
		m_DynamicVertexBuffer->Unmap(0, 0);

		HLSLShader testShader;
		testShader.Initialize(L"../ResourceLibrary/Shaders/VS.hlsl", HLSLShader::ShaderType::VERTEX);
	}

	void RendererAPI::Update() //Ry: Note, if this was actually in Ry-Engine, this would be wrapped under the actual API renderer abstraction (e,g: D3D12RendererAPI class ideally if implimented)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_SwapChain.GetCurrentRenderTarget();
		barrier.Transition.Subresource = 0;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		m_CommandList.GFXCmd()->ResourceBarrier(1, &barrier);

		const float clearColor[] = { 0.25f,0.50f,1.0f,1.0f };

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_SwapChain.GetCurrentRTVHandle();
		m_CommandList.GFXCmd()->ClearRenderTargetView(rtvHandle, clearColor, 0, 0);
		barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_SwapChain.GetCurrentRenderTarget();
		barrier.Transition.Subresource = 0;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		m_CommandList.GFXCmd()->ResourceBarrier(1, &barrier);

		m_CommandList.GFXCmd()->Close();
		m_CommandQueue.ExecuteCommandList(m_CommandList.Get());
		m_SwapChain.Present();

		while (m_CommandQueue.GetFence()->GetCompletedValue() < m_CommandQueue.GetCurrentFenceValue()) 
		{
			_mm_pause();
		}
		m_CommandList.ResetCommandList();
	}

	void RendererAPI::ReleaseAll()
	{
		m_DynamicVertexBuffer.Release();
		m_CommandQueue.FlushQueue();
		m_SwapChain.Release();
		m_CommandList.Release();
		m_CommandQueue.Release();

		if (m_Device.Get())
		{
			m_Device.Reset();
		}
	}


}