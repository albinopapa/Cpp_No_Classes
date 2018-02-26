#pragma once

#include "Graphics.h"

namespace Window{ struct _Window; }

namespace Game
{
	struct _Game
	{
		_Game(Window::_Window& _window);

		Graphics::_Graphics gfx;
		Window::_Window& window;
	};

	void Go( _Game& game );
	void UpdateModel( _Game& game );
	void ComposeFrame( _Game& game );
}

// 2 teams - Red / Blue ( 2 for demo maybe more for release )
// 8 units - Builder / Farmer / Fisher / Gatherer / FootSoldier / Mounted / Naval / Scout
// Buildings - 
//		Hut / House / Mansion / Tent
//		Butcher / Market / Warehouse
//		Barracks / Stable / Port 
// Terrain - 
//		Water / Canyon / Field / Dessert / Trees 
// Resources - 
//		food, ore, wood, cloth

