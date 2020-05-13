#include "GLEN_PCH.h"
#include "WinMain.h"
#include "Application.h"

using namespace GLEN;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	#if defined(_DEBUG)
	auto logger = Log();
#endif
	GLEN_WARN("Easy padding in numbers like {:08d}", 12);

	try 
	{
		return App{hInstance}.Go();


	}

	catch (const GLEN::Exception& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) // everything else
	{
		MessageBoxA(nullptr, "No Details Available...", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}