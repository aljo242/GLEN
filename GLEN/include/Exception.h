#pragma once

#include <Windows.h>
#include <exception>
#include <string>

#include "Log.h"

namespace GLEN 
{
	class Exception : public std::exception
	{
	public:
		Exception(const int line, const char* file) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;

	private:
		int m_line;
		std::string m_file;

	protected:
		mutable std::string m_whatBuffer;
	};

	class WindowException : public Exception
	{
	public:
		WindowException(const int line, const char* file, HRESULT hResult);
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hResult) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT m_hResult;
	};
}

#define WND_EXCEPTION(hResult) GLEN::WindowException(__LINE__, __FILE__, hResult)
#define WND_LASTERROR() GLEN::WindowException(__LINE__, __FILE__, ::GetLastError())
#define EXCEPTION() GLEN::Exception(__LINE__, __FILE__)