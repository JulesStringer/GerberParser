#pragma once

#include <string>

class CLayer
{
	std::string m_strName;
	// This is a DXF colour index
	// First few colours are
	// 0	Black
	// 1	Red    (do not use with laser cutter as this is highlight colour)
	// 2    Yellow
	// 3    Green
	// 4    Cyan
	// 5    Blue
	// 6    Purple
	// 7    White
	// 8    Dark grey (0x40)
	// 9    Mid grey (0x80)
	// Following colours are defined here:
	// http://sub-atomic.com/~moses/acadcolors.html
	// The above should be sufficient for most production purposes
	//
	unsigned char m_nColourIndex;
	// Regular expression to match comments against
	std::string m_strRegex;
	bool m_bInclude;
public:
	CLayer();
	CLayer(const char* pszName, unsigned char nColourIndex, const char* pszRegex);
	virtual ~CLayer();
	unsigned char ColourIndex();
	const char* Name();
	virtual bool CommentMatch(const char* psz);
	bool Include();
	void SetInclude(bool bInclude);
};

