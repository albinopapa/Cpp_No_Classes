#pragma once

#include "TextFormat.h"
#include "Colors.h"
#include "Rect.h"
#include <memory>

#ifdef GetCharWidth
#undef GetCharWidth
#endif

class FontSheet
{
public:
	FontSheet() = default;
	FontSheet( const TextFormat &Format );

	Color GetPixel( int X, int Y )const;
	Rectf GetCharRect( const char C )const;
	int GetCharWidth()const;
	int GetCharHeight()const;

private:
	Color GetPixel( int Idx )const;

private:
	int m_nCharsPerRow, m_charWidth, m_charHeight;
	std::unique_ptr<Color[]> m_pPixels;
};
