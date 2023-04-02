#include "iostream"
#include "App.h"

int main()
{
    std::cout << "Hello Graphics" << std::endl;

	Ry_Engine::AppWindow app;

	if (app.init())
	{
		while (app.isRunning())
		{
			app.broadcast();
		}
	}

    return 0;
}
