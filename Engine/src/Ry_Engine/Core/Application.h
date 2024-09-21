#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace Ry_Engine
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		static Application& Get();
	private:
		void Init();
		void Shutdown();

	private:
		bool m_Running = false;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}