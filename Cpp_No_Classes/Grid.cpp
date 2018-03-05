#include "Grid.h"
#include "Entity.h"
#include <algorithm>
#include <cassert>
#include <limits>

namespace Game
{
	namespace Cell
	{
		_Cell::_Cell( _Cell* _parent, int X, int Y, const Grid::_Grid& grid )
			:
			x( X ), y( Y ),
			parent( _parent ),
			id( X + ( Y * grid.width ) )
		{}

		void AddEntity( _Cell& cell, const _Entity& entity )
		{
			if( !IsBlocked( cell ) )
			{
				cell.occupant = &entity;
				Block( cell );
			}
		}
		void RemoveEntity( _Cell& cell )
		{
			if( IsBlocked( cell ) )
			{
				Unblock( cell );
			}
			cell.occupant = nullptr;
		}

		void Highlight( _Cell& cell )
		{
			cell.isHilighted = true;
		}
		void Unhighlight( _Cell& cell )
		{
			cell.isHilighted = false;
		}
		void Draw( const _Cell& cell, int32_t X, int32_t Y, Framework::Graphics::_Graphics& gfx )
		{
			using Framework::Graphics::_Color;
			
			const _Color base = []( const CellTerrainType t_type )->_Color
			{
				switch( t_type )
				{
					case CellTerrainType::Grass:
					{
						return { 0, 128, 16 };
					}
					case CellTerrainType::Water:
					{
						return { 0, 16, 128 };
					}
					case CellTerrainType::Sand:
					{
						return { 160, 120, 32 };
					}
					default:
						return { 0,0,0 };
				}
			}(cell.terrain);
			
			if( cell.isHilighted )
			{
				const auto h_color = cell.hilight_color;

				const uint32_t srcA1 = Framework::Graphics::GetA( h_color );
				const uint32_t srcR1 = (Framework::Graphics::GetR( h_color ) * srcA1)>>8;
				const uint32_t srcG1 = (Framework::Graphics::GetG( h_color ) * srcA1)>>8;
				const uint32_t srcB1 = (Framework::Graphics::GetB( h_color ) * srcA1)>>8;

				const uint32_t srcA2 = 255 - srcA1;
				const uint32_t srcR2 = (Framework::Graphics::GetR( base ) * srcA2) >> 8;
				const uint32_t srcG2 = (Framework::Graphics::GetG( base ) * srcA2) >> 8;
				const uint32_t srcB2 = (Framework::Graphics::GetB( base ) * srcA2) >> 8;

				const _Color c = { srcR1 + srcR2, srcG1 + srcG2, srcB1 + srcG2 };

				Framework::Graphics::PutPixel( gfx, X, Y, c );
			}

			Framework::Graphics::PutPixel( gfx, X, Y, base );
		}

		float GetF( const _Cell& _this )
		{
			return _this.g + _this.h;
		}
		float ManhattenDistance( const _Cell& _this, const _Cell& end )
		{
			const auto mdx = std::abs( _this.x - end.x );
			const auto mdy = std::abs( _this.y - end.y );

			return static_cast< float >( mdx + mdy );
		}
		void Block( _Cell& cell )
		{
			cell.isBlocked = true;
		}
		void Unblock( _Cell& cell )
		{
			cell.isBlocked = false;
		}
		bool IsBlocked( const _Cell& cell )
		{
			return cell.isBlocked;
		}
	}
	namespace Grid
	{
		_Grid::_Grid( size_t Width, size_t Height )
			:
			width( static_cast< int32_t >( Width ) ),
			height( static_cast< int32_t >( Height ) ),
			cells( Width * Height )
		{

		}

		void AddEntity( _Grid& grid, const _Entity& entity )
		{
			const auto position = entity.position;
			assert( position.x >= 0.f && position.y >= 0.f );

			const auto idx = 
				( int32_t( position.x ) / Cell::_Cell::cell_size ) +
				( ( int32_t( position.y ) / Cell::_Cell::cell_size ) * grid.width );

			

			AddEntity( grid.cells[ idx ], entity );
		}
		void RemoveEntity( _Grid& grid, const _Entity& entity )
		{
			const auto entPos = Game::GetPosition( entity );
			Cell::RemoveEntity( grid.cells[ FlattenIndex( grid, entPos ) ] );
		}
		bool HasEntity( const _Grid& grid, const Vec2i& MouseClickPos )
		{
			return Cell::IsBlocked( grid.cells[ FlattenIndex( grid, MouseClickPos ) ] );
		}
		const Game::_Entity& GetEntity( const _Grid& grid, const Vec2i& MouseClickPos )
		{
			assert( HasEntity( grid, MouseClickPos ) );

			return *grid.cells[ FlattenIndex( grid, MouseClickPos ) ].occupant;
		}

