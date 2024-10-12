#pragma once

#include "D3D12Core.h"

namespace Ry_Engine
{
	class D3D12Adapter : public Microsoft::WRL::ComPtr<IDXGIAdapter>
	{
	public:
		D3D12Adapter() = default;
		D3D12Adapter(IDXGIAdapter* pAdapter);
		~D3D12Adapter();
	};
}