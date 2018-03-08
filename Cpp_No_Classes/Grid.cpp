#include "Grid.h"
#include "Entity.h"
#include <algorithm>
#include <cassert>
#include <limits>

namespace Game
{	
	// Private forward declarations
	namespace Grid
	{
		Cell::_Cell& Cell( _Grid& grid, size_t idx );

		void FindPath( _Path& path, const Vec2f& start, const Vec2f& end );
		Vec2f NextPathPosition( _Path& path, const Vec2f& curPos );
		void ClearOpenList( _Path& path );
		void ClearVisitedList( _Path& path );
		void ClearPathToGoal( _Path& path );
		void SetStartAndGoal( _Path& path, Cell::_Cell start, Cell::_Cell end );
		void ContinuePath( _Path& path );
		void PathOpened( _Path& path, int X, const int Y, float newCost, Cell::_Cell* _parent );
		Cell::_Cell* GetNextCell( _Path& path );
	}

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
		void Draw( const _Cell& cell, int32_t X, int32_t Y, Graphics& gfx )
		{
			const Color base = []( const CellTerrainType t_type )->Color
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
			}( cell.terrain );

			const auto c = ( cell.isHilighted ) ?
				cell.hilight_color.AlphaBlend( base ) : base;

			gfx.PutPixel( X, Y, c );
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

		_Path::_Path( _Grid& _grid )
			:
			grid( _grid )
		{

		}

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
			const auto x = gridPos.x / Cell::_Cell::cell_size;
			const auto y = gridPos.y / Cell::_Cell::cell_size;

			assert( x >= 0 && y >= 0 );
			assert( x < grid.width && y < grid.height );

			if( grid.hilightedCell )
			{
				Cell::Unhighlight( *grid.hilightedCell );
			}

			const auto idx = x + ( y * grid.width );

