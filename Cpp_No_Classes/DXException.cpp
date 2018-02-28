#include "DXException.h"
#include "DXErr.h"
#include <array>
#include "Logger.h"

DXException::DXException( HRESULT hr, std::wstring note, const wchar_t * file, unsigned int line )
	:
	ChiliException( file, line, std::move( note ) ),
	hr( hr )
{
	TraceLog::Instance().Flush();
}

std::wstring DXException::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring DXException::GetErrorDescription() const
{
	std::array<wchar_t, 512> wideDescription;
	DXGetErrorDescription( hr, wideDescription.data(), wideDescription.size() );
	return wideDescription.data();
}

std::wstring DXException::GetFullMessage() const
{
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring note		 = GetNote();
	const std::wstring location  = GetLocation();

	return
		std::wstring( L"Error: " )		 + errorName + L"\n" +
		std::wstring( L"Description: " ) + errorDesc + L"\n" +
		std::wstring( L"Note: " )		 + note		 + L"\n" +
		std::wstring( L"Location: " )	 + location;


	/*const std::wstring empty = L"";
	return    ( !errorName.empty() ?
		std::wstring( L"Error: " ) + errorName + L"\n" : empty ) +
		( !errorDesc.empty() ?
			std::wstring( L"Description: " ) + errorDesc + L"\n" : empty ) +
			( !note.empty() ? std::wstring( L"Note: " ) + note + L"\n" : empty ) +
		( !location.empty() ? std::wstring( L"Location: " ) + location : empty );*/
}

std::wstring DXException::GetExceptionType() const
{
	return L"DXException";
}
