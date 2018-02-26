#include "Keyboard.h"

namespace Keyboard
{
	_Keyboard::Event::Event( State S, uint8_t Keycode )
		:
		state( S ), keycode( Keycode )
	{}

	bool IsPressed( const _Keyboard& keyboard, uint8_t key )
	{
		return keyboard.states[ key ];
	}

	bool HasEvent( const _Keyboard& keyboard )
	{
		return !keyboard.events.empty();
	}

	_Keyboard::Event Read( _Keyboard& keyboard )
	{
		if( keyboard.events.empty() )
			return _Keyboard::Event();

		const auto ev = keyboard.events.front();
		keyboard.events.pop();
		
		return ev;
	}

	void OnPress( _Keyboard& keyboard, const uint8_t key )
	{
		keyboard.events.emplace( _Keyboard::State::KeyPressed, key );
	}

	void OnRelease( _Keyboard& keyboard, const uint8_t key )
	{
		keyboard.events.emplace( _Keyboard::State::KeyReleased, key );
	}
}