			if(idx >= 0 && idx < grid.width * grid.height)
			{
				Highlight( grid.cells[ idx ] );
				grid.hilightedCell = &grid.cells[ idx ];
			}
		}

		size_t FlattenIndex( const _Grid& grid, const int32_t X, const int32_t Y )
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
		Cell::_Cell& Cell( _Grid& grid, size_t idx )
		{
			return grid.cells[ idx ];
		}
		Cell::_Cell& Cell( _Grid& grid, int32_t X, int32_t Y )
		{
			const auto idx = FlattenIndex( grid, X, Y );
			return Cell( grid, idx );
		}

		void Draw( const _Grid& grid, Graphics& gfx )
		{
			constexpr auto c_size = int32_t( Cell::_Cell::cell_size );

			for( int32_t y = 0; y < grid.height; ++y )
			{
				const int32_t row = y * grid.width;
				const int32_t cy = y * c_size;

				for( int32_t x = 0; x < grid.width; ++x )
				{
					const auto idx = size_t( x + row );
					const auto cx = x * c_size;
					Cell::Draw( grid.cells[ idx ], cx, cy, gfx );
				}
			}
		}

		void FindPath( _Path& path, const Vec2f& start, const Vec2f& end)
		{
			if( !path.startInitialized )
			{
				ClearOpenList( path );
				ClearVisitedList( path );
				ClearPathToGoal( path );

				Cell::_Cell startCell, endCell;
				startCell.x = int( start.x ) / Cell::_Cell::cell_size;
				startCell.y = int( start.y ) / Cell::_Cell::cell_size;
				endCell.x = int( end.x ) / Cell::_Cell::cell_size;
				endCell.y = int( end.y ) / Cell::_Cell::cell_size;

				SetStartAndGoal( path, startCell, endCell );
				path.startInitialized = true;
			}
			if( path.startInitialized )
			{
				ContinuePath( path );
			}
		}
		Vec2f NextPathPosition( _Path& path, const Vec2f& curPos )
		{
			constexpr float sqRadius = Cell::_Cell::cell_size * Cell::_Cell::cell_size;

			if( path.pathToGoal.empty() ) 
			{
				return curPos;
			}

			const auto nextPos = path.pathToGoal.back();
			auto sqDist = curPos.DistanceSq( nextPos );

			if( sqDist < sqRadius )
			{
				path.pathToGoal.pop_back();
			}

			return nextPos;
		}

		void ClearOpenList( _Path& path )
		{
			path.openList.clear();
		}
		void ClearVisitedList( _Path& path )
		{
			path.visitedList.clear();
		}
		void ClearPathToGoal( _Path& path )
		{
			path.pathToGoal.clear();
		}

		void SetStartAndGoal( _Path& path, Cell::_Cell start, Cell::_Cell end )
		{
			auto& c_start = path.grid.cells[ start.x + ( start.y * path.grid.width ) ];
			auto& c_end = path.grid.cells[ end.x + ( end.y * path.grid.width ) ];
			c_start = Cell::_Cell( nullptr, start.x, start.y, path.grid );
			c_end = Cell::_Cell( &end, end.x, end.y, path.grid );

			c_start.g = 0;
			c_start.h = ManhattenDistance( *path.start, *path.end );
			
			path.start = &c_start;
			path.end = &c_end;
			
			path.openList.push_back( path.start );
		}

		void ContinuePath( _Path& path )
		{
			if( path.openList.empty() ) return;

			Cell::_Cell* curCell = GetNextCell( path );

			if( curCell->id == path.end->id )
			{
				path.end->parent = curCell->parent;

				for( Cell::_Cell* getPath = path.end; getPath != nullptr; getPath = getPath->parent )
				{
					constexpr auto half_size = int32_t( Cell::_Cell::cell_size / 2 );
					const auto cellPosX = ( getPath->x * int32_t( Cell::_Cell::cell_size ) ) + half_size;
					const auto cellPosY = ( getPath->y * int32_t( Cell::_Cell::cell_size ) ) + half_size;

					const auto worldPos = Vec2f( float( cellPosX ), float( cellPosY ) );
					path.pathToGoal.push_back( worldPos );
				}

				path.goalFound = true;
			}
			else
			{
				// North cell
				PathOpened( path, curCell->x, curCell->y - 1, curCell->g + 1, curCell );
				// NEast cell
				PathOpened( path, curCell->x + 1, curCell->y - 1, curCell->g + 1.4f, curCell );
				// East cell
				PathOpened( path, curCell->x + 1, curCell->y, curCell->g + 1, curCell );
				// SEast cell
				PathOpened( path, curCell->x + 1, curCell->y + 1, curCell->g + 1.4f, curCell );
				// South cell
				PathOpened( path, curCell->x, curCell->y + 1, curCell->g + 1, curCell );
				// SWest cell
				PathOpened( path, curCell->x - 1, curCell->y + 1, curCell->g + 1.4f, curCell );
				// West cell
				PathOpened( path, curCell->x - 1, curCell->y, curCell->g + 1, curCell );
				// NWest cell
				PathOpened( path, curCell->x - 1, curCell->y - 1, curCell->g + 1.4f, curCell );

				auto beg = path.openList.begin();
				auto end = path.openList.end();
				auto remit = std::remove_if(
					beg, end, [ curCell ]( const Cell::_Cell* node )
				{
					return curCell->id == node->id;
				} );

				path.openList.erase( remit, end );
			}
		}
		void PathOpened( _Path& path, int X, const int Y, float newCost, Cell::_Cell* _parent )
		{
			// Caclulate this cell's position
			const auto id = FlattenIndex( path.grid, X, Y );

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
			auto child = Cell::_Cell( _parent, X, Y, path.grid );
			child.g = newCost;
			child.h = ManhattenDistance( *_parent, *path.end );

			// Return if cell is in open and is already occupied or in visited lists 
			if( is_in_list( path.openList ) )
			{
				if( IsBlocked( *path.openList[ id ] ) ||
					is_in_list( path.visitedList ) )
				{
					return;
				}


				auto* node = path.openList[ id ];
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
				path.grid.cells[ id ] = child;
				path.openList.push_back( &path.grid.cells[ id ] );
			}
			
		}
		Cell::_Cell* GetNextCell( _Path& path )
		{
			float bestValue = std::numeric_limits<float>::max();
			int cellIndex = -1, i = 0;

			Cell::_Cell* nextCell = nullptr;

			for( auto* node : path.openList )
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
				nextCell = path.openList[ cellIndex ];
				path.visitedList.push_back( nextCell );

				if( path.openList.size() > 1 )
					std::swap( path.openList[ cellIndex ], path.openList.back() );

				if( path.openList.size() >= 1 )
					path.openList.pop_back();
			}

			return nextCell;
		}
		std::vector<Vec2f> GetPath( _Path& path )
		{
			std::reverse( path.pathToGoal.begin(), path.pathToGoal.end() );
			return path.pathToGoal;
		}
	}
}
