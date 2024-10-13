#include "Ry-Engine_PCH.h"
#include "D3D12Debug.h"

namespace Ry_Engine
{
	D3D12Debug D3D12Debug::m_Instance;


	void D3D12Debug::EnableDebug()
	{
		GetInterface();
		Get()->EnableDebugLayer();
		std::cout << "[D3D12Debug]: Enabled Debug Layer!\n";
	}

	void D3D12Debug::GetInterface()
	{
		if (!Get())
		{
			DX_CALL(D3D12GetDebugInterface(IID_PPV_ARGS(GetAddressOf())));
		}
	}
}