#pragma once

#include "Color.h"
#include "Framework.h"
#include "Graphics.h"
#include "string_util.h"
#include "ImageLoader.h"
#include <memory>
#include <vector>
#include <iomanip>

namespace Graphics
{
	struct _Sprite
	{
		_Sprite() = default;
		_Sprite( String::string Filename, const EffectFn Effect );
		_Sprite( const size_t Width, const size_t Height, const EffectFn& Effect );
		_Sprite( _Sprite&& other );

		_Sprite& operator=( _Sprite&& other );

		size_t width = 0, height = 0;
		std::unique_ptr<Color::_Color[]> frame;
		EffectFn effect = nullptr;
	};	

	struct _AnimatedSprite
	{
		_AnimatedSprite() = default;
		_AnimatedSprite(
			String::string basename,
			String::string file_extension,
			size_t FrameCount,
			float HoldFrameTime,
			const EffectFn& Effect );

		std::vector<_Sprite> frames;
		std::vector<_Sprite>::iterator cur_frame;
		float holdFrameTime = 0.f, curHoldTime = 0.f;
	};
}

auto PassThrough = [](
	Graphics::_Graphics& gfx,
	const Color::_Color src1,
	const Color::_Color src2 )->Color::_Color
{	
	return src1;
};

auto AlphaEffect = [](
	Graphics::_Graphics& gfx,
	const Color::_Color src1,
	const Color::_Color src2 )->Color::_Color
{
	const auto src1a = Framework::GetA( src1 );
	const auto src1r = Framework::GetR( src1 ) * src1a;
	const auto src1g = Framework::GetG( src1 ) * src1a;
	const auto src1b = Framework::GetB( src1 ) * src1a;

	const auto src2a = 255ui8 - src1a;
	const auto src2r = Framework::GetA( src2 ) * src2a;
	const auto src2g = Framework::GetG( src2 ) * src2a;
	const auto src2b = Framework::GetB( src2 ) * src2a;

	const auto resr = static_cast<uint8_t>( src1r + src2r );
	const auto resg = static_cast<uint8_t>( src1g + src2g );
	const auto resb = static_cast<uint8_t>( src1b + src2b );

	return { resr,resg,resb };
};

