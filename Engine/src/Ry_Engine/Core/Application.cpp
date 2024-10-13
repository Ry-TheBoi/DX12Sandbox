#include "Ry-Engine_PCH.h"
#include "Application.h"

#include <windowsx.h>

#include "Ry_Engine/Renderer/D3D12/Debug/DXGIDebug.h"

extern bool g_ApplicationRunning;

static Ry_Engine::Application* s_Instance = nullptr;

namespace Ry_Engine
{
	LRESULT CALLBACK WindProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg){

		case WM_NCCREATE: {
			LPCREATESTRUCT param = reinterpret_cast<LPCREATESTRUCT>(lparam);
			Application* pointer = reinterpret_cast<Application*>(param->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pointer));
			std::cout << "Created a window!\n";
			break;
		}

		case WM_CREATE: {
			Application::Get().OnCreate(hwnd);
			break;
		}

		case WM_DESTROY: {
			Application::Get().OnDestroy();
			PostQuitMessage(0);
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

	void Application::OnCreate(HWND hwnd)
	{
		std::cout << "Created Real Window!\n";
		m_Renderer.Init(hwnd);
	}

	void Application::OnDestroy()
	{
		m_Running = false;
		DXGIDebug::Get().GetLiveObjects();
		std::cout << "Window Closed - Terminating program\n";
	}

	void Application::Run()
	{
		while (m_Running)
		{
			MSG message;
			while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
				TranslateMessage(&message);
				DispatchMessage(&message);

			}
			//std::cout << "Hello, this is a while loop message! \n";
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

		m_Running = true;
	}

	void Application::Shutdown()
	{
		g_ApplicationRunning = false;
	}
}