#include "Ry-Engine_PCH.h"
#include "RendererAPI.h"

#include "Ry_Engine/Renderer/D3D12/D3D12Factory.h"
#include "Ry_Engine/Renderer/D3D12/D3D12Adapter.h"

namespace Ry_Engine
{
	RendererAPI::~RendererAPI()
	{
	}

	void RendererAPI::Init(HWND hwnd)
	{
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