#include "Ry-Engine_PCH.h"
#include "D3D12Device.h"

namespace Ry_Engine
{
	D3D12Device::D3D12Device(IDXGIAdapter* pAdapter)
	{
		Init(pAdapter);
	}

	D3D12Device::~D3D12Device()
	{
	}

	void D3D12Device::Init(IDXGIAdapter* pAdapter)
	{
		DX_CALL(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(GetAddressOf())));
		std::cout << "[D3D12Device]: Created Virtual Device!\n";
	}
}