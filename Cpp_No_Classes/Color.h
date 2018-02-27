#pragma once

#include <cstdint>

namespace Color
{
	struct _Color
	{
		constexpr _Color() = default;
		constexpr _Color( uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255 )
			:
			value( ( A << 24 ) | ( R << 16 ) | ( G << 8 ) | B )
		{}
		
		uint32_t value = 255<<24;
	};
}