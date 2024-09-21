#include "Application.h"

#include <iostream>

extern bool g_ApplicationRunning;

static Ry_Engine::Application* s_Instance = nullptr;

namespace Ry_Engine
{
	Application::Application()
	{
		s_Instance = this;
		Init();
	}

	Application::~Application()
	{
		Shutdown();
		s_Instance = nullptr;
	}

	void Application::Run()
	{
		m_Running = true;

		while (m_Running)
		{
			std::cout << "Hello, this is a while loop message! \n";
		}
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

	void Application::Init()
	{
	}

	void Application::Shutdown()
	{
		g_ApplicationRunning = false;
	}
}