#pragma once

#include "Color.h"
#include "Direct3D.h"
#include <memory>

namespace Framework
{
	namespace Window
	{
		struct _Window;
	}
	namespace Graphics
	{
		constexpr int32_t ScreenWidth	= 16 * 50;
		constexpr int32_t ScreenHeight	= 16 * 45;

		struct _Graphics
		{
			_Graphics(Window::_Window& window);
			Direct3D::_Direct3D d3d;
			std::unique_ptr<_Color[]> pBuffer;
		};
	}
}