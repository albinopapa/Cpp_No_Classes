#include "Grid.h"
#include "Entity.h"
#include <cassert>

namespace Game
{
	_Grid::_Grid( size_t Width, size_t Height )
		:
		width( static_cast< int32_t >( Width ) ),
		height( static_cast< int32_t >( Height ) )
	{

	}

	void AddEntity( _Cell& cell, const _Entity& entity )
	{
		cell.occupants.push_back( &entity );
	}
	void AddEntity( _Grid& grid, const _Entity& entity, const Math::Vector::_Vec2<float>& position )
	{
		assert( position.x >= 0.f && position.y >= 0.f );

		const auto idx = int32_t( position.x ) + ( int32_t( position.y )* grid.width );

		AddEntity( grid.cells[ idx ], entity );
	}

}