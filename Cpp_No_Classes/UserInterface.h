#pragma once

#include "Framework.h"


namespace Game
{
	struct _Game;

	namespace Grid
	{
		struct _Grid;
	}
	struct _Viewport
	{
		TrackingCamera cam;
		Grid::_Grid* pGrid = nullptr;
		_Game* pGame = nullptr;
	};

	void Draw( const _Viewport, Graphics& gfx );
}