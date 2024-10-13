#pragma once

#include "Ry_Engine/Renderer/D3D12/D3D12Core.h"

namespace Ry_Engine
{
	//Used for more application specific stuff compared to D3D12Debug which is more for D3D12 Renderer stuff
	class DXGIDebug : public Microsoft::WRL::ComPtr<IDXGIDebug>
	{
	public:
		DXGIDebug() = default;

		static inline DXGIDebug& Get() { return m_Instance; }

		void EnableDebug();
		void GetLiveObjects();
	private:
		void GetInterface();
	private:
		static DXGIDebug m_Instance;
	};
}