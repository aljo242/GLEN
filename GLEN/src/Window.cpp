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
		/*
		case WM_SIZE:
		{
			RECT clientRect = {};
			::GetClientRect(hWnd, &clientRect);

			const int width{ clientRect.right - clientRect.left };
			const int height{ clientRect.bottom - clientRect.top };

			Resize(width, height);
#if defined(_DEBUG)
			spdlog::debug("Resized Window To: {} x {}", width, height);
#endif
		}
		break;
		*/
		////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////// END WINDOW INTERACTIVE MESSAGES //////////////////////////////////////

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			if (wParam == VK_F11)
			{
			//	SetFullscreen(!m_fullScreen);
				break;
			}
			//if (!(lParam & 0x40000000) || m_Keyboard.autoRepeatFlag)
			//{
			//	//m_Keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
			//	GLEN_CRITICAL(static_cast<unsigned char>(wParam));
			//}
			break;
		}

		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			//Keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		}

		case WM_CHAR:
		{
			//Keyboard.OnChar(static_cast<unsigned char>(wParam));
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
				//
				break;
			}

			if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height) // are we inside the window?
			{
				//m_Mouse.OnMouseMove(pt.x, pt.y);
				//if (!m_Mouse.isInWindow)
				{
					SetCapture(m_hWnd);
				////	m_Mouse.OnMouseEnter();
				}
			}
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
				//	m_Mouse.OnMouseMove(pt.x, pt.y);
				}
				else
				{
					ReleaseCapture();
				//	m_Mouse.OnMouseLeave();
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



}