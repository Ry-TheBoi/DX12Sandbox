#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <Windows.h>

#include "Ry_Engine/Renderer/RendererAPI.h"

namespace Ry_Engine
{
	class Application
	{
	public:
		Application();
		~Application();

		void OnCreate(HWND hwnd);
		void OnDestroy();
		void Run();

		static Application& Get();
	private:
		void Init();
		void Shutdown();

	private:
		HWND m_WindowHandle = nullptr;
		bool m_Running = false;
		RendererAPI m_Renderer;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}