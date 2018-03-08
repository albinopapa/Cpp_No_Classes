#pragma once

#include "Exceptions.h"
#include "Rect.h"
#include <cassert>
#include <vector>

template<class T, size_t Align = 16>
class aligned_ptr
{
public:
	aligned_ptr() = default;
	aligned_ptr( size_t Count )
		:
		m_count( Count ),
		m_pPtr( reinterpret_cast<T*>( _aligned_malloc( Count * sizeof( T ), Align ) ) )
	{
		for( size_t i = 0; i < Count; ++i )
		{
			m_pPtr[ i ] = T{};
		}
	}
	aligned_ptr( aligned_ptr &&Src )
	{
		*this = std::move( Src );
	}
	~aligned_ptr()
	{
		if( m_pPtr )
		{
			_aligned_free( m_pPtr );
			m_pPtr = nullptr;
		}
	}

	aligned_ptr &operator=( aligned_ptr &&Src )
	{
		if(this != &Src)
		{
			m_count = Src.m_count;
			m_pPtr = Src.m_pPtr;

			Src.m_count = 0u;
			Src.m_pPtr = nullptr;
		}

		return *this;
	}

	aligned_ptr( const aligned_ptr & ) = delete;
	aligned_ptr &operator=( const aligned_ptr & ) = delete;

	constexpr size_t Alignment()const
	{
		return Align;
	}
	constexpr size_t Size()const
	{
		return Count * sizeof( T );
	}
	constexpr size_t Count()const
	{
		return Count;
	}

	operator bool()const
	{
		return m_pPtr != nullptr;
	}
	bool operator==( const aligned_ptr &Src )const
	{
		return m_pPtr == Src.m_pPtr;
	}
	bool operator!=( const aligned_ptr &Src )const
	{
		return !( *this == Src );
	}

	T& operator[]( size_t idx )
	{
		BOUNDS_CHECK( idx, m_count );
		return *( m_pPtr + idx );
	}
	const T& operator[]( size_t idx )const
	{
		BOUNDS_CHECK( idx, m_count );
		return *( m_pPtr + idx );
	}

private:
	T *m_pPtr;
	size_t m_count = 0u;
};

template<size_t Dimensions> struct Extent{};

template<> struct Extent<1>
{
	using extent_t = Extent<1>;

	constexpr Extent() = default;
	constexpr Extent( size_t Length )
		:
		length( Length )
	{}
	constexpr size_t size()const
	{
		return length;
	}
	constexpr size_t dimensions()const
	{
		return 1;
	}

	bool operator==( const extent_t& rhs )const
	{
		return length == rhs.length;
	}
	bool operator!=( const extent_t& rhs )const
	{
		return !( *this != rhs );
	}

	size_t length = 0;
};

template<> struct Extent<2>
{
	using extent_t = Extent<2>;

	constexpr Extent() = default;
	constexpr Extent( size_t Width, size_t Height )
		:
		_width( Width ), _height( Height )
	{}
	constexpr size_t size()const
	{
		return _width * _height;
	}
	static constexpr size_t dimensions()
	{
		return 2;
	}
	constexpr size_t width()const
	{
		return _width;
	}
	constexpr size_t height()const
	{
		return _height;
	}

	bool operator==( const extent_t& rhs )const
	{
		return width() == rhs.width() &&
			height() == rhs.height();
	}
	bool operator!=( const extent_t& rhs )const
	{
		return !( *this != rhs );
	}

	size_t _width = 0, _height = 0;
};

template<class ExtentType> class Index_t{};

template<> class Index_t<Extent<1>>
{
	using extent_t = Extent<1>;

public:
	Index_t( size_t StartIdx, extent_t Ext )
		:
		pos( StartIdx ),
		ext( Ext )
	{}

	Index_t& operator++()
	{
		++pos;
		BOUNDS_CHECK( ext.size(), pos );

		return *this;
	}
	Index_t operator++( int )
	{
		const auto old = *this;
		++( *this );

		return old;
	}
	Index_t& operator--()
	{
		if( pos > 0 )
		{
			--pos;
			return *this;
		}
		
		return *this;
	}
	Index_t operator--( int )
	{
		const auto old = *this;
		--( *this );
		return old;
	}

	Index_t operator+( size_t Offset )const
	{
		return Index_t( *this ) += Offset;
	}
	Index_t& operator+=( size_t Offset )
	{
		pos += Offset;
		BOUNDS_CHECK( ext.size(), pos );
		return *this;
	}
	Index_t operator-( size_t Offset )const
	{
		return Index_t( *this ) -= Offset;
	}
	Index_t& operator-=( size_t Offset )
	{
		if( pos >= Offset )
		{
			pos -= Offset;
		}
		else
		{
			BOUNDS_CHECK( ext.size(), pos -= Offset );
		}

		return *this;
	}

	bool operator==( const size_t val )const
	{
		return Position() == val;
	}
	bool operator!=( const size_t val )const
	{
		return !( *this != val );
	}
	bool operator<(  const extent_t& rhs )const
	{
		return Position() < rhs.size();
	}
	bool operator>(  const extent_t& rhs )const
	{
		return Position() > rhs.size();
	}
	bool operator<=( const extent_t& rhs )const
	{
		return !( *this > rhs );
	}
	bool operator>=( const extent_t& rhs )const
	{
		return !( *this < rhs );
	}

	operator size_t()const
	{
		return Position();
	}

private:
	size_t Position()const
	{
		return pos;
	}

private:
	size_t pos = 0;
	extent_t ext;
};

