#include "Game.h"
#include <random>

namespace Game
{

	_Game::_Game( MainWindow& _window )
		:
		window( _window ),
		gfx( _window ),
		grid( Graphics::ScreenWidth / 16, Graphics::ScreenHeight / 16 )
	{
		{
			auto& unit = units.emplace_back( Units::traits<Units::Job::Builder>(), Team::Red, grid );
			Units::SetPosition( unit, { 100.f,100.f } );
		}
		{
			auto& unit = units.emplace_back( Units::traits<Units::Job::Builder>(), Team::Blue, grid );
			Units::SetPosition( unit, { 600.f,600.f } );
		}

		for( auto& unit : units )
		{
			Grid::AddEntity( grid, unit );
		}
	}

	void Go( _Game& game )
	{
		game.gfx.BeginFrame();
		UpdateModel( game );
		ComposeFrame( game );
		game.gfx.EndFrame();
	}
	void UpdateModel( _Game& game )
	{		
		const auto mousePos = game.window.mouse.GetPos();
		if( Graphics::ScreenRect.Contains( mousePos ) )
		{
			Grid::HighlightCell( game.grid, mousePos );			
		}
		
	}
	void ComposeFrame( _Game& game )
	{
		for( const auto& unit : game.units )
		{
			Units::Draw( unit, game.gfx );
		}
	}
}