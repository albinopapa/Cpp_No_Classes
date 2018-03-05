#include "Keyboard.h"

namespace Framework::Input::Keyboard
{
	Event::Event( State S, uint8_t Keycode )
		:
		state( S ), keycode( Keycode )
	{}
}