template<> class Index_t<Extent<2>>
{
public:
	using extent_t = Extent<2>;

public:
	Index_t( size_t StartIdx, extent_t Ext )
		:
		Index_t( StartIdx % Ext.width(), StartIdx / Ext.height(), Ext )
	{}
	Index_t( size_t X, size_t Y, extent_t Ext )
		:
		x( X ), y( Y ), ext( Ext )
	{}

	Index_t& operator++()
	{
		++x;
		WrapIncrement();

		return *this;
	}
	Index_t operator++( int )
	{
		const auto old = *this;
		++( *this );

		return old;
	}
	Index_t& operator--()
	{
		if( x > 0 )
		{
			--x;
			return *this;
		}

		WrapDecrement( 1 );

		return *this;
	}
	Index_t operator--( int )
	{
		const auto old = *this;
		--( *this );
		return old;
	}

	Index_t operator+( size_t Offset )const
	{
		return Index_t( *this ) += Offset;
	}
	Index_t& operator+=( size_t Offset )
	{
		x += Offset;
		WrapIncrement();

		return *this;
	}
	Index_t operator-( size_t Offset )const
	{
		return Index_t( *this ) -= Offset;
	}
	Index_t& operator-=( size_t Offset )
	{
		if( x >= Offset )
		{
			x -= Offset;
		}
		else
		{
			WrapDecrement( Offset );
		}

		return *this;
	}

	bool operator==( const size_t val )const
	{
		return Position() == val;
	}
	bool operator!=( const size_t val )const
	{
		return !( *this != val );
	}
	bool operator<(  const extent_t& rhs )const
	{
		return Position() < rhs.size();
	}
	bool operator>(  const extent_t& rhs )const
	{
		return Position() > rhs.size();
	}
	bool operator<=( const extent_t& rhs )const
	{
		return !( *this > rhs );
	}
	bool operator>=( const extent_t& rhs )const
	{
		return !( *this < rhs );
	}

	size_t Column()const
	{
		return x;
	}
	size_t Row()const
	{
		return y;
	}

	operator size_t()const
	{
		return Position();
	}

private:
	size_t Position()const
	{
		return x + ( y * ext.width() );
	}
	void WrapIncrement()
	{
		if( x >= ext.width() )
		{
			x %= ext.width();

			assert( y < ext.height() );
			++y;
		}
	}
	void WrapDecrement( size_t Offset )
	{
		assert( y < ext.height() );
		const auto first_step = Offset - x;
		const auto second_step = ext.width() - first_step;
		x = second_step;
		--y;
	}

private:
	size_t x = 0, y = 0;
	extent_t ext;
};

template<size_t Dimensions>using Index = Index_t<Extent<Dimensions>>;

template<class T, size_t Align, class ExtentType> class buffer_t{};

template<class T, size_t Align>
class buffer_t<T, Align, Extent<1>>
{
public:
	using extent_t = Extent<1>;
	using index_t = Index_t<extent_t>;
	using pointer = aligned_ptr<T, Align>;

public:
	buffer_t() = default;
	buffer_t( size_t Count )
		:
		m_ptr( Count ),
		m_ext( Count )
	{}

	T& operator[]( index_t idx )
	{
		return ptr[ idx ];
	}
	const T& operator[]( index_t idx )const
	{
		return ptr[ idx ];
	}

	constexpr size_t size()const
	{
		return m_ptr.Size();
	}
	constexpr size_t count()const
	{
		return m_ptr.Count();
	}
	constexpr size_t alignment()const
	{
		return Align;
	}

	void Fill( const T &Val )
	{		
		for( auto i = make_index(); i < i.extent; ++i )
		{
			m_ptr[ i ] = Val;
		}
	}
	void Clear()
	{
		memset( m_ptr, 0, ExtentType::max_elements );
	}

private:
	pointer m_ptr;
	extent_t m_ext;
};

