#include "Direct3D.h"
#include "Framework.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sprites.h"
#include "Window.h"

#include <cassert>

namespace Framework
{
	// Color methods
	namespace Graphics
	{
		void SetA( _Color &C, uint8_t Value )
		{
			C.value = ( C.value & 0x00FFFFFF ) | ( Value << 24 );
		}
		void SetR( _Color &C, uint8_t Value )
		{
			C.value = ( C.value & 0xFF00FFFF ) | ( Value << 16 );
		}
		void SetG( _Color &C, uint8_t Value )
		{
			C.value = ( C.value & 0xFFFF00FF ) | ( Value << 8 );
		}
		void SetB( _Color &C, uint8_t Value )
		{
			C.value = ( C.value & 0xFFFFFF00 ) | ( Value );
		}
	

		// Sprite methods

		_Sprite Copy( const _Sprite& source )
		{
			const auto buffSize = source.width * source.height;
			Graphics::_Sprite result( source.width, source.height, source.effect );

			const Graphics::_Color* it = source.frame.get();
			const Graphics::_Color* const end = &source.frame[ buffSize ];
			Graphics::_Color* dst_it = result.frame.get();

			for( ; it != end; ++it, ++dst_it )
			{
				*dst_it = *it;
			}

			return result;
		}
		void AdvanceFrame( Graphics::_AnimatedSprite& _this, float dt )
		{
			_this.curHoldTime += dt;
			if( _this.curHoldTime >= _this.holdFrameTime )
			{
				++_this.cur_frame;
				if( _this.cur_frame == _this.frames.end() )
				{
					_this.cur_frame = _this.frames.begin();
				}
			}
		}

		// Graphics methods

