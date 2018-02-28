#pragma once

namespace Game
{
	enum class Occupant
	{
		Unit,		// Mobile entities
		Building,	// Constructable entities
		Landscape	// Terrain entities
	};

	struct _Entity
	{
		Occupant occ_type;
	};
}