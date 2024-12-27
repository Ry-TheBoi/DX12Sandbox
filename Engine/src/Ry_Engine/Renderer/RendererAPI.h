#pragma once
#include <Windows.h>

#include "D3D12/D3D12Device.h"
#include "D3D12/D3D12CommandQueue.h"
#include "D3D12/D3D12CommandList.h"
#include "D3D12/D3D12SwapChain.h"
#include "D3D12/D3D12Resource.h"
#include "D3D12/D3D12Pipeline.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace Ry_Engine
{
	class RendererAPI
	{
	public:
		RendererAPI() = default;
		~RendererAPI();

		void Init(HWND hwnd, const UINT width, const UINT height);
		void Update();
		void ReleaseAll();
	private:
		D3D12Device m_Device; //TODO: when implementing in Ry-Engine, we'll do this all under D3D12Core
		D3D12CommandQueue m_CommandQueue;
		D3D12CommandList m_CommandList;
		D3D12SwapChain m_SwapChain;
		D3D12Resource m_DynamicVertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_DynamicVBView;
		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT m_SRRect;
		D3D12Pipeline m_Pipeline;

		D3D12Resource m_CBPassData;

		glm::mat4 m_ViewProjectionMatrix;
	private:
		UINT m_Width;
		UINT m_Height;
	private: //temp testvars
		UINT m_Curentidx = 0;
	};
}