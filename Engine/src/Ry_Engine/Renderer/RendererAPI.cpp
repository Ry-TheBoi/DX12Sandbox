#include "Ry-Engine_PCH.h"
#include "RendererAPI.h"

#include "Ry_Engine/Renderer/D3D12/D3D12Factory.h"
#include "Ry_Engine/Renderer/D3D12/D3D12Adapter.h"
#include "Ry_Engine/Renderer/D3D12/Debug/D3D12Debug.h"

namespace Ry_Engine
{
	RendererAPI::~RendererAPI()
	{
		if (m_Device.Get())
		{
			m_Device.Reset();
		}
	}

	void RendererAPI::Init(HWND hwnd)
	{
		//Enable debug layer if needed
#ifdef RYAPP_DEBUG
		D3D12Debug::Get().EnableDebug();
#endif

		//Create factory
		D3D12Factory factory;

		//Create Adapter
		D3D12Adapter adapter = factory.GetAdapter();

		//Get description
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wcout << "Selected Device: " << desc.Description << std::endl;

		//Initialize device
		m_Device.Init(adapter.Get());
		m_Device->SetName(L"Ry-Engine GPU Device");
	}


}