#include "Window.h"
#include "Framework.h"

namespace Window
{
	_Window::_Window()
	{
		wc.cbSize = sizeof( wc );
		wc.lpfnWndProc = Framework::MsgProc;
		wc.hInstance = GetModuleHandle( nullptr );
		wc.lpszClassName = TEXT( "MyClass" );
		wc = wc;

		if( RegisterClassEx( &wc ) == 0 )
			throw std::runtime_error( "Failed to register the Window class" );

		constexpr uint32_t style = WS_OVERLAPPEDWINDOW;
		constexpr uint32_t exStyle = WS_EX_OVERLAPPEDWINDOW;

		
		AdjustWindowRectEx( &rect, style, FALSE, exStyle );

		{
			const auto x = rect.left;
			const auto y = rect.top;
			const auto width = rect.right - rect.left;
			const auto height = rect.bottom - rect.top;

			hWnd = CreateWindowEx(
				exStyle, wc.lpszClassName, TEXT( "CppNoClass" ), style,
				x, y, width, height, nullptr, nullptr, wc.hInstance, this
			);
		}

		ShowWindow( hWnd, SW_SHOWDEFAULT );
	}
	_Window::~_Window()
	{
		UnregisterClass( wc.lpszClassName, wc.hInstance );
	}
}

