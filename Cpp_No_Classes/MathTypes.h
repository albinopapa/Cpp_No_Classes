#pragma once

#include <type_traits>

namespace Math::Vec2
{
	template<class T>
	struct _Vec2
	{
		static_assert( std::is_arithmetic_v<T>, "T must be arithmatic type." );

		_Vec2() = default;
		_Vec2( T X, T Y )
			:
			x( X ), y( Y )
		{}

		T x = 0, y = 0;
	};
}


namespace Math::Rect
{
	template<class T>
	struct _Rect
	{
		static_assert( std::is_arithmetic_v<T>, "T must be arithmatic type." );

		_Rect() = default;
		_Rect( T Left, T Top, T Right, T Bottom )
			:
			left( Left ), top( Top ), right( Right ), bottom( Bottom )
		{}

		T left = 0, top = 0, right = 0, bottom = 0;
	};

}

namespace Math
{
	template<class T> 
	std::enable_if_t< std::is_arithmetic_v<T>, Vec2::_Vec2<T> >operator+(
		const Vec2::_Vec2<T>& lhs, const Vec2::_Vec2<T>& rhs )
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y };
	}
	template<class T> std::enable_if_t<std::is_arithmetic_v<T>, Vec2::_Vec2<T>>operator-(
		const Vec2::_Vec2<T>& lhs, const Vec2::_Vec2<T>& rhs )
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y };
	}
	template<class T> std::enable_if_t<std::is_arithmetic_v<T>, Vec2::_Vec2<T>>operator*(
		const Vec2::_Vec2<T>& lhs, const T& rhs )
	{
		return { lhs.x * rhs, lhs.y * rhs };
	}

	template<class T>
	bool Overlaps( const Rect::_Rect<T>& r1, const Rect::_Rect<T>& r2 )
	{
		return
			r1.left < r2.right && r1.right > r2.left &&
			r1.top < r2.bottom && r1.bottom > r2.top;
	}
	template<class T>
	bool Contains( const Rect::_Rect<T>& rect )
	{

	}

}