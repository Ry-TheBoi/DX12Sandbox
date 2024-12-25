#include "Ry-Engine_PCH.h"
#include "D3D12RootSignature.h"

namespace Ry_Engine
{
	D3D12RootSignature::~D3D12RootSignature()
	{
		Release();
	}

	void D3D12RootSignature::Initialize(ID3D12Device* pDevice)
	{
		D3D12_ROOT_PARAMETER rootParameter[1];
		rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameter[0].Descriptor.ShaderRegister = 0;
		rootParameter[0].Descriptor.RegisterSpace = 0;
		rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rsDesc = { };
		rsDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
		rsDesc.Desc_1_0.pParameters = rootParameter;
		rsDesc.Desc_1_0.NumParameters = 1;
		rsDesc.Desc_1_0.NumStaticSamplers = 0;
		rsDesc.Desc_1_0.pStaticSamplers = 0;
		rsDesc.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		ID3DBlob* pSerializedRS = nullptr;
		ID3DBlob* pErrorBlob = nullptr;
		DX_CALL(D3D12SerializeVersionedRootSignature(&rsDesc, &pSerializedRS, &pErrorBlob), "Error serializing the root signature");
		if (pErrorBlob) {
			std::cout << "[D3D12] RootSignature serialization error: " << (const char*)pErrorBlob->GetBufferPointer();
		}
		DX_CALL(pDevice->CreateRootSignature(0, pSerializedRS->GetBufferPointer(), pSerializedRS->GetBufferSize(), IID_PPV_ARGS(GetAddressOf())), "Error creating the root signature");
		std::cout << "[D3D12Factory]: Created D3D12 Root Signatures\n";
	}

	void D3D12RootSignature::Release()
	{
		if (Get()) {
			Reset();
		}
	}
}