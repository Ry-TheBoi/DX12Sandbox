#include "Window.h"

namespace Ry_Engine
{

	Window* window = nullptr;

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{

		case WM_CREATE:
		{
			//Event fired when the window will be created
			Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
			window->setHWND(hwnd);
			window->onCreate();
			break;
		}

		case WM_DESTROY:
		{
			//Even fired when the window needs to be destroyed
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onDestroy();
			::PostQuitMessage(0);
			break;
		}

		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);

		}

		return NULL;
	}

	Window::Window()
	{
	}

	bool Window::init()
	{
		WNDCLASSEX wc;
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc.hInstance = NULL;
		wc.lpszClassName = L"Ry_Engine";
		wc.lpszMenuName = L"";
		wc.style = NULL;
		wc.lpfnWndProc = &WndProc;

		if (!::RegisterClassEx(&wc))
			return false;

		//Create window
		m_Hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"Ry_Engine", L"Ry Engine - DirectX 12 Sandbox", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 1600, 900, NULL, NULL, NULL, this);

		if (!m_Hwnd)
			return false;

		//show window
		::ShowWindow(m_Hwnd, SW_SHOW);
		::UpdateWindow(m_Hwnd);


		m_Running = true; //Set this running flag to true

		return true;
	}

	bool Window::broadcast()
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		this->onUpdate();

		Sleep(0);

		return true;
	}

	bool Window::isRunning()
	{
		return m_Running; //returns the state if the Application is running
	}

	RECT Window::getWindowRect()
	{
		RECT rc;
		::GetClientRect(this->m_Hwnd, &rc);
		return rc;
	}

	void Window::setHWND(HWND hwnd)
	{
		this->m_Hwnd = hwnd;
	}

	bool Window::release()
	{
		//Destroy window
		if (!::DestroyWindow(m_Hwnd))
			return false;

		return true;
	}

	void Window::onCreate()
	{
	}

	void Window::onUpdate()
	{
	}

	void Window::onDestroy()
	{
		m_Running = false;
	}

	Window::~Window()
	{
	}

}