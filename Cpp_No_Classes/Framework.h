#pragma once

#include "Color.h"
#include "LeanWindows.h"
#include "MathTypes.h"
#include <cstdint>

namespace Framework
{
	namespace Direct3D
	{
		struct _Direct3D;

		void Present( const _Direct3D& d3d, const Graphics::_Color* pBuffer );
	}
	namespace Graphics
	{
		struct _Graphics;
		struct _Sprite;
		struct _AnimatedSprite;

		// Color methods
		constexpr uint8_t GetA( _Color C )
		{
			return ( C.value >> 24 ) & 0xFF;
		}
		constexpr uint8_t GetR( _Color C )
		{
			return ( C.value >> 16 ) & 0xFF;
		}
		constexpr uint8_t GetG( _Color C )
		{
			return ( C.value >> 8 ) & 0xFF;
		}
		constexpr uint8_t GetB( _Color C )
		{
			return ( C.value & 0xFF );
		}

		void SetA( _Color &C, uint8_t Value );
		void SetR( _Color &C, uint8_t Value );
		void SetG( _Color &C, uint8_t Value );
		void SetB( _Color &C, uint8_t Value );

		// Sprite methods

		_Sprite Copy( const _Sprite& source );
		void AdvanceFrame( _AnimatedSprite& _this, float dt );

		// Graphics methods

		void BeginFrame( _Graphics& gfx, _Color C );
		void EndFrame( _Graphics& gfx );
		void DrawRect( _Graphics& gfx, int32_t X, int32_t Y, int32_t Width, int32_t Height, _Color C, bool Filled = true );
		void DrawCircle( _Graphics& gfx, int32_t X, int32_t Y, int32_t Radius, _Color C, bool Filled = true );
		void DrawLine( _Graphics& gfx, int32_t StartX, int32_t StartY, int32_t EndX, int32_t EndY, _Color C );
		void DrawSprite( _Graphics& gfx, const int32_t X, const int32_t Y, const _Sprite& Sprite );
		void PutPixel( _Graphics& gfx, int32_t X, int32_t Y, _Color C );

	}
	namespace Input
	{
		namespace Keyboard
		{
			struct Event;
			struct _Keyboard;

			bool IsPressed( const _Keyboard& keyboard, uint8_t key );
			bool HasEvent( const _Keyboard& keyboard );
			Event Read( _Keyboard& keyboard );
			void OnPress( _Keyboard& keyboard, const uint8_t key );
			void OnRelease( _Keyboard& keyboard, const uint8_t key );
		}
		namespace Mouse
		{
			struct Event;
			struct _Mouse;

			bool LeftIsPressed( const _Mouse& mouse );
			bool RightIsPressed( const _Mouse& mouse );

			bool HasEvent( const _Mouse& mouse );
			Event Read( _Mouse& mouse );

			Vec2i GetPosition( const _Mouse& mouse );
			void OnMove( _Mouse& mouse, int16_t X, int16_t Y );
			void OnPress( _Mouse& mouse, int16_t X, int16_t Y, uint8_t btncode );
			void OnRelease( _Mouse& mouse, int16_t X, int16_t Y, uint8_t btncode );
		}
	}
	namespace Window
	{
		struct _Window;

		LRESULT CALLBACK MsgProc( HWND WinHandle, UINT Message, WPARAM wParam, LPARAM lParam );
		void ProcessMessages( _Window& window );
		LRESULT HandleMessages( _Window& window );
		Input::Mouse::_Mouse& GetMouse( _Window& window );
		const Input::Mouse::_Mouse& GetMouse( const _Window& window );
	}
}

using EffectFn = Framework::Graphics::_Color( *)(
	Framework::Graphics::_Graphics& gfx,
	const Framework::Graphics::_Color src1,
	const Framework::Graphics::_Color src2 );
