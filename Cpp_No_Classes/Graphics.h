#pragma once

#include "Color.h"
#include "Direct3D.h"
#include <memory>

namespace Window{ struct _Window; }

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

}