		void HighlightCell( _Grid& grid, const Vec2i& gridPos )
		{
			assert( gridPos.x >= 0 && gridPos.y >= 0 );
			assert( gridPos.x < grid.width && gridPos.y < grid.height );

			if( grid.hilightedCell )
			{
				Cell::Unhighlight( *grid.hilightedCell );
			}

			const auto idx = gridPos.x + ( gridPos.y * grid.width );
			if(idx >= 0 && idx < grid.width * grid.height)
			{
				Highlight( grid.cells[ idx ] );
				grid.hilightedCell = &grid.cells[ idx ];
			}
		}

		size_t FlattenIndex( const _Grid& grid, const int X, const int Y )
		{
			assert( X >= 0 );
			assert( X < grid.width );
			assert( Y >= 0 );
			assert( Y < grid.height );

			return X + ( Y * grid.width );
		}
		size_t FlattenIndex( const _Grid& grid, const Vec2f& worldPos )
		{
			return FlattenIndex( grid,
				static_cast< int >( worldPos.x ) / Cell::_Cell::cell_size,
				static_cast< int >( worldPos.y ) / Cell::_Cell::cell_size
			);
		}
		size_t FlattenIndex( const _Grid& grid, const Vec2i& worldPos )
		{
			return FlattenIndex( grid,
				worldPos.x / Cell::_Cell::cell_size,
				worldPos.y / Cell::_Cell::cell_size 
			);
		}
		Cell::_Cell& Cell( _Grid& grid, int X, int Y )
		{
			const auto idx = FlattenIndex( grid, X, Y );
			return Cell( grid, idx );
		}
		Cell::_Cell& Cell( _Grid& grid, size_t idx )
		{
			return grid.cells[ idx ];
		}

		void Draw( const _Grid& grid, Framework::Graphics::_Graphics& gfx )
		{
			for( int32_t y = 0; y < grid.height; ++y )
			{
				const auto row = y * grid.width;
				const auto cy = y * Cell::_Cell::cell_size;

				for( int32_t x = 0; x < grid.width; ++x )
				{
					const auto idx = x + row;
					const auto cx = x * Cell::_Cell::cell_size;
					Cell::Draw( grid.cells[ idx ], cx, cy, gfx );
				}
			}
		}

		void FindPath( _Grid& grid, const Vec2f& start, const Vec2f& end)
		{
			if( !grid.startInitialized )
			{
				ClearOpenList( grid );
				ClearVisitedList( grid );
				ClearPathToGoal( grid );

				Cell::_Cell startCell, endCell;
				startCell.x = int( start.x ) / Cell::_Cell::cell_size;
				startCell.y = int( start.y ) / Cell::_Cell::cell_size;
				endCell.x = int( end.x ) / Cell::_Cell::cell_size;
				endCell.y = int( end.y ) / Cell::_Cell::cell_size;

				SetStartAndGoal( grid, startCell, endCell );
				grid.startInitialized = true;
			}
			if( grid.startInitialized )
			{
				ContinuePath( grid );
			}
		}
		Vec2f NextPathPosition( _Grid& grid, const Vec2f& curPos )
		{
			constexpr float sqRadius = Cell::_Cell::cell_size * Cell::_Cell::cell_size;

			if( grid.pathToGoal.empty() ) 
			{
				return curPos;
			}

			const auto nextPos = grid.pathToGoal.back();
			auto sqDist = Math::Vector::DistanceSq( curPos, nextPos );

			if( sqDist < sqRadius )
			{
				grid.pathToGoal.pop_back();
			}

			return nextPos;
		}

		void ClearOpenList( _Grid& grid )
		{
			grid.openList.clear();
		}
		void ClearVisitedList( _Grid& grid )
		{
			grid.visitedList.clear();
		}
		void ClearPathToGoal( _Grid& grid )
		{
			grid.pathToGoal.clear();
		}

		void SetStartAndGoal( _Grid& grid, Cell::_Cell start, Cell::_Cell end )
		{
			auto& c_start = grid.cells[ start.x + ( start.y * grid.width ) ];
			auto& c_end = grid.cells[ end.x + ( end.y * grid.width ) ];
			c_start = Cell::_Cell( nullptr, start.x, start.y, grid );
			c_end = Cell::_Cell( &end, end.x, end.y, grid );

			c_start.g = 0;
			c_start.h = ManhattenDistance( *grid.start, *grid.end );
			
			grid.start = &c_start;
			grid.end = &c_end;
			
			grid.openList.push_back( grid.start );
		}

