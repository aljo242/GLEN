#pragma once
#include "GLEN_PCH.h"
#include "Exception.h"


namespace GLEN {

	LRESULT CALLBACK MessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MessageAdapter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	class Window
	{
	public:
		Window(HINSTANCE hInst,
			const std::wstring& windowClassName,
			const std::wstring& windowTitle,
			const uint32_t width,
			const uint32_t height);
		~Window();

		HWND Get_hWnd() { return m_hWnd; }
		LRESULT HandleMessage(HWND hWNd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	private:
		HWND m_hWnd;
		RECT m_nonFullscreenRect;

		//Keyboard m_Keyboard;
		//Mouse m_Mouse;
		int m_width;
		int m_height;;
		int m_screenWidth;
		int m_screenHeight;
		bool m_cursorEnabled;
		bool m_fullscreen;
		std::vector<BYTE> rawBuffer;

		HINSTANCE m_hInst;
		std::wstring m_windowClassName;
		std::wstring m_windowTitle;

		void RegisterWindowClass();
		HWND CreateWindow(const int width, const int height);

		void ConfineCursor() noexcept;
		void FreeCursor() noexcept;
		void HideCursor() noexcept;
		void ShowCursor() noexcept;
		void SetFullscreen(const bool fullscreen) noexcept;
	};


	inline void ThrowIfFailedLast(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw WND_LASTERROR();
		}
	}

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw WND_EXCEPTION(hr);
		}
	}
}

