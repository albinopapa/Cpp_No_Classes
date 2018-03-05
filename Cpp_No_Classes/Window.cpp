#include "Window.h"
#include "Framework.h"

namespace Framework::Window
{
	_Window::_Window()
	{
		wc.cbSize = sizeof( wc );
		wc.lpfnWndProc = Framework::Window::MsgProc;
		wc.hInstance = GetModuleHandle( nullptr );
		wc.lpszClassName = TEXT( "MyClass" );
		wc = wc;

		if( RegisterClassEx( &wc ) == 0 )
			throw std::runtime_error( "Failed to register the Window class" );

		constexpr uint32_t style	= WS_POPUP;
		constexpr uint32_t exStyle	= 0;
				
		{
			RECT dtrect{};
			auto dtHandle = GetDesktopWindow();
			GetWindowRect( dtHandle, &dtrect );

			const auto dtWidth = dtrect.right - dtrect.left;
			const auto dtHeight = dtrect.bottom - dtrect.top;

			rect.left = ( dtWidth - Graphics::ScreenWidth ) / 2;
			rect.top = ( dtHeight - Graphics::ScreenHeight ) / 2;
			rect.right = Graphics::ScreenWidth + rect.left;
			rect.bottom = Graphics::ScreenHeight + rect.top;

			AdjustWindowRectEx( &rect, style, FALSE, exStyle );

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