		void ContinuePath( _Grid& grid )
		{
			if( grid.openList.empty() ) return;

			Cell::_Cell* curCell = GetNextCell( grid );

			if( curCell->id == grid.end->id )
			{
				grid.end->parent = curCell->parent;

				for( Cell::_Cell* getPath = grid.end; getPath != nullptr; getPath = getPath->parent )
				{
					constexpr auto half_size = int32_t( Cell::_Cell::cell_size / 2 );
					const auto cellPosX = ( getPath->x * int32_t( Cell::_Cell::cell_size ) ) + half_size;
					const auto cellPosY = ( getPath->y * int32_t( Cell::_Cell::cell_size ) ) + half_size;

					const auto worldPos = Vec2f( float( cellPosX ), float( cellPosY ) );
					grid.pathToGoal.push_back( worldPos );
				}

				grid.goalFound = true;
			}
			else
			{
				// North cell
				PathOpened( grid, curCell->x, curCell->y - 1, curCell->g + 1, curCell );
				// NEast cell
				PathOpened( grid, curCell->x + 1, curCell->y - 1, curCell->g + 1.4f, curCell );
				// East cell
				PathOpened( grid, curCell->x + 1, curCell->y, curCell->g + 1, curCell );
				// SEast cell
				PathOpened( grid, curCell->x + 1, curCell->y + 1, curCell->g + 1.4f, curCell );
				// South cell
				PathOpened( grid, curCell->x, curCell->y + 1, curCell->g + 1, curCell );
				// SWest cell
				PathOpened( grid, curCell->x - 1, curCell->y + 1, curCell->g + 1.4f, curCell );
				// West cell
				PathOpened( grid, curCell->x - 1, curCell->y, curCell->g + 1, curCell );
				// NWest cell
				PathOpened( grid, curCell->x - 1, curCell->y - 1, curCell->g + 1.4f, curCell );

				auto beg = grid.openList.begin();
				auto end = grid.openList.end();
				auto remit = std::remove_if(
					beg, end, [ curCell ]( const Cell::_Cell* node )
				{
					return curCell->id == node->id;
				} );

				grid.openList.erase( remit, end );
			}
		}
		void PathOpened( _Grid& grid, int X, const int Y, float newCost, Cell::_Cell* _parent )
		{
			// Caclulate this cell's position
			const auto id = FlattenIndex( grid, X, Y );

			// Used to find a cell in a list
			auto match_id = [ id ]( const Cell::_Cell* cell )
			{
				return id == cell->id;
			};
			auto is_in_list = [ &match_id, id ]( auto list )
			{
				auto it =
					std::find_if( list.begin(), list.end(), match_id );

				return it != list.end();
			};

			// Determine if this cell is less expensive to travel through
			auto child = Cell::_Cell( _parent, X, Y, grid );
			child.g = newCost;
			child.h = ManhattenDistance( *_parent, *grid.end );

			// Return if cell is in open and is already occupied or in visited lists 
			if( is_in_list( grid.openList ) )
			{
				if( IsBlocked( *grid.openList[ id ] ) ||
					is_in_list( grid.visitedList ) )
				{
					return;
				}


				auto* node = grid.openList[ id ];
				float newF = child.g + newCost + node->h;

				if( newF > GetF( *node ) )
				{
					return;
				}

				node->g = child.g + newCost;
				node->parent = &child;

				*node = child;
			}
			else
			{
				grid.cells[ id ] = child;
				grid.openList.push_back( &grid.cells[ id ] );
			}
			
		}
		Cell::_Cell* GetNextCell( _Grid& grid )
		{
			float bestValue = std::numeric_limits<float>::max();
			int cellIndex = -1, i = 0;

			Cell::_Cell* nextCell = nullptr;

			for( auto* node : grid.openList )
			{
				const auto f = GetF( *node );
				if( f < bestValue )
				{
					bestValue = f;
					cellIndex = i;
				}
				++i;
			}

			if( cellIndex >= 0 )
			{
				nextCell = grid.openList[ cellIndex ];
				grid.visitedList.push_back( nextCell );

				if( grid.openList.size() > 1 )
					std::swap( grid.openList[ cellIndex ], grid.openList.back() );

				if( grid.openList.size() >= 1 )
					grid.openList.pop_back();
			}

			return nextCell;
		}

	}
}
