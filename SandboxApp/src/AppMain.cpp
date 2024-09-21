#include "Ry_Engine/Core/Application.h"
#include "Ry_Engine/EntryMain.h"


Ry_Engine::Application* Ry_Engine::CreateApplication(int argc, char** argv)
{
	Ry_Engine::Application* app = new Ry_Engine::Application();

	return app;
}