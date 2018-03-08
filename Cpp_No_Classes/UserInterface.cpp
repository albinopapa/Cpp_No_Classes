#include "UserInterface.h"
#include "Game.h"

namespace Game
{

	void Draw( const _Viewport& vp, Graphics& gfx )
	{
		const auto clip = gfx.ScreenRect;
		const auto& camPos = vp.cam.GetPosition();
		constexpr auto invCellSize = 1.f / float( Cell::_Cell::cell_size );

		auto viewport = Recti{
			int32_t( camPos.x ),
			int32_t( camPos.y ),
			int32_t( camPos.x ) + clip.GetWidth(),
			int32_t( camPos.y ) + clip.GetHeight()
		}.Translate( static_cast< Vec2i >( camPos ) );

		const int32_t xStart = int32_t( camPos.x * invCellSize );
		const int32_t yStart = int32_t( camPos.y * invCellSize );
		const int32_t xEnd = int32_t( ( camPos.x + clip.GetWidth() ) * invCellSize );
		const int32_t yEnd = int32_t( ( camPos.y + clip.GetHeight() ) * invCellSize );

		for2d( xStart, yStart, xEnd, yEnd,[]( Index<2> idx )
		{

		} );
		
	}
}