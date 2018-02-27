#pragma once

#include "Color.h"
#include "Framework.h"
#include "Graphics.h"
#include "string_util.h"
#include <memory>
#include <vector>

namespace Graphics
{
	struct _Sprite
	{
		_Sprite() = default;
		_Sprite( const size_t Width, const size_t Height, const EffectFn& Effect )
			:
			width( Width ), height( Height ),
			frame( std::make_unique<Color::_Color[]>( Width * Height ) ),
			effect( Effect )
		{}
		_Sprite( _Sprite&& other )
		{
			*this = std::move( other );
		}

		_Sprite& operator=( _Sprite&& other )
		{
			if( this != &other )
			{
				width = other.width;
				height = other.height;
				frame = std::move( other.frame );

				other.width = 0;
				other.height = 0;
			}
			return *this;
		}

		size_t width = 0, height = 0;
		std::unique_ptr<Color::_Color[]> frame;
		EffectFn effect = nullptr;
	};

	

	struct _AnimatedSprite
	{
		_AnimatedSprite() = default;
		_AnimatedSprite( String::string &filename, float HoldFrameTime );

		std::vector<_Sprite> frames;
		std::vector<_Sprite>::iterator cur_frame;
		float holdFrameTime = 0.f, curHoldTime = 0.f;
	};
}

auto PassThrough = [](
	Graphics::_Graphics& gfx,
	int32_t X, int32_t Y,
	const Graphics::_Sprite& sprite,
	const Color::_Color src2 )->Color::_Color
{
	const auto idx = X + ( Y * sprite.width );
	Framework::PutPixel( gfx, X, Y, sprite.frame[idx] );
};

auto AlphaEffect = [](
	Graphics::_Graphics& gfx,
	int32_t X, int32_t Y,
	const Graphics::_Sprite& sprite,
	const Color::_Color src2 )->Color::_Color
{
	const auto idx = X + ( Y * sprite.width );
	const auto src1a = Framework::GetA( sprite.frame[ idx ] );
	const auto src1r = Framework::GetR( sprite.frame[ idx ] ) * src1a;
	const auto src1g = Framework::GetG( sprite.frame[ idx ] ) * src1a;
	const auto src1b = Framework::GetB( sprite.frame[ idx ] ) * src1a;

	const auto src2a = 255ui8 - src1a;
	const auto src2r = Framework::GetA( src2 ) * src2a;
	const auto src2g = Framework::GetG( src2 ) * src2a;
	const auto src2b = Framework::GetB( src2 ) * src2a;

	const auto resr = static_cast<uint8_t>( src1r + src2r );
	const auto resg = static_cast<uint8_t>( src1g + src2g );
	const auto resb = static_cast<uint8_t>( src1b + src2b );


	Framework::PutPixel( gfx, X, Y, { resr,resg,resb } );
};

