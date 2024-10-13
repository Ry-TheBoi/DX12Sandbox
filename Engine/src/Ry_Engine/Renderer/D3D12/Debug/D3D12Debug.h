#pragma once

#include "Ry_Engine/Renderer/D3D12/D3D12Core.h"

namespace Ry_Engine
{
	class D3D12Debug : public Microsoft::WRL::ComPtr<ID3D12Debug>
	{
	public:
		D3D12Debug() = default;

		static inline D3D12Debug& Get() { return m_Instance; }

		void EnableDebug();
	private:
		void GetInterface();
	private:
		static D3D12Debug m_Instance;
	};
}