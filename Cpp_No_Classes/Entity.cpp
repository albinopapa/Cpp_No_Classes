#include "Entity.h"

namespace Game
{
	Math::Vector::_Vec2<float> GetPosition( const _Entity& entity )
	{
		return entity.position;
	}
	Math::Vector::_Vec2<float> GetVelocity( const _Entity& entity )
	{
		return entity.velocity;
	}
}