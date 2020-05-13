#include "GLEN_PCH.h"
#include "Application.h"

using namespace GLEN;

App::App(HINSTANCE hInstance)
	:
	m_wnd(hInstance, L"WINDOW", L"WINDOW TITLE", 1280, 720)
{}

int App::Go()
{
	// message pump
	MSG msg;
	BOOL gResult; // return type for windows message calls
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		DoFrame();
	}

	if (gResult == -1)
	{
		return -1;
	}

	return static_cast<int>(msg.wParam); // exit code given when PostQuitMessage called 
}

void App::DoFrame()
{
	using namespace std::chrono;
	auto start = high_resolution_clock::now();
}