#pragma once

#include "Framework.h"

enum class Team
{
	Red, Blue
};

namespace Game
{
	enum class EntityCategory
	{
		Unit,		// Mobile entities			( farmer, foot soldier, etc.. )
		Building,	// Constructable entities	( House, Wall, Barracks, etc... )
		Resource,	// Gaterable entities		( Farms, Fish, Trees, etc... )
		Landscape	// Terrain entities			( Cliffs, Water, Boulders, etc... )
	};	

	struct _Entity
	{
		EntityCategory category;
		Vec2f position = { 400.f,300.f };
		Vec2f velocity = { 0.f,0.f };
		Team team = Team::Red;
	};

	Vec2f GetPosition( const _Entity& entity );
	Vec2f GetVelocity( const _Entity& entity );
	void SetPosition( _Entity& entity, const Vec2f& Position );
	void SetVelocity( _Entity& entity, const Vec2f& Velocity );
}