#include "GLEN_PCH.h"
#include "Window.h"

namespace GLEN {
	LRESULT WINAPI MessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			// the firstCREATESTRUCT param is the optional param in CreateWindow fxm
			const CREATESTRUCTW* const pCreate { reinterpret_cast<CREATESTRUCTW*>(lParam) };
			Window* const pWnd {static_cast<Window*>(pCreate->lpCreateParams)}; // lpCreateParams = Window.this

			SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

			// now set windowProc to Message Handler (MessageAdapter
			SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MessageAdapter));

			return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
		}

		return DefWindowProcW(hWnd, msg, lParam, wParam);
	}

	LRESULT WINAPI MessageAdapter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* const pWnd { reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)) };
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	Window::Window(HINSTANCE hInst, 
		const std::wstring& windowClassName, 
		const std::wstring& windowTitle, 
		const uint32_t width, 
		const uint32_t height)
		:
		m_hInst(hInst),
		m_windowClassName(windowClassName),
		m_windowTitle(windowTitle),
		m_width(width),
		m_height(height)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01; //mouse page
		rid.usUsage		= 0x02; //mouse usage
		rid.dwFlags		= 0x0;
		rid.hwndTarget	= nullptr;
		if(RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			throw WND_LASTERROR();
		}

		RegisterWindowClass();
		m_hWnd = CreateWindow(width, height);
	}

	Window::~Window()
	{
		UnregisterClassW(m_windowClassName.c_str(), m_hInst);
		DestroyWindow(m_hWnd);
	}


	LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (msg)
		{
		
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		
		case WM_KILLFOCUS:
		{
			//m_Keyboard.ClearState();
			GLEN_INFO("KILL FOCUS");

			if (!m_cursorEnabled)
			{
				if (wParam & WA_ACTIVE)
				{
					ConfineCursor();
					HideCursor();
				}
				else
				{
					FreeCursor();
					ShowCursor();
				}
				break;
			}
		}
		
		case WM_SIZE:
		{
			RECT clientRect = {};
			::GetClientRect(hWnd, &clientRect);

			const int width{ clientRect.right - clientRect.left };
			const int height{ clientRect.bottom - clientRect.top };

#if defined(_DEBUG)
			spdlog::debug("Resized Window To: {} x {}", width, height);
#endif
		}
		break;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////// END WINDOW INTERACTIVE MESSAGES //////////////////////////////////////

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			if (wParam == VK_F11)
			{
				SetFullscreen(!m_fullscreen);
				break;
			}
			//if (!(lParam & 0x40000000) || m_Keyboard.autoRepeatFlag)
			//{
			//	m_Keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
			//	GLEN_CRITICAL(static_cast<unsigned char>(wParam));
			//}
			break;
		}

		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			//m_Keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		}

		case WM_CHAR:
		{
			//m_Keyboard.OnChar(static_cast<unsigned char>(wParam));
			break;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////// END KEYBOARD MESSAGES ///////////////////////////////////////////

		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			if (!m_cursorEnabled)
			{
				//if (!m_Mouse.isInWindow)
				//{
				//	SetCapture(m_hWnd);
				//	m_Mouse.OnMouseEnter();
				//	HideCursor();
				//}
				break;
			}

			if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height) // are we inside the window?
			{
				//m_Mouse.OnMouseMove(pt.x, pt.y);
				//if (!m_Mouse.isInWindow)
				//{
				//	SetCapture(m_hWnd);
				//	m_Mouse.OnMouseEnter();
				//}
			}
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					//m_Mouse.OnMouseMove(pt.x, pt.y);
				}
				else
				{
					ReleaseCapture();
					//m_Mouse.OnMouseLeave();
				}
			}

			break;
		}

		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);

			SetForegroundWindow(m_hWnd);
			//if (!m_Mouse.isInWindow)
			//{
			//	ConfineCursor();
			//	HideCursor();
			//}

			//m_Mouse.OnLeftPressed(pt.x, pt.y);
			GLEN_INFO("LEFT AT: {}, {}", pt.x, pt.y);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);

			SetForegroundWindow(m_hWnd);
			//if (!m_Mouse.isInWindow)
			//{
			//	ConfineCursor();
			//	HideCursor();
			//}

			//m_Mouse.OnRightPressed(pt.x, pt.y);
			GLEN_INFO("RIGHT AT: {}, {}", pt.x, pt.y);
			break;
		}

		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			//m_Mouse.OnLeftReleased(pt.x, pt.y);

			if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height) // is it outside window?
			{
				ReleaseCapture();
				//m_Mouse.OnMouseLeave();
			}
			break;
		}

		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			//m_Mouse.OnRightReleased(pt.x, pt.y);

			if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height) // is it outside window?
			{
				ReleaseCapture();
				//m_Mouse.OnMouseLeave();
			}
			break;
		}

		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);

			//m_Mouse.OnWheelDelta(pt.x, pt.y, delta);

			break;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////// END STANDARD MOUSE MESSAGES /////////////////////////////////////

		case WM_INPUT:
		{
			//if (!m_Mouse.rawEnabled)
			//{
			//	break;
			//}

			UINT size;
			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				nullptr,
				&size,
				sizeof(RAWINPUTHEADER)) == -1)
			{
				//DJINN_WND_LASTERROR();
				break;
			}

			rawBuffer.resize(size);
			// read in the input data
			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				rawBuffer.data(),
				&size,
				sizeof(RAWINPUTHEADER)) != size)
			{
				//DJINN_WND_LASTERROR();
				break;
			}

			auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
			if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
			{
				//m_Mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
			}

			break;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////// END RAW MOUSE MESSAGES //////////////////////////////////////////
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}


	void Window::RegisterWindowClass()
	{
		// register the window class so we can actually render inside it
		WNDCLASSEX wcex { 0 }; 
		
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_OWNDC;
		wcex.lpfnWndProc = MessageSetup;
		wcex.cbClsExtra = 0;
		wcex.cbClsExtra = 0;
		wcex.hInstance = m_hInst; // we provide this instance pointer so the function does not need to return anything
		wcex.hIcon = nullptr;
		wcex.hCursor = nullptr;
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = m_windowClassName.c_str();
		wcex.hIconSm = nullptr;

		// create window instance
		static ATOM atom = RegisterClassEx(&wcex);
		assert(atom > 0);
	}

	HWND Window::CreateWindow(const int width, const int height)
	{
		// create a new window rectangle and adjust it to the full window size
		m_nonFullscreenRect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
		ThrowIfFailed(AdjustWindowRect(&m_nonFullscreenRect, WS_OVERLAPPEDWINDOW, FALSE));

		// get TRUE window sizes
		// origin is TOP LEFT corner
		m_width = m_nonFullscreenRect.right - m_nonFullscreenRect.left;
		m_height = m_nonFullscreenRect.bottom - m_nonFullscreenRect.top;

		// center the window on the screen and clamp ot 0,0
		const int windowX = std::max<int>(0, (m_screenWidth - m_width) / 2);
		const int windowY = std::max<int>(0, (m_screenHeight - m_height) / 2);

		HWND hWnd = CreateWindowExW(
			WS_EX_OVERLAPPEDWINDOW,
			m_windowClassName.c_str(),
			m_windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			m_width,
			m_height,
			nullptr,
			nullptr,
			m_hInst,
			this
		);

		if (hWnd == nullptr)
		{
			throw WND_LASTERROR();
		}

		ShowWindow(hWnd, SW_SHOW);

		return hWnd;
	}


	void Window::ConfineCursor() noexcept
	{
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		MapWindowPoints(m_hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
		ClipCursor(&rect);
	}

	void Window::FreeCursor() noexcept
	{
		ClipCursor(nullptr);
	}

	void Window::HideCursor() noexcept
	{
		ClipCursor(nullptr);
		while(::ShowCursor(FALSE) >= 0);
	}

	void Window::ShowCursor() noexcept
	{
		while(::ShowCursor(TRUE) < 0);
	}

	void Window::SetFullscreen(bool fullscreen) noexcept
	{
		if (m_fullscreen != fullscreen)
		{
			m_fullscreen = fullscreen;
			if (m_fullscreen) // switch to full screen
			{
				// store the current window dimensions to restore to
				::GetWindowRect(m_hWnd, &m_nonFullscreenRect);

				static const UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
				::SetWindowLong(m_hWnd, GWL_STYLE, windowStyle);

				//Query for the nearest display device
				// need to pick one in multi-monitor setup
				HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFOEX monitorInfo = {};
				monitorInfo.cbSize = sizeof(MONITORINFOEX);
				::GetMonitorInfo(hMonitor, &monitorInfo);

				::SetWindowPos(m_hWnd, HWND_TOP,
					monitorInfo.rcMonitor.left,
					monitorInfo.rcMonitor.top,
					monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
					monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
					SWP_FRAMECHANGED | SWP_NOACTIVATE);

				::ShowWindow(m_hWnd, SW_MAXIMIZE);
			}

			else
			{
				// Restore all the window decorations to what they should be in non-fullscreen
				::SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

				::SetWindowPos(m_hWnd, HWND_NOTOPMOST,
					m_nonFullscreenRect.left,
					m_nonFullscreenRect.top,
					m_nonFullscreenRect.right - m_nonFullscreenRect.left,
					m_nonFullscreenRect.bottom - m_nonFullscreenRect.top,
					SWP_FRAMECHANGED | SWP_NOACTIVATE);

				::ShowWindow(m_hWnd, SW_NORMAL);
			}
		}
	}
}