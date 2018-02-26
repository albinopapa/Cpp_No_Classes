#include "Graphics.h"
#include <cassert>
#include "MathTypes.h"

namespace Graphics
{
	_Graphics::_Graphics( Window::_Window& window )
		:
		d3d( window ),
		pBuffer( std::make_unique<Color::_Color[]>( ScreenWidth*ScreenHeight ) )
	{}

	void BeginFrame( _Graphics & gfx, Color::_Color C )
	{
		Math::Vec2::_Vec2<float> v,u;
		const auto res = v + u;

		if( Color::GetR( C ) == Color::GetG( C ) && 
			Color::GetR( C ) == Color::GetB( C ) )
		{
			constexpr auto buffSize = 
				ScreenWidth * ScreenHeight * sizeof( Color::_Color );

			memset( gfx.pBuffer.get(), Color::GetR( C ), buffSize );
		}
		else
		{
			Graphics::DrawRect( gfx, 0, 0, ScreenWidth, ScreenHeight, C );
		}
	}

	void EndFrame( _Graphics & gfx )
	{
		Direct3D::Present( gfx.d3d, gfx.pBuffer.get() );
	}

	void DrawRect( _Graphics& gfx, int32_t X, int32_t Y, int32_t Width, int32_t Height, Color::_Color C, bool Filled )
	{
		if(Filled)
		{
			for( int32_t y = Y; y < Y + Height; ++y )
			{
				for( int32_t x = X; x < X + Width; ++x )
				{
					Graphics::PutPixel( gfx, x, y, C );
				}
			}
		}
		else
		{
			const auto left = X;
			const auto top = Y;
			const auto right = X + Width;
			const auto bottom = Y + Height;

			DrawLine( gfx, left,  top,    right, top,    C ); // Top
			DrawLine( gfx, left,  top,    left,  bottom, C ); // Left
			DrawLine( gfx, left,  bottom, right, bottom, C ); // Bottom
			DrawLine( gfx, right, top,    right, bottom, C ); // Right
		}
	}

	void DrawCircle( _Graphics & gfx, int32_t X, int32_t Y, int32_t Radius, Color::_Color C, bool Filled )
	{
		const auto rSq = Radius * Radius;
		if(Filled)
		{
			for( int32_t y = - Radius; y < Radius; ++y )
			{
				for( int32_t x = -Radius; x < Radius; ++y )
				{
					const auto distSq = ( x * x ) + ( y*y );
					if( distSq < rSq )
						Graphics::PutPixel( gfx, x, y, C );
				}
			}
		}
		else
		{
			const int xPivot = ( int )( Radius * 0.707107f + 0.5f );
			for( int x = 0; x <= xPivot; x++ )
			{
				const int y = ( int )( sqrt( ( float )( rSq - x * x ) ) + 0.5f );
				Graphics::PutPixel( gfx, X + x, Y + y, C );
				Graphics::PutPixel( gfx, X - x, Y + y, C );
				Graphics::PutPixel( gfx, X + x, Y - y, C );
				Graphics::PutPixel( gfx, X - x, Y - y, C );
				Graphics::PutPixel( gfx, X + y, Y + x, C );
				Graphics::PutPixel( gfx, X - y, Y + x, C );
				Graphics::PutPixel( gfx, X + y, Y - x, C );
				Graphics::PutPixel( gfx, X - y, Y - x, C );
			}
		}
	}

	void DrawLine( _Graphics& gfx, int32_t StartX, int32_t StartY, int32_t EndX, int32_t EndY, Color::_Color C )
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

	void PutPixel( _Graphics & gfx, int32_t X, int32_t Y, Color::_Color C )
	{
		assert( X >= 0 );
		assert( Y >= 0 );
		assert( X < ScreenWidth );
		assert( Y < ScreenHeight );

		gfx.pBuffer[ X + ( Y * ScreenWidth ) ] = C;
	}
}
