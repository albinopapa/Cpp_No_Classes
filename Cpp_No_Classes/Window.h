#pragma once

// Must be included first
#include "LeanWindows.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"


namespace Framework::Window
{
	struct _Window
	{
		_Window();
		~_Window();

		WNDCLASSEX wc{};
		HWND hWnd = nullptr;
		RECT rect = { 100,100,100 + Graphics::ScreenWidth,100 + Graphics::ScreenHeight};

		Input::Mouse::_Mouse mouse;
		Input::Keyboard::_Keyboard keyboard;
		MSG msg{};
		bool wants_to_quit = false;
	};

}
