#include "Ry-Engine_PCH.h"
#include "D3D12Factory.h"

namespace Ry_Engine
{
	D3D12Factory::D3D12Factory()
	{
		//Create factory
#if RYAPP_DEBUG
		DX_CALL(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&ptr_)));
#else
		DX_CALL(CreateDXGIFactory2(0, IID_PPV_ARGS(&ptr_)));
#endif
	}

	D3D12Factory::~D3D12Factory()
	{
	}

	D3D12Adapter D3D12Factory::GetAdapter()
	{
		ComPtr<IDXGIFactory6> factory_v6;
		ComPtr<IDXGIAdapter> adapter;

		if (Get()->QueryInterface(IID_PPV_ARGS(&factory_v6)) == S_OK)
		{
			DX_CALL(factory_v6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));
			std::cout << "[D3D12Factory]: Created DXGI Factory!\n";
		}
		else 
		{
			std::abort();
		}

		return D3D12Adapter(adapter.Get());
	}


}