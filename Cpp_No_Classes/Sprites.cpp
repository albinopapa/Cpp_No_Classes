#include "Sprites.h"
#include "ImageLoader.h"

namespace Framework::Graphics
{
	_Sprite::_Sprite( String::string Filename, const EffectFn Effect )
		:
		effect( Effect )
	{
		auto bitmap = ImageLoader::Load( std::move( Filename ) );

		UINT w, h;
		bitmap->GetSize( &w, &h );

		*this = _Sprite(
			static_cast< size_t >( w ),
			static_cast< size_t >( h ),
			Effect
		);

		bitmap->CopyPixels(
			nullptr,
			static_cast<UINT>( w * sizeof( _Color ) ),
			static_cast<UINT>( w * sizeof( _Color ) * h ),
			reinterpret_cast< BYTE* >( frame.get() )
		);
	}
	_Sprite::_Sprite( const size_t Width, const size_t Height, const EffectFn& Effect )
		:
		width( Width ), height( Height ),
		frame( std::make_unique<_Color[]>( Width * Height ) ),
		effect( Effect )
	{}
	_Sprite::_Sprite( _Sprite&& other )
	{
		*this = std::move( other );
	}

	_Sprite& _Sprite::operator=( _Sprite&& other )
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

	_AnimatedSprite::_AnimatedSprite(
		String::string basename,
		String::string file_extension,
		size_t FrameCount,
		float HoldFrameTime,
		const EffectFn& Effect )
		:
		holdFrameTime( HoldFrameTime )
	{
		size_t padding = 0;
		for( size_t value = FrameCount; value > 0; value /= 10 )
		{
			++padding;
		}

		for( size_t i = 0; i < FrameCount; ++i )
		{
			String::stringstream ss;
			ss.fill( L'0' );

			ss << basename << std::setw( padding ) << i << file_extension;
			frames.emplace_back( ss.str(), Effect );
		}

		cur_frame = frames.begin();
	}

}