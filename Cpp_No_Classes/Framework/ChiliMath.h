/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	ChiliMath.h																			  *
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

#include <cmath>
#include <algorithm>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;
constexpr float absMask = -0.0f;
constexpr float PIDiv2 = PI * .5f;
constexpr float deg2rad = PI / 180.f;
constexpr float rad2deg = 180.f / PI;

template <typename T>
inline auto sq( const T& x )
{
	return x * x;
}

template<typename T>
inline T wrap_angle( T theta )
{
	const T modded = fmod( theta, ( T )2.0 * ( T )PI_D );
	return ( modded > ( T )PI_D ) ?
		( modded - ( T )2.0 * ( T )PI_D ) : modded;
}

template<class T>
T Clamp( const T &Value, const T &MinValue, const T &MaxValue )
{
	return std::max( MinValue, std::min( MaxValue, Value ) );
}

constexpr float ToRadians( float A )
{
	return deg2rad * A;
}

constexpr float ToDegrees( float A )
{
	return rad2deg * A;
}