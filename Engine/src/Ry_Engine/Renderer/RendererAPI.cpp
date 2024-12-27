#include "Ry-Engine_PCH.h"
#include "RendererAPI.h"

#include <vector>

#include "Ry_Engine/Renderer/D3D12/D3D12Factory.h"
#include "Ry_Engine/Renderer/D3D12/D3D12Adapter.h"
#include "Ry_Engine/Renderer/D3D12/Debug/D3D12Debug.h"

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

		//Initialize resources
		m_DynamicVertexBuffer.Initialize(m_Device.Get(), KBs(16), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);
		m_DynamicVertexBuffer.Get()->SetName(L"Dynamic vertex buffer");
		
		std::vector<Vertex> vertices;
		for (int i = 0; i < 3; i++) {
			Vertex vertexData;
			vertexData.color = { 1.0f,0.15f,1.0f,1.0f };
			if (i == 0) {
				vertexData.position = { -.5f,-.5f,0.0f };
			}
			else if (i == 1) {
				vertexData.position = { 0.0f,.5f,0.0f };
			}
			else {
				vertexData.position = { .5f,-.5f,0.0f };
			}
			vertices.push_back(vertexData);
		}

		void* destination = nullptr;
		m_DynamicVertexBuffer->Map(0, 0, &destination);
		memcpy(destination, vertices.data(), sizeof(Vertex) * vertices.size());
		m_DynamicVertexBuffer->Unmap(0, 0);

		m_DynamicVBView.BufferLocation = m_DynamicVertexBuffer.Get()->GetGPUVirtualAddress();
		m_DynamicVBView.StrideInBytes = sizeof(Vertex);
		m_DynamicVBView.SizeInBytes = KBs(16);

		//Initialize Pipeline
		m_Pipeline.Initialize(m_Device.Get());

		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = m_Width;
		m_Viewport.Height = m_Height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_SRRect.left = 0;
		m_SRRect.right = m_Viewport.Width;
		m_SRRect.top = 0;
		m_SRRect.bottom = m_Viewport.Height;
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
		m_CommandList.GFXCmd()->OMSetRenderTargets(1, &rtvHandle, false, 0);
		m_CommandList.GFXCmd()->RSSetViewports(1, &m_Viewport);
		m_CommandList.GFXCmd()->RSSetScissorRects(1, &m_SRRect);
		m_CommandList.GFXCmd()->SetGraphicsRootSignature(m_Pipeline.GetRootSignature());
		m_CommandList.GFXCmd()->SetPipelineState(m_Pipeline.Get());
		m_CommandList.GFXCmd()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList.GFXCmd()->IASetVertexBuffers(0, 1, &m_DynamicVBView);
		//TODO: draw stuff
		m_CommandList.GFXCmd()->DrawInstanced(3, 1, 0, 0);

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