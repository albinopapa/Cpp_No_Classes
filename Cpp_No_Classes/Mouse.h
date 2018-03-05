#pragma once

#include <cstdint>
#include <queue>

namespace Framework::Input::Mouse
{
	enum class State
	{
		Idle,
		LeftPressed, LeftReleased,
		MiddlePressed, MiddleReleased,
		RightPressed, RightReleased,
		XButton1Pressed, XButton1Released,
		XButton2Pressed, XButton2Released
	};
	struct Event
	{
		Event() = default;
		Event( State S, int16_t X, int16_t Y );

		State state = State::Idle;
		int16_t x = 0, y = 0;
	};
	struct _Mouse
	{
		int16_t x = 0, y = 0;
		bool leftPressed = false;
		bool rightPressed = false;
		std::queue<Event> events;
	};
}
