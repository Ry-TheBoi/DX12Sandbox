#include "Ry-Engine_PCH.h"
#include "D3D12Pipeline.h"

namespace Ry_Engine
{
	D3D12Pipeline::~D3D12Pipeline()
	{
		Release();
	}

	void D3D12Pipeline::Initialize(ID3D12Device* pDevice)
	{
		//Initialize root sig and shaders here for now
		m_RootSignature.Initialize(pDevice);

		m_Shaders[0].Initialize(L"../ResourceLibrary/Shaders/VS.hlsl", HLSLShader::ShaderType::VERTEX);
		m_Shaders[1].Initialize(L"../ResourceLibrary/Shaders/PS.hlsl", HLSLShader::ShaderType::PIXEL);

		//Create pipeline state
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
		desc.pRootSignature = m_RootSignature.Get();
		desc.VS.pShaderBytecode = m_Shaders[0].GetByteBlob()->GetBufferPointer();
		desc.VS.BytecodeLength = m_Shaders[0].GetByteBlob()->GetBufferSize();
		desc.PS.pShaderBytecode = m_Shaders[1].GetByteBlob()->GetBufferPointer();
		desc.PS.BytecodeLength = m_Shaders[1].GetByteBlob()->GetBufferSize();
		desc.BlendState.AlphaToCoverageEnable = false;
		desc.BlendState.IndependentBlendEnable = false;
		desc.BlendState.RenderTarget[0].BlendEnable = false;
		desc.BlendState.RenderTarget[0].LogicOpEnable = false;
		desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		desc.SampleMask = 0xFFFFFFFF;
		desc.SampleDesc = { 1,0 };
		//desc.RasterizerState = {};

		desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //might do none
		desc.RasterizerState.FrontCounterClockwise = false;
		desc.RasterizerState.DepthClipEnable = true;
		desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		//desc.DepthStencilState = {};
		desc.DepthStencilState.DepthEnable = false;
		desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		D3D12_INPUT_ELEMENT_DESC elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		D3D12_INPUT_LAYOUT_DESC inputLayout = {};
		inputLayout.NumElements = 2;
		inputLayout.pInputElementDescs = elements;
		desc.InputLayout = inputLayout;
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.NodeMask = 0;
		desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		DX_CALL(pDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(GetAddressOf())), "Error creating the pipeline state object");
		std::cout << "[D3D12Device]: Created Pipeline State\n";
	}

	void D3D12Pipeline::Release()
	{
		m_RootSignature.Release();
		m_Shaders[0].Release();
		m_Shaders[1].Release();
		if (Get()) {
			Reset();
		}
	}
}