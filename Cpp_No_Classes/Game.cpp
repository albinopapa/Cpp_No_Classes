#include "Game.h"
#include "Window.h"
#include "Framework.h"
#include <random>

namespace Game
{
	template<Units::Job _job>
	Units::_Unit Create( Units::Team team)
	{
		/*return Units::_Unit(
			Units::traits<Units::Job::Builder>::attack,
			Units::traits<Units::Job::Builder>::base_hp,
			Units::traits<Units::Job::Builder>::armor,
			Units::traits<Units::Job::Builder>::job,
			team
		);*/
		return Units::_Unit(
			Units::traits<_job>::attack,
			Units::traits<_job>::base_hp,
			Units::traits<_job>::armor,
			Units::traits<_job>::job,
			team
		);
	}
	
	void CreateUnits( _Game& game )
	{
		game.units.reserve( 25 );

		std::random_device rd;
		std::mt19937 rng( rd() );
		std::uniform_int_distribution<uint32_t> job_dist( 0, 7 );
		std::uniform_int_distribution<uint32_t> team_dist( 0, 1 );

		for( size_t i = 0; i < 25; ++i )
		{
			const auto job_idx = 
				static_cast< Units::Job >( job_dist( rng ) );
			const auto team_idx =
				static_cast< Units::Team >( team_dist( rng ) );

			switch( job_idx )
			{
				case Units::Job::Builder:
					game.units.push_back( Create<Units::Job::Builder>( team_idx ) );
					break;
				case Units::Job::Farmer:
					game.units.push_back( Create<Units::Job::Farmer>( team_idx ) );
					break;
				case Units::Job::Fisher:
					game.units.push_back( Create<Units::Job::Fisher>( team_idx ) );
					break;
				case Units::Job::Gatherer:
					game.units.push_back( Create<Units::Job::Gatherer>( team_idx ) );
					break;
				case Units::Job::FootSoldier:
					game.units.push_back( Create<Units::Job::FootSoldier>( team_idx ) );
					break;
				case Units::Job::Mounted:
					game.units.push_back( Create<Units::Job::Mounted>( team_idx ) );
					break;
				case Units::Job::Naval:
					game.units.push_back( Create<Units::Job::Naval>( team_idx ) );
					break;
				case Units::Job::Scout:
					game.units.push_back( Create<Units::Job::Scout>( team_idx ) );
					break;
			}
		}
	}

	_Game::_Game( Window::_Window& _window )
		:
		window( _window ),
		gfx( _window )
	{
		CreateUnits( *this );
	}

	void Go( _Game& game )
	{
		Framework::BeginFrame( game.gfx, { 0,0,0 } );
		UpdateModel( game );
		ComposeFrame( game );
		Framework::EndFrame( game.gfx );
	}
	void UpdateModel( _Game& game )
	{
		for( auto unit : game.units )
		{
			Units::Update( unit, .016f );

			AddEntity( game.grid, unit, unit.position );
		}
	}
	void ComposeFrame( _Game& game )
	{
		Framework::DrawRect( game.gfx, 100, 100, 30, 30, { 0, 255, 0 } );
	}
}