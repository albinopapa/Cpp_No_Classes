#include "Graphics.h"
#include <cassert>

Framework::Graphics::_Graphics::_Graphics( Framework::Window::_Window& window )
	:
	d3d( window ),
	pBuffer( std::make_unique<Graphics::_Color[]>( ScreenWidth*ScreenHeight ) )
{}
