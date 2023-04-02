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
	}

	void AppWindow::onUpdate()
	{
		Window::onUpdate();
	}

	void AppWindow::onDestroy()
	{
		Window::onDestroy();
	}
}