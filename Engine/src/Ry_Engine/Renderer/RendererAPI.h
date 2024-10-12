#pragma once
#include <Windows.h>

#include "D3D12/D3D12Device.h"

namespace Ry_Engine
{
	class RendererAPI
	{
	public:
		RendererAPI() = default;
		~RendererAPI();

		void Init(HWND hwnd);
	private:
		D3D12Device m_Device; //TODO: when implementing in Ry-Engine, we'll do this all under D3D12Core
	};
}