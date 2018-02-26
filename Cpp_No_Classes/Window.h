#pragma once

// Must be included first
#include "LeanWindows.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"

LRESULT CALLBACK MsgProc( HWND WinHandle, UINT Message, WPARAM wParam, LPARAM lParam );

namespace Window
{
	struct _Window
	{
		_Window();
		~_Window();

		WNDCLASSEX wc{};
		HWND hWnd = nullptr;
		RECT rect = { 100,100,100 + Graphics::ScreenWidth,100 + Graphics::ScreenHeight};

		Mouse::_Mouse mouse;
		Keyboard::_Keyboard keyboard;
		MSG msg{};
		bool wants_to_quit = false;
	};

	void ProcessMessages( _Window& window );
	LRESULT HandleMessages( _Window& window );
}
