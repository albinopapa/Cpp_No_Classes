#pragma once

#include "Framework.h"
#include <vector>

namespace Game
{
	struct _Entity;
	namespace Grid
	{
		struct _Grid;
	}

	namespace Cell
	{
		enum class CellTerrainType
		{
			Grass,Water,Sand
		};
		struct _Cell
		{
			_Cell() = default;
			_Cell( _Cell* _parent, int X, int Y, const Grid::_Grid& grid );

			static constexpr size_t cell_size = 16;
			static constexpr Color hilight_color = Color{ 160ui8, 92ui8, 64ui8, 128ui8 };

			bool isHilighted = false;
			CellTerrainType terrain;
			const _Entity* occupant;

			_Cell* parent = nullptr;
			int x = 0, y = 0;
			int id = 0;
			float g = 0.f;
			float h = 0.f;

			bool isBlocked = false;
		};

		void AddEntity( _Cell& cell, const _Entity& entity );
		void RemoveEntity( _Cell& cell );

		void Block( _Cell& cell );
		void Unblock( _Cell& cell );
		bool IsBlocked( const _Cell& cell );

		void Highlight( _Cell& cell );
		void Unhighlight( _Cell& cell );

		float GetF( const _Cell& _this );
		float ManhattenDistance( const _Cell& _this, const _Cell& end );

		void Draw( const _Cell& cell, int32_t X, int32_t Y, Graphics& gfx );

	}
	namespace Grid
	{
		struct _Grid;
		struct _Path
		{
			_Path( _Grid& _grid );

			_Grid& grid;
			Cell::_Cell* start, *end;
			std::vector<Vec2f> pathToGoal;
			std::vector<Cell::_Cell*> openList;
			std::vector<Cell::_Cell*> visitedList;

			bool startInitialized = false;
			bool goalFound = false;
		};
		struct _Grid
		{
			_Grid( size_t Width, size_t Height );

			int32_t width, height;
			std::vector<Cell::_Cell> cells;
			Cell::_Cell* hilightedCell = nullptr;
		};

		void AddEntity( _Grid& grid, const _Entity& entity );
		void RemoveEntity( _Grid& grid, const _Entity& entity );
		bool HasEntity( const _Grid& grid, const Vec2i& MouseClickPos );
		const Game::_Entity& GetEntity( const _Grid& grid, const Vec2i& MouseClickPos );

		void HighlightCell( _Grid& grid, const Vec2i& gridPos );
		void Draw( const _Grid& grid, Graphics& gfx );

		size_t FlattenIndex( const _Grid& grid, const int32_t X, const int32_t Y );
		size_t FlattenIndex( const _Grid& grid, const Vec2f& worldPos );
		size_t FlattenIndex( const _Grid& grid, const Vec2i& worldPos );
		Cell::_Cell& Cell( _Grid& grid, int32_t X, int32_t Y );

		void FindPath( _Path& path, const Vec2f& start, const Vec2f& end );
		Vec2f NextPathPosition( _Path& path, const Vec2f& curPos );
		std::vector<Vec2f> GetPath( _Path& path );
	}
}