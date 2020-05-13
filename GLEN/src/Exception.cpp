#include "GLEN_PCH.h"
#include "Exception.h"

using namespace GLEN;

Exception::Exception(const int line, const char* file) noexcept
	:
	m_line(line),
	m_file(file)
{}

const char* GLEN::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << "\n"
		<< GetOriginString();
	m_whatBuffer = oss.str();
	GLEN_ERROR(m_whatBuffer);

	return m_whatBuffer.c_str();
}

const char* GLEN::Exception::GetType() const noexcept
{
	return "Default GLEN Exception";
}

int GLEN::Exception::GetLine() const noexcept
{
	return m_line;
}

const std::string& GLEN::Exception::GetFile() const noexcept
{
	return m_file;
}

std::string GLEN::Exception::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[FILE] " << m_file << "\n"
		<< "[LINE] " << m_line;

	return oss.str();
}

WindowException::WindowException(const int line, const char* file, HRESULT hResult)
	:
	Exception(line, file),
	m_hResult(hResult)
{}

const char* WindowException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << "\n"
		<< "[ERROR CODE] " << ::GetErrorMode() << "\n"
		<< "[Description] " << GetErrorString() << "\n"
		<< GetOriginString();

	m_whatBuffer = oss.str();
	GLEN_ERROR(m_whatBuffer);
	return m_whatBuffer.c_str();
}

const char* GLEN::WindowException::GetType() const noexcept
{
	return "GLEN Windows Exception";
}

std::string GLEN::WindowException::TranslateErrorCode(HRESULT hResult) noexcept
{
	char* pMsgBuff {nullptr};
	DWORD nMsgLen {FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuff), 0, nullptr)};

	if (nMsgLen == 0)
	{
		GLEN_ERROR("Unidentified Error Code");
		return "ERROR: Unidentified Error Code";
	}

	std::string errorString = pMsgBuff;
	LocalFree(pMsgBuff);
	return errorString;
}

HRESULT WindowException::GetErrorCode() const noexcept
{
	return m_hResult;
}

std::string WindowException::GetErrorString() const noexcept
{
	return TranslateErrorCode(m_hResult);
}



