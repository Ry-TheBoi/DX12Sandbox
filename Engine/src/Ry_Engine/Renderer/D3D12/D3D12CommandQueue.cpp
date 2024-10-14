#include "Ry-Engine_PCH.h"
#include "D3D12CommandQueue.h"

namespace Ry_Engine
{
	D3D12CommandQueue::~D3D12CommandQueue()
	{
		Release();
	}

	void D3D12CommandQueue::Init(ID3D12Device* pDevice)
	{
		//Direct commandqueue 
		D3D12_COMMAND_QUEUE_DESC description = {};
		description.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		description.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
		description.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		description.NodeMask = 0;

		DX_CALL(pDevice->CreateCommandQueue(&description, IID_PPV_ARGS(GetAddressOf())));
		DX_CALL(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_Fence.GetAddressOf())));

		std::cout << "[D3D12CommandQueue]: Created Basic Command Queue\n";
	}

	void D3D12CommandQueue::Release()
	{
		if (Get())
			Reset();

		if (m_Fence.Get())
			m_Fence.Reset();
	}

	void D3D12CommandQueue::ExecuteCommandList(ID3D12CommandList* pCommandList)
	{
		Get()->ExecuteCommandLists(1, (ID3D12CommandList* const*)&pCommandList);
		m_CurrentFenceValue++;
		Get()->Signal(m_Fence.Get(), m_CurrentFenceValue);
	}

	void D3D12CommandQueue::FlushQueue()
	{
		if (Get()) {
			for (unsigned int i = 0; i < 2; i++) { //Hardcoded max swapchain buffers to 2 for now
				Get()->Signal(m_Fence.Get(), m_CurrentFenceValue + i);
			}
		}
	}
}