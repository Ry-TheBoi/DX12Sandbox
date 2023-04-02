#pragma once

//Ryan TODO: Remind me to create a premake structure for this
#include "Window/Window.h"

namespace Ry_Engine
{
	class AppWindow : public Window
	{
	public:
		AppWindow();
		~AppWindow();

		// Inherited via Window
		virtual void onCreate() override;
		virtual void onUpdate() override;
		virtual void onDestroy() override;
	};
}