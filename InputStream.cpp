#include "InputStream.h"



CInputStream::CInputStream()
	: m_nChar(0)
	, m_dXFactor(1)
	, m_dYFactor(1)
	, m_dUnits(1)
{
}


CInputStream::~CInputStream()
{
}
void CInputStream::Set(const char* pszText)
{
	m_strText = pszText;
}
const char CInputStream::PeekChar()
{
	return m_strText[m_nChar];
}
const char CInputStream::GetChar()
{
	return m_strText[m_nChar++];
}
double CInputStream::GetSignedDouble()
{
	double d = 0;
	bool bSign = false;
	if (m_strText[m_nChar] == '+')
	{
		m_nChar++;
	}
	else if (m_strText[m_nChar] == '-')
	{
		bSign = true;
		m_nChar++;
	}
	while (m_nChar < m_strText.length() && isdigit(m_strText[m_nChar]))
	{
		d = d * 10 + m_strText[m_nChar] - '0';
		m_nChar++;
	}
	if (m_strText[m_nChar] == '.')
	{
		double dFactor = 0.1;
		m_nChar++;
		while (m_nChar < m_strText.length() && isdigit(m_strText[m_nChar]))
		{
			d += (m_strText[m_nChar] - '0') * dFactor;
			dFactor *= 0.1;
			m_nChar++;
		}
	}
	if (bSign)
	{
		d = -d;
	}
	return d;
}
double CInputStream::GetXCoordinate()
{
	return GetSignedDouble() * m_dXFactor * m_dUnits;
}
double CInputStream::GetYCoordinate()
{
	return GetSignedDouble() * m_dYFactor * m_dUnits;
}
double CInputStream::GetSize()
{
	return GetSignedDouble() * m_dUnits;
}
void CInputStream::SetXFactor(double d)
{
	m_dXFactor = d;
}
void CInputStream::SetYFactor(double d)
{
	m_dYFactor = d;
}
void CInputStream::SetUnits(double d)
{
	m_dUnits = d;
}
unsigned int CInputStream::GetInt()
{
	unsigned int n = 0;
	while (m_nChar < m_strText.length() && isdigit(m_strText[m_nChar]))
	{
		n = n * 10 + m_strText[m_nChar] - '0';
		m_nChar++;
	}
	return n;
}
bool CInputStream::AtEOF()
{
	return m_nChar >= m_strText.length();
}
bool CInputStream::AtEOL()
{
	return m_nChar >= m_strText.length() || m_strText[m_nChar] == '\r' || m_strText[m_nChar] == '\n';
}
bool CInputStream::AtEndExtended()
{
	return strncmp(m_strText.c_str() + m_nChar, "*%", 2) == 0;
}
std::string CInputStream::GetExtendedCommand()
{
	std::string str = "";
	str += GetChar();
	str += GetChar();
	return str;
}
bool CInputStream::Terminator(const char* pszTerminator)
{
	return AtEOF() || strncmp(m_strText.c_str() + m_nChar, pszTerminator, strlen(pszTerminator)) == 0;
}
bool CInputStream::GetMatching(const char* pszMatch)
{
	if (strncmp(m_strText.c_str() + m_nChar, pszMatch, strlen(pszMatch)) == 0)
	{
		m_nChar += strlen(pszMatch);
		return true;
	}
	return false;
}
void CInputStream::SkipToEndBlock()
{
	while (!AtEOF() && m_strText[m_nChar] != '*')
	{
		m_nChar++;
	}
}
void CInputStream::SkipToEndExtended()
{
	while (!AtEOF() && m_strText[m_nChar] != '%')
	{
		SkipToEndBlock();
		if (m_strText[m_nChar] == '*')
		{
			m_nChar++;
		}
	}
	if (m_strText[m_nChar] == '%')
	{
		m_nChar++;
	}
}
void CInputStream::SkipToNextLine()
{
	while (!AtEOF() && m_strText[m_nChar] != '\r' && m_strText[m_nChar] != '\n' )
	{
		m_nChar++;
	}
	while (!AtEOF() && (m_strText[m_nChar] == '\r' || m_strText[m_nChar] == '\n') )
	{
		m_nChar++;
	}
}
void CInputStream::SkipWhiteSpace()
{
	while (!AtEOF() && m_strText[m_nChar] <= ' ')
	{
		m_nChar++;
	}
}
void CInputStream::GetCoordinateFormat()
{
	unsigned char ch = GetChar();
	ch = GetChar();
	ch = GetChar();
	ch = GetChar();
	if (!isdigit(ch))
	{
		throw "Invalid X digits";
	}
	ch = GetChar();
	if (isdigit(ch))
	{
		double d = 1;
		int nDecimal = ch - '0';
		while (nDecimal > 0)
		{
			d *= 0.1;
			nDecimal--;
		}
		SetXFactor(d);
	}
	else
	{
		throw "Invalid X decimals";
	}
	ch = GetChar();
	if (ch != 'Y')
	{
		throw "%FS Y missing";
	}
	ch = GetChar();
	if (!isdigit(ch))
	{
		throw "Invalid Y digits";
	}
	ch = GetChar();
	if (isdigit(ch))
	{
		double d = 1;
		int nDecimal = ch - '0';
		while (nDecimal > 0)
		{
			d *= 0.1;
			nDecimal--;
		}
		SetYFactor(d);
	}
	else
	{
		throw "Invalid Y decimals";
	}
	SkipToEndExtended();
}
