#pragma once

#include <bitset>
#include <cstdint>
#include <queue>

namespace Keyboard
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

	struct _Keyboard
	{
		std::queue<Event> events;
		std::bitset<256> states;
	};
}
