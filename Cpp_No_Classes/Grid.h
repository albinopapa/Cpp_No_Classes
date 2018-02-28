#pragma once

#include "MathTypes.h"
#include <vector>

namespace Game
{
	struct _Entity;

	struct _Cell
	{
		std::vector<const _Entity*> occupants;
	};

	struct _Grid
	{
		_Grid( size_t Width, size_t Height );

		int32_t width, height;
		std::vector<_Cell> cells;
	};

	void AddEntity( _Cell& cell, const _Entity& entity );
	void AddEntity( _Grid& grid, const _Entity& entity, const Math::Vector::_Vec2<float>& position );
}