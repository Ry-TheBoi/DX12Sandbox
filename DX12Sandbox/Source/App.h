#pragma once

#include "Window/Window.h"
#include "Renderer/Renderer.h"

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
	private:
		Renderer m_Renderer;
	};
}