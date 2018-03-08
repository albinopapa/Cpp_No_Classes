#include "Units.h"
#include "Grid.h"
#include <cassert>


// Private forward declarations
namespace Game::Units
{
	namespace Builder{ void Update( _Unit& unit, float dt, Grid::_Grid& grid ); }
	namespace Farmer{ void Update( _Unit& unit, float dt, Grid::_Grid& grid ); }
	namespace Fisher{ void Update( _Unit& unit, float dt, Grid::_Grid& grid ); }
	namespace Gatherer{ void Update( _Unit& unit, float dt, Grid::_Grid& grid ); }
	namespace FootSoldier{ void Update( _Unit& unit, float dt, Grid::_Grid& grid ); }
	namespace Mounted{ void Update( _Unit& unit, float dt, Grid::_Grid& grid ); }
	namespace Naval{ void Update( _Unit& unit, float dt, Grid::_Grid& grid ); }
	namespace Scout{ void Update( _Unit& unit, float dt, Grid::_Grid& grid ); }
}

// Units base class method definitions
namespace Game::Units
{

	_Unit::_Unit( float Attack, float HP, float Armor, Job _Job, Team _Team, Grid::_Grid& grid )
		:
		attack( Attack ), 
		hp( HP ), 
		armor( Armor ), 
		job( _Job ),
		path( grid )
	{
		switch( job )
		{
			case Job::Builder:
				vtable.Update = &Builder::Update;				
				break;
			case Job::Farmer:
				vtable.Update = &Farmer::Update;
				break;
			case Job::Fisher:
				vtable.Update = &Fisher::Update;
				break;
			case Job::Gatherer:
				vtable.Update = &Gatherer::Update;
				break;
			case Job::FootSoldier:
				vtable.Update = &FootSoldier::Update;
				break;
			case Job::Mounted:
				vtable.Update = &Mounted::Update;
				break;
			case Job::Naval:
				vtable.Update = &Naval::Update;
				break;
			case Job::Scout:
				vtable.Update = &Scout::Update;
				break;
		}
		entity.team = _Team;
		entity.category = Game::EntityCategory::Unit;
	}

	_Unit::operator _Entity&() 
	{
		return entity;
	}
	_Unit::operator const _Entity&( )const
	{
		return entity;
	}

	void Update( _Unit& _this, float dt, Grid::_Grid& grid )
	{
		assert( _this.vtable.Update );

		_this.vtable.Update( _this, dt, grid );
		
	}	
	void Draw( const _Unit& _this, Graphics& gfx )
	{
		const auto color = _this.entity.team == Team::Red ?
			Colors::Red : Colors::Blue;

		const auto rect = _this.rect.Translate( GetPosition( _this ) );

		gfx.DrawRect( static_cast< Recti >( rect ), color );
	}

	Vec2f GetPosition( const _Unit& _this )
	{
		return Game::GetPosition( _this.entity );
	}
	Job GetJob( const _Unit& _this )
	{
		return _this.job;
	}
	bool IsAlive( const _Unit& _this )
	{
		return _this.hp > 0.f;
	}
	bool IsIdle( const _Unit& _this )
	{
		return _this.jobstate == JobState::isIdle;
	}

	void SetAttackingUnit( _Unit& _this, const _Unit& pAttacker )
	{
		_this.p_attacker = &pAttacker;
	}
	void SetPosition( _Unit& _this, const Vec2f position )
	{
		_this.entity.position = position;
	}
	void SetTargetPosition( _Unit& _this, const Vec2f* position )
	{
		_this.targetPosition = position;
	}
	void SetJobPosition( _Unit& _this, const Vec2f position )
	{
		_this.jobLocation = position;
	}
}

