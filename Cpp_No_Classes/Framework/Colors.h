/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Colors.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include <algorithm>
#include <cctype>
#ifdef min
#undef min
#undef max
#endif


class Color
{
public:
	uint32_t dword = 0u;

public:
	constexpr Color() = default;
	constexpr Color( uint32_t dw )
		:
		dword( dw )
	{}
	constexpr Color( uint32_t x, uint32_t r, uint32_t g, uint32_t b )
		:
		dword( ( x << 24u ) | ( r << 16u ) | ( g << 8u ) | b )
	{}
	constexpr Color( uint32_t r, uint32_t g, uint32_t b )
		:
		Color( 255u, r, g, b )
	{}
	constexpr Color( Color col, uint32_t x )
		:
		Color( ( x << 24u ) | ( col.dword & 0x00FFFFFF ) )
	{}
	constexpr uint32_t GetX() const
	{
		return dword >> 24u;
	}
	constexpr uint32_t GetA() const
	{
		return GetX();
	}
	constexpr uint32_t GetR() const
	{
		return ( dword >> 16u ) & 0xFFu;
	}
	constexpr uint32_t GetG() const
	{
		return ( dword >> 8u ) & 0xFFu;
	}
	constexpr uint32_t GetB() const
	{
		return dword & 0xFFu;
	}
	void SetX( uint32_t x )
	{
		dword = ( dword & 0xFFFFFFu ) | ( x << 24u );
	}
	void SetA( uint32_t a )
	{
		SetX( a );
	}
	void SetR( uint32_t r )
	{
		dword = ( dword & 0xFF00FFFFu ) | ( r << 16u );
	}
	void SetG( uint32_t g )
	{
		dword = ( dword & 0xFFFF00FFu ) | ( g << 8u );
	}
	void SetB( uint32_t b )
	{
		dword = ( dword & 0xFFFFFF00u ) | b;
	}

	Color &operator+=( const Color &C )
	{
		*this = *this + C;

		return *this;
	}
	Color &operator*=( uint32_t S )
	{
		*this = *this * S;
		return *this;
	}
	constexpr Color AlphaBlend( Color Src )const
	{
		const auto srcAlpha0 = GetA();
		const auto srcAlpha1 = 255u - srcAlpha0;

		return ( *this * srcAlpha0 ) + ( Src * srcAlpha1 );
	}
	constexpr uint32_t Brightness()const
	{
		const uint32_t b = ( GetR() + GetG() + GetB() ) / 3u;

		return b;
	}
	constexpr bool operator==( const Color C )const
	{
		return dword == C.dword;
	}
	constexpr bool operator!=( const Color C )const
	{
		return !( *this == C );
	}

	friend constexpr Color operator+( const Color, const Color );
	friend constexpr Color operator*( const Color, uint32_t );
};
constexpr Color operator+( const Color leftSide, const Color rightSide )
{
	return {
		std::min( leftSide.GetR() + rightSide.GetR(), 255u ),
		std::min( leftSide.GetG() + rightSide.GetG(), 255u ),
		std::min( leftSide.GetB() + rightSide.GetB(), 255u )
	};
}
constexpr Color operator*( const Color leftSide, uint32_t S )
{
	return {
		( leftSide.GetR() * S ) >> 8u,
		( leftSide.GetG() * S ) >> 8u,
		( leftSide.GetB() * S ) >> 8u
	};
}

namespace Colors
{
	constexpr Color MakeRGB( uint32_t r, uint32_t g, uint32_t b )
	{
		return Color( 255u, r, g, b );
	}
	constexpr Color White = Color( 255u, 255u, 255u );
	constexpr Color Black = Color( 0u, 0u, 0u );
	constexpr Color Gray = Color( 0x80u, 0x80u, 0x80u );
	constexpr Color LightGray = Color( 0xD3u, 0xD3u, 0xD3u );
	constexpr Color Red = Color( 255u, 0u, 0u );
	constexpr Color Green = Color( 0u, 255u, 0u );
	constexpr Color Blue = Color( 0u, 0u, 255u );
	constexpr Color Yellow = Color( 255u, 255u, 0u );
	constexpr Color Cyan = Color( 0u, 255u, 255u );
	constexpr Color Magenta = Color( 255u, 0u, 255u );
	constexpr Color Orange = Color( 255, 128, 64 );
}