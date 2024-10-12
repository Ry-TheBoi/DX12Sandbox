#pragma once

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