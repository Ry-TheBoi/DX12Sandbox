#include "Ry-Engine_PCH.h"
#include "HLSLShader.h"
#include <d3dcompiler.h>

namespace Ry_Engine
{
	HLSLShader::~HLSLShader()
	{
		Release();
	}

	void HLSLShader::Initialize(const LPCWSTR filename, const ShaderType shaderType)
	{
		UINT flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ALL_RESOURCES_BOUND;
		const char* target = "";
		switch (shaderType) {
		case ShaderType::VERTEX: {
			target = "vs_5_1";
			break;
		}
		case ShaderType::PIXEL: {
			target = "ps_5_1";
			break;
		}
		default: {
			std::cout << "Error unspported shader compilation target\n";
			return;
		}
		}
		ID3DBlob* errorMsg = nullptr;
		HRESULT result = D3DCompileFromFile(filename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", target, flags, 0, &m_ByteCode, &errorMsg);
		if (result != S_OK) {
			std::cout << "Error in loading shader!: " << result << std::endl;
		}
		if (errorMsg) {
			std::cout << "Shader compilation error: " << (const char*)errorMsg->GetBufferPointer() << std::endl;
			return;
		}
		std::cout << "Compiled " << filename << " [Complete!]\n";
	}

	void HLSLShader::Release()
	{
		if (m_ByteCode) {
			m_ByteCode->Release();
		}
	}
}