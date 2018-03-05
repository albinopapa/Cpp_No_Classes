#include "Game.h"
#include "Window.h"
#include "Framework.h"
#include <random>

namespace Game
{
	namespace Gfx = Framework::Graphics;
	namespace Mouse = Framework::Input::Mouse;
	namespace Keyboard = Framework::Input::Keyboard;
	namespace Window = Framework::Window;

	_Game::_Game( Window::_Window& _window )
		:
		window( _window ),
		gfx( _window ),
		grid( Gfx::ScreenWidth / 16, Gfx::ScreenHeight / 16 )
	{
		{
			auto& unit = units.emplace_back( Units::traits<Units::Job::Builder>(), Team::Red );
			Units::SetPosition( unit, { 100.f,100.f } );
		}
		{
			auto& unit = units.emplace_back( Units::traits<Units::Job::Builder>(), Team::Blue );
			Units::SetPosition( unit, { 600.f,600.f } );
		}

		for( auto& unit : units )
		{
			Grid::AddEntity( grid, unit );
		}
	}

	void Go( _Game& game )
	{
		Gfx::BeginFrame( game.gfx, { 0,0,0 } );
		UpdateModel( game );
		ComposeFrame( game );
		Gfx::EndFrame( game.gfx );
	}
	void UpdateModel( _Game& game )
	{
		const auto mousePos = Mouse::GetPosition( Window::GetMouse( game.window ) );
		Grid::HighlightCell( game.grid, mousePos );
				
		const auto goal = Vec2f( float( mousePos.x ), float( mousePos.y ) );

		for( auto& unit : game.units )
		{
			Units::Update( unit, .016f, game.grid );
			const auto uniPos = Units::GetPosition( unit );
			Grid::FindPath( game.grid, uniPos, goal );
			while(!game.grid.goalFound)
			{
				Grid::FindPath( game.grid, uniPos, goal );
			}
		}
	}
	void ComposeFrame( _Game& game )
	{
		for( const auto& unit : game.units )
		{
			Units::Draw( game.gfx, unit );
		}
	}
}