		void BeginFrame( _Graphics & gfx, _Color C )
		{
			if( GetR( C ) == GetG( C ) &&
				GetR( C ) == GetB( C ) )
			{
				constexpr auto buffSize =
					ScreenWidth * ScreenHeight * sizeof( Graphics::_Color );

				memset( gfx.pBuffer.get(), GetR( C ), buffSize );
			}
			else
			{
				DrawRect( gfx, 0, 0, ScreenWidth, ScreenHeight, C );
			}
		}
		void EndFrame( _Graphics & gfx )
		{
			Present( gfx.d3d, gfx.pBuffer.get() );
		}
		void DrawRect( _Graphics& gfx, int32_t X, int32_t Y, int32_t Width, int32_t Height, _Color C, bool Filled )
		{
			if( Filled )
			{
				for( int32_t y = Y; y < Y + Height; ++y )
				{
					for( int32_t x = X; x < X + Width; ++x )
					{
						PutPixel( gfx, x, y, C );
					}
				}
			}
			else
			{
				const auto left = X;
				const auto top = Y;
				const auto right = X + Width;
				const auto bottom = Y + Height;

				DrawLine( gfx, left, top, right, top, C ); // Top
				DrawLine( gfx, left, top, left, bottom, C ); // Left
				DrawLine( gfx, left, bottom, right, bottom, C ); // Bottom
				DrawLine( gfx, right, top, right, bottom, C ); // Right
			}
		}
		void DrawCircle( _Graphics & gfx, int32_t X, int32_t Y, int32_t Radius, _Color C, bool Filled )
		{
			const auto rSq = Radius * Radius;
			if( Filled )
			{
				for( int32_t y = -Radius; y < Radius; ++y )
				{
					for( int32_t x = -Radius; x < Radius; ++y )
					{
						const auto distSq = ( x * x ) + ( y*y );
						if( distSq < rSq )
							PutPixel( gfx, x, y, C );
					}
				}
			}
			else
			{
				const int xPivot = ( int )( Radius * 0.707107f + 0.5f );
				for( int x = 0; x <= xPivot; x++ )
				{
					const int y = ( int )( sqrt( ( float )( rSq - x * x ) ) + 0.5f );
					PutPixel( gfx, X + x, Y + y, C );
					PutPixel( gfx, X - x, Y + y, C );
					PutPixel( gfx, X + x, Y - y, C );
					PutPixel( gfx, X - x, Y - y, C );
					PutPixel( gfx, X + y, Y + x, C );
					PutPixel( gfx, X - y, Y + x, C );
					PutPixel( gfx, X + y, Y - x, C );
					PutPixel( gfx, X - y, Y - x, C );
				}
			}
		}
		void DrawLine( _Graphics& gfx, int32_t StartX, int32_t StartY, int32_t EndX, int32_t EndY, _Color C )
		{
			const auto dx = EndX - StartX;
			const auto dy = EndY - StartY;

			if( dy == 0 && dx == 0 )
			{
				PutPixel( gfx, StartX, StartY, C );
			}
			else if( abs( dy ) > abs( dx ) )
			{
				if( dy < 0 )
				{
					std::swap( StartX, EndX );
					std::swap( StartY, EndY );
				}
				const float m = ( float )dx / ( float )dy;
				const float b = StartX - m * StartY;
				for( int y = StartY; y <= EndY; y = y + 1 )
				{
					const int x = ( int )( m*y + b + 0.5f );
					PutPixel( gfx, x, y, C );
				}
			}
			else
			{
				if( dx < 0 )
				{
					std::swap( StartX, EndX );
					std::swap( StartY, EndY );
				}
				const float m = ( float )dy / ( float )dx;
				const float b = StartY - m * StartX;

				for( int x = StartX; x <= EndX; x = x + 1 )
				{
					const int y = ( int )( m*x + b + 0.5f );
					PutPixel( gfx, x, y, C );
				}
			}
		}
		void DrawSprite( _Graphics& gfx, const int32_t X, const int32_t Y, const _Sprite& Sprite )
		{
			for( int32_t dy = Y, sy = 0; dy < Y + Sprite.height; ++dy, ++sy )
			{
				for( int32_t dx = X, sx = 0; dx < X + Sprite.width; ++dx, ++sx )
				{
					const auto src_idx = sx + ( sy * Sprite.width );
					const auto dst_idx = dx + ( dy * Graphics::ScreenWidth );
					auto color =
						Sprite.effect( gfx, Sprite.frame[ src_idx ], gfx.pBuffer[ dst_idx ] );

					PutPixel( gfx, dx, dy, color );
				}
			}
		}
		void PutPixel( _Graphics & gfx, int32_t X, int32_t Y, _Color C )
		{
			assert( X >= 0 );
			assert( Y >= 0 );
			assert( X < ScreenWidth );
			assert( Y < ScreenHeight );

			gfx.pBuffer[ X + ( Y * ScreenWidth ) ] = C;
		}
	}