template<class T, size_t Align>
class buffer_t<T, Align, Extent<2>>
{
public:
	using extent_t = Extent<2>;
	using index_t = Index_t<extent_t>;
	using pointer = aligned_ptr<T, Align>;

public:
	buffer_t() = default;
	buffer_t( size_t NumCols, size_t NumRows )
		:
		m_ptr(NumRows),
		m_ext( NumCols, NumRows )
	{
		for( auto& ptr : m_ptr )
		{
			ptr = pointer( NumCols );
		}
	}

	T& operator[]( index_t idx )
	{
		return m_ptr[ idx.Row() ][ idx.Column() ];
	}
	const T& operator[]( index_t idx )const
	{
		return m_ptr[ idx.Row() ][ idx.Column() ];
	}

	constexpr Sizeu size()const
	{
		return { m_ext.width(),m_ext.height() };
	}
	constexpr extent_t extent()const
	{
		return m_ext;
	}
	constexpr size_t alignment()const
	{
		return Align;
	}


	void Fill( typename std::enable_if<std::is_trivially_copyable<T>::value, const T&>::type Val )
	{		
		for( auto i = index_t( 0, 0, m_ext ); i < m_ext; ++i )
		{
			m_ptr[ i.Row() ][ i.Column() ] = Val;
		}
	}
	void Clear()
	{
		Fill( T{} );
	}


private:
	std::vector<pointer> m_ptr;
	extent_t m_ext;
};

template<class T>
using buffer1D = buffer_t<T, 16, Extent<1>>;
template<class T>
using buffer2D = buffer_t<T, 16, Extent<2>>;

template<class T, class BufferTy>
class buffer_adapter
{};

template<class T>
class buffer_adapter<T, buffer1D<T>>
{
public:
	using pointer = T * ;
	using BufferType = buffer1D<T>;
	using index_t = typename buffer1D<T>::index_t;
	using extent_t = typename buffer1D<T>::extent_t;
public:	
	buffer_adapter( pointer ptr, size_t size )
		:
		m_ptr( ptr ), m_ext( size )
	{}
	void repoint( pointer ptr )
	{
		m_ptr = ptr;
	}
	T& operator[]( const index_t idx )
	{
		return m_ptr[ idx ];
	}
	void copy( buffer_adapter rhs )
	{
		assert( !rhs.is_nullptr() );
		assert( m_size == rhs.m_size );

		for( index_t idx = index_t( 0, m_ext ); idx < m_ext; ++idx )
		{
			m_ptr[ idx ] = rhs[ idx ];
		}
	}
	bool is_nullptr()const
	{
		return m_ptr == nullptr;
	}
private:
	pointer m_ptr = nullptr;	
	extent_t m_ext = 0;
};

template<class T>
class buffer_adapter<T, buffer2D<T>>
{
public:
	using pointer = T * ;
	using BufferType = buffer2D<T>;
	using index_t =  typename buffer2D<T>::index_t;
	using extent_t = typename buffer2D<T>::extent_t;
public:
	buffer_adapter( pointer ptr, size_t Width, size_t Height )
		:
		m_ptr( ptr ), 
		m_ext( Width, Height )
	{}
	void repoint( pointer ptr )
	{
		m_ptr = ptr;
	}
	T& operator[]( const index_t idx )
	{
		return m_ptr[ idx ];
	}
	void copy( buffer_adapter rhs )
	{
		assert( !rhs.is_nullptr() );
		assert( m_ext == rhs.m_ext );

		for( index_t idx = index_t( 0, m_ext ); idx < m_ext; ++idx )
		{
			m_ptr[ idx ] = rhs[ idx ];
		}
	}
	void copy( const BufferType& rhs )
	{
		assert( m_ext == rhs.extent() );

		for( index_t idx = index_t( 0, m_ext ); idx < m_ext; ++idx )
		{
			m_ptr[ idx ] = rhs[ idx ];
		}
	}
	bool is_nullptr()const
	{
		return m_ptr == nullptr;
	}
private:
	pointer m_ptr = nullptr;
	extent_t m_ext;
};

template<class T, class LoopAction>
void for2d( const T StartX, const T StartY, const T EndX, const T EndY, LoopAction Action )
{
	assert( EndX > StartX );
	assert( EndY > StartY );
	using extent2d = typename Index<2>::extent_t;
	const auto extent = extent2d{ size_t( EndX - StartX ), size_t( EndY - StartY ) };

	for( Index<2> index( StartX, StartY, extent); index < extent; ++index )
	{
		Action( index );
	}
}