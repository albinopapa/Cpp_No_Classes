#include "Entity.h"

namespace Game
{
	Vec2f GetPosition( const _Entity& entity )
	{
		return entity.position;
	}
	Vec2f GetVelocity( const _Entity& entity )
	{
		return entity.velocity;
	}
	void SetPosition( _Entity& entity, const Vec2f& Position )
	{
		entity.position = Position;
	}
	void SetVelocity( _Entity& entity, const Vec2f& Velocity )
	{
		entity.velocity = Velocity;
	}

}