#include "Window.h"
#include "Game.h"
#include "string_util.h"
#include "Framework.h"

INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, INT )
{
	try
	{
		Framework::Window::_Window window;
		Game::_Game game( window );

		while( !window.wants_to_quit )
		{
			Game::Go( game );
			Framework::Window::ProcessMessages( window );
		}
	}
	catch( const std::exception& e)
	{
		const auto msg = String::encode( e.what() );
		const auto title = String::encode( "Something went wrong" );
		MessageBox( nullptr, msg.c_str(), title.c_str(), MB_OK );

		return -1;
	}

	return 0;
}

