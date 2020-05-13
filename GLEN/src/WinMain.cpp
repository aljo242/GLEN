#include "GLEN_PCH.h"
#include "WinMain.h"

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

	return 0;
}