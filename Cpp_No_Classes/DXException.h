#pragma once

#include "LeanWindows.h"
#include "ChiliException.h"

class DXException :public ChiliException
{
public:
	DXException( HRESULT hr, std::wstring note, const wchar_t* file, unsigned int line );

	std::wstring GetErrorName() const;
	std::wstring GetErrorDescription() const;
	std::wstring GetFullMessage() const override;
	std::wstring GetExceptionType() const override;
private:
	HRESULT hr;
};

