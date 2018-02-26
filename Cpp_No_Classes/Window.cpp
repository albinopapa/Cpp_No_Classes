#include "Window.h"
#include <windowsx.h>

namespace Window
{
	_Window::_Window()
	{
		wc.cbSize = sizeof( wc );
		wc.lpfnWndProc = MsgProc;
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

	void ProcessMessages( _Window & window )
	{		
		while( PeekMessage( &window.msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &window.msg );
			DispatchMessage( &window.msg );
		}
	}

	LRESULT HandleMessages( _Window& window )
	{
		switch( window.msg.message )
		{
			case WM_CLOSE:
				DestroyWindow( window.hWnd );
				window.wants_to_quit = true;
				return 0;
			case WM_DESTROY:
				PostQuitMessage( 0 );
				return 0;
			case WM_KEYDOWN:
				Keyboard::OnPress( window.keyboard, static_cast< uint8_t >( window.msg.wParam ) );
				return 0;
			case WM_KEYUP:
				Keyboard::OnRelease( window.keyboard, static_cast< uint8_t >( window.msg.wParam ) );
				return 0;
			case WM_MOUSEMOVE:
			{
				const auto x = static_cast< int16_t >( GET_X_LPARAM( window.msg.lParam ) );
				const auto y = static_cast< int16_t >( GET_Y_LPARAM( window.msg.lParam ) );
				
				Mouse::OnMove( window.mouse, x, y );
				return 1;
			}
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_XBUTTONDOWN:
			{
				const auto x = static_cast< int16_t >( GET_X_LPARAM( window.msg.lParam ) );
				const auto y = static_cast< int16_t >( GET_Y_LPARAM( window.msg.lParam ) );
				const auto btn = static_cast< uint8_t >( window.msg.wParam );
				
				Mouse::OnPress( window.mouse, x, y, btn );
				return 0;
			}
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP:
			{
				const auto x = static_cast< int16_t >( GET_X_LPARAM( window.msg.lParam ) );
				const auto y = static_cast< int16_t >( GET_Y_LPARAM( window.msg.lParam ) );
				const auto btn = static_cast< uint8_t >( window.msg.wParam );
				
				Mouse::OnRelease( window.mouse, x, y, btn );
				return 0;
			}
			default:
				return DefWindowProc( 
					window.hWnd, window.msg.message, window.msg.wParam, window.msg.lParam );
		}
	}
}

LRESULT CALLBACK MsgProc( HWND winhandle, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_CREATE )
	{
		auto* pStruct =
			reinterpret_cast< LPCREATESTRUCT >( lParam );

		auto* pWindow = 
			reinterpret_cast< Window::_Window* >( pStruct->lpCreateParams );

		if( pWindow )
			SetWindowLongPtr( winhandle, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( pWindow ) );

		return 0;
	}

	auto* pWindow =
		reinterpret_cast< Window::_Window* >( GetWindowLongPtr( winhandle, GWLP_USERDATA ) );

	if( pWindow )
		return Window::HandleMessages( *pWindow );

	return DefWindowProc( winhandle, message, wParam, lParam );
}