	// Direct3D methods
	namespace Direct3D
	{
		void Present( const _Direct3D& d3d, const Graphics::_Color* pBuffer )
		{
			using Graphics::_Color;
			HRESULT hr;

			D3D11_MAPPED_SUBRESOURCE mappedSysBufferTexture{};

			// lock and map the adapter memory for copying over the sysbuffer
			if( FAILED( hr = d3d.pImmediateContext->Map( d3d.pSysBufferTexture.Get(), 0u,
				D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture ) ) )
			{
				throw std::runtime_error( "Mapping sysbuffer" );
			}
			// setup parameters for copy operation
			_Color* pDst = reinterpret_cast<_Color*>( mappedSysBufferTexture.pData );
			const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( _Color );
			const size_t srcPitch = Graphics::ScreenWidth;
			const size_t rowBytes = srcPitch * sizeof( _Color );

			// perform the copy line-by-line
			for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
			{
				memcpy( &pDst[ y * dstPitch ], &pBuffer[ y * srcPitch ], rowBytes );
			}
			// release the adapter memory
			d3d.pImmediateContext->Unmap( d3d.pSysBufferTexture.Get(), 0u );

			// render offscreen scene texture to back buffer
			d3d.pImmediateContext->IASetInputLayout( d3d.pInputLayout.Get() );
			d3d.pImmediateContext->VSSetShader( d3d.pVertexShader.Get(), nullptr, 0u );
			d3d.pImmediateContext->PSSetShader( d3d.pPixelShader.Get(), nullptr, 0u );
			d3d.pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			const UINT stride = sizeof( FSQVertex );
			const UINT offset = 0u;
			d3d.pImmediateContext->IASetVertexBuffers( 0u, 1u, d3d.pVertexBuffer.GetAddressOf(), &stride, &offset );
			d3d.pImmediateContext->PSSetShaderResources( 0u, 1u, d3d.pSysBufferTextureView.GetAddressOf() );
			d3d.pImmediateContext->PSSetSamplers( 0u, 1u, d3d.pSamplerState.GetAddressOf() );
			d3d.pImmediateContext->Draw( 6u, 0u );

			// flip back/front buffers
			if( FAILED( hr = d3d.pSwapChain->Present( 1u, 0u ) ) )
			{
				if( hr == DXGI_ERROR_DEVICE_REMOVED )
				{
					throw std::runtime_error( "Presenting back buffer [device removed]" );
				}
				else
				{
					throw std::runtime_error( "Presenting back buffer" );
				}
			}
		}
	}

	
	namespace Input
	{
		// Keyboard methods
		namespace Keyboard
		{
			bool IsPressed( const _Keyboard& keyboard, uint8_t key )
			{
				return keyboard.states[ key ];
			}
			bool HasEvent( const _Keyboard& keyboard )
			{
				return !keyboard.events.empty();
			}
			Event Read( _Keyboard& keyboard )
			{
				if( keyboard.events.empty() )
					return Keyboard::Event();

				const auto ev = keyboard.events.front();
				keyboard.events.pop();

				return ev;
			}
			void OnPress( _Keyboard& keyboard, const uint8_t key )
			{
				keyboard.events.emplace( Keyboard::State::KeyPressed, key );
				if( keyboard.events.size() > 4 )
				{
					keyboard.events.pop();
				}
				keyboard.states[ key ] = true;
			}
			void OnRelease( _Keyboard& keyboard, const uint8_t key )
			{
				keyboard.events.emplace( Keyboard::State::KeyReleased, key );
				keyboard.states[ key ] = false;
			}
		}

		// Mouse methods
		namespace Mouse
		{
			bool LeftIsPressed( const _Mouse& mouse )
			{
				return mouse.leftPressed;
			}
			bool RightIsPressed( const _Mouse& mouse )
			{
				return mouse.rightPressed;
			}
			bool HasEvent( const _Mouse& mouse )
			{
				return !mouse.events.empty();
			}
			Event Read( _Mouse& mouse )
			{
				if( mouse.events.empty() )
					return Mouse::Event();

				const auto ev = mouse.events.front();
				mouse.events.pop();

				return ev;
			}
			Vec2i GetPosition( const _Mouse& mouse )
			{
				return { int32_t( mouse.x ),int32_t( mouse.y ) };
			}
			void OnMove( _Mouse& mouse, int16_t X, int16_t Y )
			{
				mouse.events.emplace( Mouse::State::Idle, X, Y );
			}
			void OnPress( _Mouse& mouse, int16_t X, int16_t Y, uint8_t btncode )
			{
				const auto left = btncode & 1;
				const auto right = btncode & 2;
				const auto middle = btncode & 16;
				const auto xb1 = btncode & 32;
				const auto xb2 = btncode & 64;
				if( left )
				{
					mouse.events.emplace( Mouse::State::LeftPressed, X, Y );
					mouse.leftPressed = true;
				}
				if( middle )
				{
					mouse.events.emplace( Mouse::State::MiddlePressed, X, Y );
				}
				if( right )
				{
					mouse.events.emplace( Mouse::State::RightPressed, X, Y );
					mouse.rightPressed = true;
				}
				if( xb1 )
				{
					mouse.events.emplace( Mouse::State::XButton1Pressed, X, Y );
				}
				if( xb2 )
				{
					mouse.events.emplace( Mouse::State::XButton2Pressed, X, Y );
				}
			}
			void OnRelease( _Mouse& mouse, int16_t X, int16_t Y, uint8_t btncode )
			{
				const auto left = btncode & 1;
				const auto right = btncode & 2;
				const auto middle = btncode & 16;
				const auto xb1 = btncode & 32;
				const auto xb2 = btncode & 64;
				if( left )
				{
					mouse.events.emplace( Mouse::State::LeftReleased, X, Y );
					mouse.leftPressed = false;
				}
				if( middle )
				{
					mouse.events.emplace( Mouse::State::MiddleReleased, X, Y );
				}
				if( right )
				{
					mouse.events.emplace( Mouse::State::RightReleased, X, Y );
					mouse.rightPressed = false;
				}
				if( xb1 )
				{
					mouse.events.emplace( Mouse::State::XButton1Released, X, Y );
				}
				if( xb2 )
				{
					mouse.events.emplace( Mouse::State::XButton2Released, X, Y );
				}
			}
		}
	}

