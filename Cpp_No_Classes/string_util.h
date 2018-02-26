#pragma once

#include <string>
#include <sstream>

namespace String
{
#if defined(UNICODE) || defined(_UNICODE)
	using string_type = std::wstring;
	using stream_type = std::wstringstream;
	using cstring_type = wchar_t*;
#else
	using string_type = std::string;
	using stream_type = std::stringstream;
	using cstring_type = char*;
#endif
	template<class T = void> struct is_char_type :std::false_type
	{};
	template<> struct is_char_type<char> :std::true_type{};
	template<> struct is_char_type<wchar_t> : std::true_type{};

	template<class T> size_t length( const T* str )
	{
		static_assert( is_char_type<T>::value, "Function only works with const char * or const wchar_t * types." );
		if constexpr( std::is_same_v<T,char> )
		{
			return strlen( str );
		}
		else
		{
			return wcslen( str );
		}
	}

	using cstring = cstring_type;
	using string = typename string_type;
	using stringstream = typename stream_type;
	
	string encode( const char*str)
	{
		const auto len = length( str ) + 1;
		string res = string( str, str + len );

		return res;
	}
	string encode( const std::string& str )
	{
		return string( str.begin(), str.end() );
	}
	string encode( const wchar_t* str )
	{
		const auto len = length( str ) + 1;
		string res = string( str, str + len );

		return res;
	}
	string encode( const std::wstring& str )
	{
		return string( str.begin(), str.end() );
	}
	
}