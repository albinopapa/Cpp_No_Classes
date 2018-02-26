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

	constexpr uint8_t GetA( _Color C ){return ( C.value >> 24 ) & 0xFF;}
	constexpr uint8_t GetR( _Color C ){return ( C.value >> 16 ) & 0xFF;}
	constexpr uint8_t GetG( _Color C ){return ( C.value >> 8 ) & 0xFF;}
	constexpr uint8_t GetB( _Color C ){return ( C.value & 0xFF );}

	inline void SetA( _Color &C, uint8_t Value ){C.value = ( C.value & 0x00FFFFFF ) | ( Value << 24 );}
	inline void SetR( _Color &C, uint8_t Value ){C.value = ( C.value & 0xFF00FFFF ) | ( Value << 16 );}
	inline void SetG( _Color &C, uint8_t Value ){C.value = ( C.value & 0xFFFF00FF ) | ( Value << 8 );}
	inline void SetB( _Color &C, uint8_t Value ){C.value = ( C.value & 0xFFFFFF00 ) | ( Value );}

}