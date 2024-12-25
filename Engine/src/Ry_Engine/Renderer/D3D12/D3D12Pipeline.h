#pragma once
#include "D3D12Core.h"
#include "D3D12RootSignature.h"
#include "HLSLShader.h"

namespace Ry_Engine
{
	class D3D12Pipeline : public Microsoft::WRL::ComPtr<ID3D12PipelineState>
	{
	public:
		D3D12Pipeline() = default;
		~D3D12Pipeline();
		void Initialize(ID3D12Device* pDevice);
		void Release();
	private:
		D3D12RootSignature m_RootSignature;
		HLSLShader m_Shaders[2]; //Ry: For now, a basic vertex & pixel shader
	};
}