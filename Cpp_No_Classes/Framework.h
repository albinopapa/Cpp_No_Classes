#pragma once

#include "Color.h"
#include <cstdint>

namespace Direct3D
{
	struct _Direct3D;
}
namespace Graphics
{
	struct _Graphics;
	struct _Sprite;
}
namespace Keyboard
{
	struct Event;
	struct _Keyboard;
}
namespace Mouse
{
	struct Event;
	struct _Mouse;
}
namespace Widnow
{
	struct _Window;
}

using EffectFn = Color::_Color( *)( 
	Graphics::_Graphics& gfx, 
	int32_t X, int32_t Y, 
	const Graphics::_Sprite& sprite,
	const Color::_Color src2 );


namespace Framework
{
	// Color methods

	constexpr uint8_t GetA( Color::_Color C )
	{
		return ( C.value >> 24 ) & 0xFF;
	}
	constexpr uint8_t GetR( Color::_Color C )
	{
		return ( C.value >> 16 ) & 0xFF;
	}
	constexpr uint8_t GetG( Color::_Color C )
	{
		return ( C.value >> 8 ) & 0xFF;
	}
	constexpr uint8_t GetB( Color::_Color C )
	{
		return ( C.value & 0xFF );
	}
	void SetA( Color::_Color &C, uint8_t Value );
	void SetR( Color::_Color &C, uint8_t Value );
	void SetG( Color::_Color &C, uint8_t Value );
	void SetB( Color::_Color &C, uint8_t Value );

	// Sprite methods

	Graphics::_Sprite Copy( const Graphics::_Sprite& source );

	// Graphics methods

	void BeginFrame( Graphics::_Graphics& gfx, Color::_Color C );
	void EndFrame( Graphics::_Graphics& gfx );
	void DrawRect( Graphics::_Graphics& gfx, int32_t X, int32_t Y, int32_t Width, int32_t Height, Color::_Color C, bool Filled = true );
	void DrawCircle( Graphics::_Graphics& gfx, int32_t X, int32_t Y, int32_t Radius, Color::_Color C, bool Filled = true );
	void DrawLine( Graphics::_Graphics& gfx, int32_t StartX, int32_t StartY, int32_t EndX, int32_t EndY, Color::_Color C );
	void DrawSprite(
		Graphics::_Graphics& gfx,
		const int32_t X,
		const int32_t Y,
		const Graphics::_Sprite& Sprite );
	void PutPixel( Graphics::_Graphics& gfx, int32_t X, int32_t Y, Color::_Color C );

	// Direct3D methods

	void Present( const Direct3D::_Direct3D& d3d, const Color::_Color* pBuffer );

	// Keyboard methods

	bool IsPressed( const Keyboard::_Keyboard& keyboard, uint8_t key );
	bool HasEvent( const Keyboard::_Keyboard& keyboard );

	Keyboard::Event Read( Keyboard::_Keyboard& keyboard );

	void OnPress( Keyboard::_Keyboard& keyboard, const uint8_t key );
	void OnRelease( Keyboard::_Keyboard& keyboard, const uint8_t key );

	// Mouse methods

	bool LeftIsPressed( const Mouse::_Mouse& mouse );
	bool RightIsPressed( const Mouse::_Mouse& mouse );

	bool HasEvent( const Mouse::_Mouse& mouse );
	Mouse::Event Read( Mouse::_Mouse& mouse );

	void OnMove( Mouse::_Mouse& mouse, int16_t X, int16_t Y );
	void OnPress( Mouse::_Mouse& mouse, int16_t X, int16_t Y, uint8_t btncode );
	void OnRelease( Mouse::_Mouse& mouse, int16_t X, int16_t Y, uint8_t btncode );

	// Window methods

	LRESULT CALLBACK MsgProc( HWND WinHandle, UINT Message, WPARAM wParam, LPARAM lParam );
	void ProcessMessages( Window::_Window& window );
	LRESULT HandleMessages( Window::_Window& window );

}