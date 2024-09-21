#pragma once
#include "Core/Application.h"

#ifdef APP_PLATFORM_WINDOWS

extern Ry_Engine::Application* Ry_Engine::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

namespace Ry_Engine {

	int Main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			Ry_Engine::Application* app = Ry_Engine::CreateApplication(argc, argv);
			app->Run();
			delete app;
		}

		return 0;
	}

}

#ifdef RYAPP_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return Ry_Engine::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Ry_Engine::Main(argc, argv);
}

#endif // RYAPP_DIST

#endif // APP_PLATFORM_WINDOWS
