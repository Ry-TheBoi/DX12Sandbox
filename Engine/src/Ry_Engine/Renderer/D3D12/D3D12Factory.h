#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

#include <wrl.h> //We won't really use this for ref counting in Ry-Engine, but for this speficic program we will
#include "D3D12Core.h"
#include "D3D12Adapter.h"

namespace Ry_Engine
{
	class D3D12Factory : public Microsoft::WRL::ComPtr<IDXGIFactory2>
	{
	public:
		D3D12Factory();
		~D3D12Factory();

		D3D12Adapter GetAdapter();
	};
}