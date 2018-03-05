#include "Units.h"
#include "Graphics.h"
#include "Grid.h"
#include "Framework.h"
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
	namespace Graphics = Framework::Graphics;

	_Unit::_Unit( float Attack, float HP, float Armor, Job _Job, Team _Team )
		:
		attack( Attack ), hp( HP ), armor( Armor ), job( _Job )
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

	void Update( _Unit& unit, float dt, Grid::_Grid& grid )
	{
		assert( unit.vtable.Update );

		unit.vtable.Update( unit, dt, grid );
		ClampToScreen( unit );
	}
	void ClampToScreen( _Unit& unit )
	{
		unit.entity.position.x = Math::clamp(
			unit.entity.position.x, static_cast< float >( unit.rect.right ),
			static_cast< float >( Graphics::ScreenWidth + unit.rect.left )
		);
		unit.entity.position.y = Math::clamp(
			unit.entity.position.y, static_cast< float >( unit.rect.bottom ),
			static_cast< float >( Graphics::ScreenHeight + unit.rect.top )
		);
	}
	void Draw( Graphics::_Graphics& gfx, const _Unit& unit )
	{
		const auto color = unit.entity.team == Team::Red ?
			Graphics::_Color{ 255, 0, 0 } : Graphics::_Color{ 0, 0, 255 };

		auto rect = Math::Rect::Translate( unit.rect, Units::GetPosition( unit ) );

		Graphics::DrawRect(
			gfx,
			rect.left,
			rect.top,
			rect.right -  rect.left,
			rect.bottom - rect.top,
			color
		);
	}

	Vec2f GetPosition( const _Unit& unit )
	{
		return Game::GetPosition( unit.entity );
	}
	Job GetJob( const _Unit& unit )
	{
		return unit.job;
	}
	bool IsAlive( const _Unit& unit )
	{
		return unit.hp > 0.f;
	}
	bool IsIdle( const _Unit& unit )
	{
		return unit.jobstate == JobState::isIdle;
	}

	void SetAttackingUnit( _Unit& source, const _Unit& pAttacker )
	{
		source.p_attacker = &pAttacker;
	}
	void SetPosition( _Unit& unit, const Vec2f position )
	{
		unit.entity.position = position;
	}
}

// Derived class definitions
namespace Game::Units::Builder
{
	void HandleIdle( _Unit& unit )
	{
		unit.jobstate = JobState::isPurchasing;
	}
	void HandleTransitioning( _Unit& unit, float dt )
	{
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
	void HandlePurchasing( _Unit& unit, float dt )
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
				HandlePurchasing( unit, dt );
				break;
			case JobState::isTransitioning:
				HandleTransitioning( unit, dt );
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

// Building definitions
namespace Game::Buildings
{
	template<Type t> _Building CreateBuilding( Team _team )
	{
		return _Building (
			traits<t>::fire_resistance,
			traits<t>::melee_resistance,
			traits<t>::projectile_resistance,
			traits<t>::width,
			traits<t>::height,
			traits<t>::max_hp,
			_team,
			t
		);
	}
	_Building::_Building( Team _team, Type _type )
	{
		switch( _type )
		{
			case Buildings::Type::Hut:
				*this = CreateBuilding<Type::Hut>( _team );
				break;
			case Buildings::Type::House:
				*this = CreateBuilding<Type::House>( _team );
				break;
			case Buildings::Type::Mansion:
				*this = CreateBuilding<Type::Mansion>( _team );
				break;
			case Buildings::Type::Tent:
				*this = CreateBuilding<Type::Tent>( _team );
				break;
			case Buildings::Type::Butcher:
				*this = CreateBuilding<Type::Butcher>( _team );
				break;
			case Buildings::Type::Market:
				*this = CreateBuilding<Type::Market>( _team );
				break;
			case Buildings::Type::Warehouse:
				*this = CreateBuilding<Type::Warehouse>( _team );
				break;
			case Buildings::Type::Barracks:
				*this = CreateBuilding<Type::Barracks>( _team );
				break;
			case Buildings::Type::Stable:
				*this = CreateBuilding<Type::Stable>( _team );
				break;
			case Buildings::Type::Port:
				*this = CreateBuilding<Type::Port>( _team );
				break;
			default:
				break;
		}
				
	}
	_Building::_Building( 
		int32_t _fire_resistance,
		int32_t _melee_resistance,
		int32_t _projectile_resistance,
		int32_t _width,
		int32_t _height,
		int32_t _hp,
		Team _team,
		Type _type )
	{
	}

	_Building::operator _Entity&()
	{
		return entity;
	}
	_Building::operator const _Entity&( )const
	{
		return entity;
	}

}