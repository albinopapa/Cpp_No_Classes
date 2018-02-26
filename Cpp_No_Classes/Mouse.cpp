#include "Mouse.h"

namespace Mouse
{
	_Mouse::Event::Event( State S, int16_t X, int16_t Y )
		:
		x( X ), y( Y ), state( S )
	{}

	bool LeftIsPressed( const _Mouse& mouse )
	{
		return mouse.leftPressed;
	}
	bool RightIsPressed( const _Mouse& mouse )
	{
		return mouse.rightPressed;
	}

	bool HasEvent( const _Mouse& mouse )
	{
		return !mouse.events.empty();
	}
	_Mouse::Event Read( _Mouse& mouse )
	{
		if( mouse.events.empty() )
			return _Mouse::Event();

		const auto ev = mouse.events.front();
		mouse.events.pop();

		return ev;
	}

	void OnMove( _Mouse& mouse, int16_t X, int16_t Y )
	{
		mouse.events.emplace( _Mouse::State::Idle, X, Y );
	}
	void OnPress( _Mouse& mouse, int16_t X, int16_t Y, uint8_t btncode )
	{
		const auto left   = btncode & 1;
		const auto right  = btncode & 2;
		const auto middle = btncode & 16;
		const auto xb1	  = btncode & 32;
		const auto xb2	  = btncode & 64;
		if( left )
		{
			mouse.events.emplace( _Mouse::State::LeftPressed, X, Y );
			mouse.leftPressed = true;
		}
		if( middle )
		{
			mouse.events.emplace( _Mouse::State::MiddlePressed, X, Y );
		}
		if( right )			
		{
			mouse.events.emplace( _Mouse::State::RightPressed, X, Y );
			mouse.rightPressed = true;
		}
		if( xb1 )
		{
			mouse.events.emplace( _Mouse::State::XButton1Pressed, X, Y );
		}
		if( xb2 )
		{
			mouse.events.emplace( _Mouse::State::XButton2Pressed, X, Y );
		}
	}
	void OnRelease( _Mouse& mouse, int16_t X, int16_t Y, uint8_t btncode )
	{
		const auto left = btncode & 1;
		const auto right = btncode & 2;
		const auto middle = btncode & 16;
		const auto xb1 = btncode & 32;
		const auto xb2 = btncode & 64;
		if( left )
		{
			mouse.events.emplace( _Mouse::State::LeftReleased, X, Y );
			mouse.leftPressed = false;
		}
		if( middle )
		{
			mouse.events.emplace( _Mouse::State::MiddleReleased, X, Y );
		}
		if( right )
		{
			mouse.events.emplace( _Mouse::State::RightReleased, X, Y );
			mouse.rightPressed = false;
		}
		if( xb1 )
		{
			mouse.events.emplace( _Mouse::State::XButton1Released, X, Y );
		}
		if( xb2 )
		{
			mouse.events.emplace( _Mouse::State::XButton2Released, X, Y );
		}
	}	
}