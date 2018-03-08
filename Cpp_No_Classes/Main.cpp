#include "Game.h"
#include "string_util.h"
#include "Framework.h"

INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, INT )
{
	Beep( 750, 300 );
	try
	{
		auto instance = GetModuleHandle( nullptr );
		auto args = GetCommandLine();
		MainWindow window( instance, String::encode( args ).c_str() );
		Game::_Game game( window );

		while( window.ProcessMessage() )
		{
			Game::Go( game );
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

