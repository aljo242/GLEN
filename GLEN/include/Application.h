#pragma once

#include "Window.h"
#include <chrono>

namespace GLEN {
	class App
	{
	public:
		App(HINSTANCE hInstance);
		int Go(); // frame-message loop

	private:
		void DoFrame();
		Window m_wnd;
		std::chrono::high_resolution_clock::time_point m_past_tick;
	};

}