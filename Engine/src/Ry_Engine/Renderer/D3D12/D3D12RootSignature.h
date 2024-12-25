#pragma once
#include "D3D12Core.h"

namespace Ry_Engine
{
	class D3D12RootSignature : public Microsoft::WRL::ComPtr<ID3D12RootSignature>
	{
	public:
		D3D12RootSignature() = default;
		~D3D12RootSignature();
		void Initialize(ID3D12Device* pDevice);
		void Release();
	};
}