	// Window methods
	namespace Window
	{
		LRESULT CALLBACK MsgProc( HWND winhandle, UINT message, WPARAM wParam, LPARAM lParam )
		{
			if( message == WM_CREATE )
			{
				auto* pStruct =
					reinterpret_cast< LPCREATESTRUCT >( lParam );

				auto* pWindow =
					reinterpret_cast< _Window* >( pStruct->lpCreateParams );

				if( pWindow )
					SetWindowLongPtr( winhandle, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( pWindow ) );

				return 0;
			}

			auto* pWindow =
				reinterpret_cast< _Window* >( GetWindowLongPtr( winhandle, GWLP_USERDATA ) );

			if( pWindow )
			{
				pWindow->msg.hwnd		= winhandle;
				pWindow->msg.message	= message;
				pWindow->msg.wParam		= wParam;
				pWindow->msg.lParam		= lParam;
				return HandleMessages( *pWindow );
			}

			return DefWindowProc( winhandle, message, wParam, lParam );
		}
		void ProcessMessages( _Window & window )
		{
			while( PeekMessage( &window.msg, nullptr, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &window.msg );
				DispatchMessage( &window.msg );
			}

			if( IsPressed( window.keyboard, VK_ESCAPE ) )
			{
				DestroyWindow( window.hWnd );			
			}
		}
		LRESULT HandleMessages( _Window& window )
		{
			switch( window.msg.message )
			{
				case WM_CLOSE:
					DestroyWindow( window.hWnd );
					return 0;
				case WM_DESTROY:
					window.wants_to_quit = true;
					PostQuitMessage( 0 );
					return 0;
				case WM_KEYDOWN:
					OnPress( window.keyboard, static_cast< uint8_t >( window.msg.wParam ) );
					return 0;
				case WM_KEYUP:
					OnRelease( window.keyboard, static_cast< uint8_t >( window.msg.wParam ) );
					return 0;
				case WM_MOUSEMOVE:
				{
					const auto x = static_cast< int16_t >( GET_X_LPARAM( window.msg.lParam ) );
					const auto y = static_cast< int16_t >( GET_Y_LPARAM( window.msg.lParam ) );

					OnMove( window.mouse, x, y );
					return 1;
				}
				case WM_LBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_XBUTTONDOWN:
				{
					const auto x = static_cast< int16_t >( GET_X_LPARAM( window.msg.lParam ) );
					const auto y = static_cast< int16_t >( GET_Y_LPARAM( window.msg.lParam ) );
					const auto btn = static_cast< uint8_t >( window.msg.wParam );

					OnPress( window.mouse, x, y, btn );
					return 0;
				}
				case WM_LBUTTONUP:
				case WM_MBUTTONUP:
				case WM_XBUTTONUP:
				{
					const auto x = static_cast< int16_t >( GET_X_LPARAM( window.msg.lParam ) );
					const auto y = static_cast< int16_t >( GET_Y_LPARAM( window.msg.lParam ) );
					const auto btn = static_cast< uint8_t >( window.msg.wParam );

					OnRelease( window.mouse, x, y, btn );
					return 0;
				}
				default:
					return DefWindowProc(
						window.hWnd, window.msg.message, window.msg.wParam, window.msg.lParam );
			}
		}
		Input::Mouse::_Mouse& GetMouse( _Window& window )
		{
			return window.mouse;
		}
		const Input::Mouse::_Mouse& GetMouse( const _Window& window )
		{
			return window.mouse;
		}

	}
}