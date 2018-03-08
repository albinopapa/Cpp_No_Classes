#pragma once

#include <string>
#include <sstream>

namespace String
{
#if defined(UNICODE) || defined(_UNICODE)
	using string = std::wstring;
	using stringstream = std::wstringstream;
	using cstring = wchar_t*;
#else
	using string = std::string;
	using stringstream = std::stringstream;
	using cstring = char*;
#endif

	template<class T = void> struct is_char_type :std::false_type{};
	template<> struct is_char_type<char> :std::true_type{};
	template<> struct is_char_type<wchar_t> : std::true_type{};
	
	template<class T>
	std::enable_if_t<is_char_type<T>::value,size_t> 
		length( const T* str )
	{
		static_assert( is_char_type<T>::value, 
			"Function only works with const char * or const wchar_t * types." );
		if constexpr( std::is_same_v<T,char> )
		{
			return strlen( str );
		}
		else
		{
			return wcslen( str );
		}
	}

	namespace details
	{
#if defined(UNICODE) or defined(_UNICODE)
		inline string convert( const std::string& str )
		{
			return string( str.begin(), str.end() );
		}
		inline string convert( std::wstring&& str )
		{
			return std::move( str );
		}
#else
		inline string convert( const std::wstring& str )
		{
			return string( str.begin(), str.end() );
		}
		inline std::string convert( std::string&& str )
		{
			return std::move( str );
		}
#endif
	}

	template<class T>
	std::enable_if_t<is_char_type<T>::value, string>
		encode( const T* str )
	{
		const auto len = length( str ) + 1;
		string res = string( str, str + len );

		return res;
	}

	template<class T>
	string encode( T&& str )
	{
		return details::convert( std::forward<T>( str ) );
	}
}