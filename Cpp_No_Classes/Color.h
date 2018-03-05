#pragma once

#include <cstdint>

namespace Framework::Graphics
{
	struct _Color
	{
		constexpr _Color() = default;
		constexpr _Color( uint32_t R, uint32_t G, uint32_t B, uint32_t A = 255 )
			:
			value( ( A << 24 ) | ( R << 16 ) | ( G << 8 ) | B )
		{}
		
		uint32_t value = 255<<24;
	};
}