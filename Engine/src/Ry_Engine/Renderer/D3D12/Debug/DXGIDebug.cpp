#include "Ry-Engine_PCH.h"
#include "DXGIDebug.h"

namespace Ry_Engine
{
	DXGIDebug DXGIDebug::m_Instance;

	void DXGIDebug::EnableDebug()
	{
		GetInterface();
		std::cout << "[DXGIDebug]: Enabled Debug Layer!\n";
	}

	void DXGIDebug::GetLiveObjects()
	{
		GetInterface();
		Get()->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}

	void DXGIDebug::GetInterface()
	{
		if (!Get())
		{
			DX_CALL(DXGIGetDebugInterface1(0, IID_PPV_ARGS(GetAddressOf())));
		}
	}
}