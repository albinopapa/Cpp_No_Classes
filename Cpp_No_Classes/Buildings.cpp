#include "Buildings.h"
#include "Units.h"

// Building definitions
namespace Game::Buildings
{
	template<Type t> _Building CreateBuilding( Team _team )
	{
		return _Building(
			traits<t>::fire_resistance,
			traits<t>::melee_resistance,
			traits<t>::projectile_resistance,
			traits<t>::width,
			traits<t>::height,
			traits<t>::max_hp,
			traits<t>::num_occupants,
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
		int32_t num_occupants, 
		Team _team,
		Type _type )
	{}

	_Building::operator _Entity&( )
	{
		return entity;
	}
	_Building::operator const _Entity&( )const
	{
		return entity;
	}

}