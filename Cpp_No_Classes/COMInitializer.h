#pragma once

#include "LeanWindows.h"

class COMInitializer
{
public:
	COMInitializer();
	~COMInitializer();
private:
	HRESULT hr;
};