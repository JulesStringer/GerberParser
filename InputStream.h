#pragma once

#include <string>

class CInputStream
{
	std::string m_strText;
	unsigned int m_nChar;
	double m_dXFactor;
	double m_dYFactor;
	double m_dUnits;
public:
	CInputStream();
	~CInputStream();
	void Set(const char* pszText);
	const char PeekChar();
	const char GetChar();
	double GetSignedDouble();
	unsigned int GetInt();
	bool AtEOF();
	bool AtEOL();
	std::string GetExtendedCommand();
	bool Terminator(const char* pszTerminator);
	void SkipToEndBlock();
	void SkipToEndExtended();
	void SkipToNextLine();
	void SkipWhiteSpace();
	double GetXCoordinate();
	double GetYCoordinate();
	double GetSize();
	void SetXFactor(double d);
	void SetYFactor(double d);
	void GetCoordinateFormat();
	void SetUnits(double d);
	bool AtEndExtended();
	bool GetMatching(const char* pszMatch);
};

