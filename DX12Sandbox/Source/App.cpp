#include "App.h"

namespace Ry_Engine
{
	AppWindow::AppWindow()
	{
	}

	AppWindow::~AppWindow()
	{
	}

	void AppWindow::onCreate()
	{
		Window::init();

		RECT rc = this->getWindowRect();
		HWND window = this->getWindow();

		//Initialize Renderer
		m_Renderer = Renderer(window, rc.right - rc.left, rc.bottom - rc.top);
	}

	void AppWindow::onUpdate()
	{
		Window::onUpdate();
	}

	void AppWindow::onDestroy()
	{
		Window::onDestroy();
		//m_Renderer = nullptr;
	}
}