// Derived class definitions
namespace Game::Units::Builder
{
	void HandleIdle( _Unit& unit )
	{
		unit.jobstate = JobState::isPurchasing;
	}
	void HandleTransitioning( _Unit& unit, float dt, Grid::_Grid& grid )
	{
		// Transitioning is when unit is carrying materials to warehouse or from market 

		// If transitioning and no target is set, 
		//		- if has materials to store
		//			- Locate warehouse
		//			- Check if has materials
		//				- if no materials
		//				- go to market
		//		- if has materials to build
		//			- Locate buildsite
		//			- change state to isBuilding
		// TODO: Add building locator abstraction
		if(unit.targetPosition)
		{
			while( !unit.path.goalFound )
			{
				Grid::FindPath( unit.path, GetPosition( unit ), *unit.targetPosition );
			}
			if( unit.pathToGoal.empty() )
			{
				unit.pathToGoal = Grid::GetPath( unit.path );
				
			}
			else
			{
				if( ( unit.jobLocation.DistanceSq( *unit.targetPosition ) <=
					Cell::_Cell::cell_size * Cell::_Cell::cell_size ) )
				{
					unit.jobstate = JobState::isBuilding;
				}
				
			}
		}
		else
		{
			unit.jobstate = JobState::isIdle;
		}

		unit.entity.position += ( unit.entity.velocity * dt );
	}
	void HandleAttacking( _Unit& unit )
	{
		++unit.aniframe;
		unit.aniframe %= _Unit::max_aniframes;

		if( !unit.p_attacker || !IsAlive( *unit.p_attacker ) )
		{
			unit.jobstate = JobState::isIdle;
		}
	}
	void HandlePurchasing( _Unit& unit, float dt, Grid::_Grid& grid )
	{
		unit.entity.position += ( unit.entity.velocity * dt );
	}
	void HandleJob( _Unit& unit )
	{

	}
	void Update( _Unit& unit, float dt, Grid::_Grid& grid )
	{
		switch( unit.jobstate )
		{
			case JobState::isAttacking:
				HandleAttacking( unit );
				break;
			case JobState::isBuilding:
				HandleJob( unit );
				break;
			case JobState::isIdle:
				HandleIdle( unit );
				break;
			case JobState::isPurchasing:
				HandlePurchasing( unit, dt, grid );
				break;
			case JobState::isTransitioning:
				HandleTransitioning( unit, dt, grid );
				break;
		}
	}
}
namespace Game::Units::Farmer
{
	void HandleIdle( _Unit& unit )
	{
		unit.jobstate = JobState::isPurchasing;
	}
	void HandleTransitioning( _Unit& unit, float dt )
	{
		unit.entity.position += ( unit.entity.velocity * dt );
	}
	void HandleJob( _Unit& unit )
	{
		++unit.aniframe;
		unit.aniframe %= _Unit::max_aniframes;
	}
	void HandleAttacking( _Unit& unit )
	{
		++unit.aniframe;
		unit.aniframe %= _Unit::max_aniframes;

		if( !unit.p_attacker || !IsAlive( *unit.p_attacker ) )
		{
			unit.jobstate = JobState::isIdle;
		}
	}
	void HandlePurchasing( _Unit& unit, float dt )
	{
		unit.entity.position += ( unit.entity.velocity * dt );
	}

	void Update( _Unit& unit, float dt, Grid::_Grid& grid )
	{		
		switch( unit.jobstate )
		{
			case JobState::isAttacking:
				HandleAttacking( unit );
				break;
			case JobState::isFarming:
				HandleJob( unit );
				break;
			case JobState::isIdle:
				HandleIdle( unit );
				break;
			case JobState::isPurchasing:
				HandlePurchasing( unit, dt );
				break;
			case JobState::isTransitioning:
				HandleTransitioning( unit, dt );
				break;
		}
	}

}
namespace Game::Units::Fisher
{
	void Update( _Unit& unit, float dt, Grid::_Grid& grid )
	{

	}

}
namespace Game::Units::Gatherer
{
	void Update( _Unit & unit, float dt, Grid::_Grid& grid )
	{}
}
namespace Game::Units::FootSoldier
{
	void Update( _Unit & unit, float dt, Grid::_Grid& grid )
	{}
}
namespace Game::Units::Mounted
{
	void Update( _Unit & unit, float dt, Grid::_Grid& grid )
	{}
}
namespace Game::Units::Naval
{
	void Update( _Unit & unit, float dt, Grid::_Grid& grid )
	{}
}
namespace Game::Units::Scout
{
	void Update( _Unit & unit, float dt, Grid::_Grid& grid )
	{}
}

