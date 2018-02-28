#include "Units.h"
#include "Graphics.h"
#include "Framework.h"


namespace Units::Builder
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Farmer
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Fisher
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Gatherer
{
	void Update( _Unit& unit, float dt );
}
namespace Units::FootSoldier
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Mounted
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Naval
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Scout
{
	void Update( _Unit& unit, float dt );
}


namespace Units
{
	_Unit::_Unit( float Attack, float HP, float Armor, Job _Job, Team _Team )
		:
		attack( Attack ), hp( HP ), armor( Armor ), job( _Job ), team( _Team )
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
	}
	_Unit::operator Game::_Entity()const
	{
		return entity;
	}

	void Update( _Unit& unit, float dt )
	{
		if( unit.vtable.Update )
			unit.vtable.Update( unit, dt );

		ClampToScreen( unit );
	}
	void Draw( Graphics::_Graphics& gfx, const _Unit& unit )
	{
		const auto color = unit.team == Team::Red ? 
			Color::_Color{ 255, 0, 0 } : Color::_Color{ 0, 0, 255 };

		Framework::DrawRect(
			gfx,
			unit.position.x,
			unit.position.y,
			unit.rect.right - unit.rect.left,
			unit.rect.bottom - unit.rect.top,
			color
		);
	}
	void SetAttackingUnit( _Unit& source, const _Unit& pAttacker )
	{
		source.p_attacker = &pAttacker;
	}

	bool IsAlive( const _Unit& unit )
	{
		return unit.hp > 0.f;
	}
	void ClampToScreen( _Unit& unit )
	{
		unit.position.x = Math::clamp(
			unit.position.x, 0.f,
			static_cast< float >( Graphics::ScreenWidth )
		);
		unit.position.y = Math::clamp(
			unit.position.y, 0.f,
			static_cast< float >( Graphics::ScreenHeight )
		);
	}
}

namespace Units::Farmer
{
	void HandleIdle( _Unit& unit )
	{
		unit.jobstate = JobState::isPurchasing;
	}
	void HandleTransitioning( _Unit& unit, float dt )
	{
		unit.position += ( unit.velocity * dt );
	}
	void HandleFarming( _Unit& unit )
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
		unit.position += ( unit.velocity * dt );
	}

	void Update( _Unit& unit, float dt )
	{		
		switch( unit.jobstate )
		{
			case JobState::isAttacking:
				HandleAttacking( unit );
				break;
			case JobState::isFarming:
				HandleFarming( unit );
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
namespace Units::Fisher
{
	void Update( _Unit& unit, float dt )
	{

	}

}
namespace Units::Builder
{
	void Update( _Unit& unit, float dt )
	{

	}
}

namespace Units::Gatherer
{
	void Update( _Unit & unit, float dt )
	{}
}

namespace Units::FootSoldier
{
	void Update( _Unit & unit, float dt )
	{}
}

namespace Units::Mounted
{
	void Update( _Unit & unit, float dt )
	{}
}

namespace Units::Naval
{
	void Update( _Unit & unit, float dt )
	{}
}

namespace Units::Scout
{
	void Update( _Unit & unit, float dt )
	{}
}
