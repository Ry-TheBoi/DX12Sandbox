#include "Ry-Engine_PCH.h"
#include "Application.h"

#include <windowsx.h>

extern bool g_ApplicationRunning;

static Ry_Engine::Application* s_Instance = nullptr;

namespace Ry_Engine
{
	LRESULT CALLBACK WindProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg){

		case WM_NCCREATE: {
			std::cout << "Created a window!\n";
			break;
		}

		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

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
		//Create window class
		WNDCLASS wndClass = {};
		wndClass.lpszClassName = L"BaseWindowClass";
		wndClass.style = 0;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //cool
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //TODO later
		wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wndClass.lpszMenuName = 0;
		wndClass.hInstance = 0;
		wndClass.lpfnWndProc = WindProc; //function callbacks
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;

		RegisterClass(&wndClass);

		//Create window handle
		m_WindowHandle = CreateWindow(L"BaseWindowClass", L"Ry Engine: D3D12 Sandbox", WS_OVERLAPPEDWINDOW, 200, 200, 1280, 720, 0, 0, 0, 0);

		if (!m_WindowHandle)
		{
			std::cerr << "Failed to create window handle!\n";
			std::abort();
		}

		ShowWindow(m_WindowHandle, SW_SHOW);
		UpdateWindow(m_WindowHandle);
	}

	void Application::Shutdown()
	{
		g_ApplicationRunning = false;
	}
}