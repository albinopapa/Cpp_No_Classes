#pragma once

#include <cassert>
#include <stdexcept>
#include <string>
#include <sstream>

class Out_of_bounds : std::exception
{
public:
	Out_of_bounds( std::string Filename, const int LineNum )
		:
		m_msg( "Index is out of bounds." ),
		m_filename( std::move( Filename ) ),
		m_linenum( LineNum )
	{}

	char const* what() const final
	{
		std::stringstream ss;
		ss << m_msg
			<< '\n'
			<< "Filename: "
			<< m_filename
			<< " Line number: "
			<< m_linenum
			<< std::endl;

		return ss.str().c_str();
	}
private:
	std::string m_msg, m_filename;
	int m_linenum = 0;
};

#if defined( DEBUG )|| defined (_DEBUG)
#define BOUNDS_CHECK(idx,size)if( (idx) > (size) )throw Out_of_bounds(__FILE__,__LINE__) 
#else
#define BOUNDS_CHECK(size,idx)
#endif
