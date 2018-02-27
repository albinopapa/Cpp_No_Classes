#include "Keyboard.h"

namespace Keyboard
{
	Event::Event( State S, uint8_t Keycode )
		:
		state( S ), keycode( Keycode )
	{}
}
