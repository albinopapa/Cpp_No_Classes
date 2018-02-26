#include "Game.h"
#include "Window.h"

namespace Game
{
	_Game::_Game( Window::_Window& _window )
		:
		window( _window ),
		gfx( _window )
	{}

	void Go( _Game& game )
	{
		Graphics::BeginFrame( game.gfx, { 0,0,0 } );
		UpdateModel( game );
		ComposeFrame( game );
		Graphics::EndFrame( game.gfx );
	}
	void UpdateModel( _Game& game )
	{

	}
	void ComposeFrame( _Game& game )
	{
		Graphics::DrawRect( game.gfx, 100, 100, 30, 30, { 0, 255, 0 } );
	}
}