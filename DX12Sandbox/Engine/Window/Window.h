#pragma once
#include <Windows.h>

namespace Ry_Engine
{
	class Window
	{
	public:
		Window();

		//initialize window
		bool init();
		bool broadcast();
		bool isRunning();

		RECT getWindowRect();
		void setHWND(HWND hwnd);

		//releases window
		bool release();

		//even methods
		virtual void onCreate();
		virtual void onUpdate();
		virtual void onDestroy();

		~Window();
	protected:
		HWND m_Hwnd;
		bool m_Running;
	};
}