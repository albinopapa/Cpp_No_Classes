#include "Graphics.h"
#include <cassert>

Graphics::_Graphics::_Graphics( Window::_Window& window )
	:
	d3d( window ),
	pBuffer( std::make_unique<Color::_Color[]>( ScreenWidth*ScreenHeight ) )
{}
