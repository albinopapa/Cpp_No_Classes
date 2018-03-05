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

/*
You are a sole adventurer looking to start a colony in the middle of no where.
You goal is to survive the wilderness and defend against other colonies, perhaps even
take over and incorporate them into yours.

You start off with just a tent big enough for you.  
You have an axe to chop down trees, and some food to survive a few days on.
As a carpenter/builder this unit class is going to be building and purchasing the resources 
	to build your infrastructure.  You'll need a market to buy resources and houses for 
	citizens.  
	A tent will accommodate one person.
	A hut will accommodate one two person family.
	A house will accommodate one family; husband, wife and upto two kids.
	A mansion will accommodate one five person family and three work hands.
	The market will attract various shops owners, from blacksmiths to butchers
	The butcher shop will be used to buy/sell meat.
	The warehouse is used to store excess materials.
	The barracks is used to convert a unit to a military officer.
	The stable is used to lodge and care for your horses.
	The port is used to buy fish, or convert a unit to a naval officer.
*/