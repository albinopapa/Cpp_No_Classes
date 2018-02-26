#pragma once

#include "Color.h"
#include "Direct3D.h"
#include <memory>

namespace Graphics
{
	constexpr int32_t ScreenWidth = 800;
	constexpr int32_t ScreenHeight = 600;

	struct _Graphics
	{
		_Graphics(Window::_Window& window);
		Direct3D::_Direct3D d3d;
		std::unique_ptr<Color::_Color[]> pBuffer;
	};

	void BeginFrame( _Graphics& gfx, Color::_Color C );
	void EndFrame( _Graphics& gfx );

	void DrawRect( _Graphics& gfx, int32_t X, int32_t Y, int32_t Width, int32_t Height, Color::_Color C, bool Filled = true );
	void DrawCircle( _Graphics& gfx, int32_t X, int32_t Y, int32_t Radius, Color::_Color C, bool Filled = true );
	void DrawLine( _Graphics& gfx, int32_t StartX, int32_t StartY, int32_t EndX, int32_t EndY, Color::_Color C );
	
	void PutPixel( _Graphics& gfx, int32_t X, int32_t Y, Color::_Color C );
}