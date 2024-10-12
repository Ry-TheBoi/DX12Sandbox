#include "Ry-Engine_PCH.h"
#include "D3D12Adapter.h"

namespace Ry_Engine
{
	D3D12Adapter::D3D12Adapter(IDXGIAdapter* pAdapter)
		: Microsoft::WRL::ComPtr<IDXGIAdapter>(pAdapter)
	{
	}

	D3D12Adapter::~D3D12Adapter()
	{
	}

}