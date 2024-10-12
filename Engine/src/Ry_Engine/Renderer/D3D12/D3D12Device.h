#pragma once

#include "D3D12Core.h"

namespace Ry_Engine
{
	class D3D12Device : public Microsoft::WRL::ComPtr<ID3D12Device>
	{
	public:
		D3D12Device() = default; //in case there's a need to instantiate this class without passing a adapter
		D3D12Device(IDXGIAdapter* pAdapter);
		~D3D12Device();

		void Init(IDXGIAdapter* pAdapter);
	};
}