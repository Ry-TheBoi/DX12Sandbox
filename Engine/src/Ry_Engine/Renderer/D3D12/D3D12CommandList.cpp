#include "Ry-Engine_PCH.h"
#include "D3D12CommandList.h"

namespace Ry_Engine
{
	D3D12CommandList::~D3D12CommandList()
	{
		Release();
	}

	void D3D12CommandList::Init(ID3D12Device* pDevice)
	{
		DX_CALL(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_Allocator.GetAddressOf())));
		DX_CALL(pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_Allocator.Get(), nullptr, IID_PPV_ARGS(GetAddressOf())));
		std::cout << "[D3D12CommandList]: Created Command List\n";
	}

	void D3D12CommandList::Release()
	{
		if (Get())
			Reset();

		if (m_Allocator.Get())
			m_Allocator.Reset();
	}

	void D3D12CommandList::ResetCommandList()
	{
		m_Allocator.Get()->Reset();
		GFXCmd()->Reset(m_Allocator.Get(), 0);
	}
}