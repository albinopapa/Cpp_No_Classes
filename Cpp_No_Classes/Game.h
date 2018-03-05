#pragma once

#include "Entity.h"
#include "Graphics.h"
#include "Grid.h"
#include "Units.h"
#include <vector>

namespace Framework::Window{ struct _Window; }

namespace Game
{
	struct _Game
	{
		_Game( Framework::Window::_Window& _window);

		Framework::Graphics::_Graphics gfx;
		Framework::Window::_Window& window;

		std::vector<Units::_Unit> units;
		std::vector<Buildings::_Building> buildings;
		Grid::_Grid grid;

		Vec2i clickPos;
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

