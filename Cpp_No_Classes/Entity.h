#pragma once

#include "MathTypes.h"

enum class Team
{
	Red, Blue
};

namespace Buildings
{
	struct _Building;
}
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
		Math::Vector::_Vec2<float> position = { 400.f,300.f };
		Math::Vector::_Vec2<float> velocity = { 0.f,0.f };
		Team team = Team::Red;
	};

	Math::Vector::_Vec2<float> GetPosition( const _Entity& entity );
	Math::Vector::_Vec2<float> GetVelocity( const _Entity& entity );
}