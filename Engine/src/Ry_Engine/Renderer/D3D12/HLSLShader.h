#pragma once
#include "D3D12Core.h"

namespace Ry_Engine {

	class HLSLShader
	{
	public:
		enum ShaderType {
			VERTEX,
			PIXEL,
		};
	public:
		HLSLShader() = default;
		~HLSLShader();
		void Initialize(const LPCWSTR filename, const ShaderType shaderType);
		void Release();

	private:
		ID3DBlob* mByteCode = nullptr;
	};
}
