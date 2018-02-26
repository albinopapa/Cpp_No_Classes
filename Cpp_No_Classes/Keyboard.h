#pragma once

#include <bitset>
#include <cstdint>
#include <queue>

namespace Keyboard
{
	struct _Keyboard
	{
		enum class State
		{
			Idle, KeyPressed, KeyReleased
		};

		struct Event
		{
			Event() = default;
			Event( State S, uint8_t Keycode );

			State state = State::Idle;
			uint8_t keycode = 0;
		};

		std::queue<Event> events;
		std::bitset<256> states;
	};

	bool IsPressed( const _Keyboard& keyboard, uint8_t key );
	bool HasEvent( const _Keyboard& keyboard );

	_Keyboard::Event Read( _Keyboard& keyboard );

	void OnPress( _Keyboard& keyboard, const uint8_t key );
	void OnRelease( _Keyboard& keyboard, const uint8_t key );
}
