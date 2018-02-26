#pragma once

#include <cstdint>
#include <queue>

namespace Mouse
{
	struct _Mouse
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

		int16_t x = 0, y = 0;

		bool leftPressed = false, rightPressed = false;
		std::queue<Event> events;
	};

	bool LeftIsPressed( const _Mouse& mouse );
	bool RightIsPressed( const _Mouse& mouse );

	bool HasEvent( const _Mouse& mouse );
	_Mouse::Event Read( _Mouse& mouse );

	void OnMove( _Mouse& mouse,    int16_t X, int16_t Y );
	void OnPress( _Mouse& mouse,   int16_t X, int16_t Y, uint8_t btncode );
	void OnRelease( _Mouse& mouse, int16_t X, int16_t Y, uint8_t btncode );
}
