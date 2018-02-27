#include "Mouse.h"

namespace Mouse
{
	Event::Event( State S, int16_t X, int16_t Y )
		:
		x( X ), y( Y ), state( S )
	{}
}