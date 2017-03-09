#include "WKBGeometry.h"
#include "InputStream.h"


CWKBGeometry::CWKBGeometry()
	: m_nLinearRing(0)
	, m_nPolygon(0)
	, m_nGeometry(0)
{
}


CWKBGeometry::~CWKBGeometry()
{
}
void CWKBGeometry::Add(unsigned char* pdata, int nLen)
{
	int n = m_vecData.size();
	m_vecData.resize(nLen + n);
	memcpy(m_vecData.data() + n, pdata, nLen);
}
void CWKBGeometry::AddPoint(GM_POINT* pPt)
{
	Add((unsigned char*)pPt, sizeof(GM_POINT));
	if (m_nLinearRing > 0)
	{
		GM_LinearRing* pRing = (GM_LinearRing*)(m_vecData.data() + m_nLinearRing);
		pRing->numPoints++;
	}
}
int CWKBGeometry::Type()
{
	if (m_vecData.size() >= sizeof(WKB))
	{
		WKB* pWKB = (WKB*)m_vecData.data();
		return pWKB->wkbType;
	}
	return 0;
}
unsigned char* CWKBGeometry::Data() { return m_vecData.data(); }
#if 0
void CWKBGeometry::Parse(const char* pszWKT)
{
	int nState = 0;
	CInputStream stream;
	stream.Set(pszWKT);
	while( !stream.AtEOL())
	{
		stream.SkipWhiteSpace();
		switch (nState)
		{
		case 0:
			if (stream.GetMatching("POINT"))
			{
				nState = 3;
				WKB wkb;
				wkb.byteOrder = wkbNDR;
				wkb.wkbType = wkbPoint;
				Add((unsigned char*)&wkb, sizeof(wkb));
				break;
			}
			else if (stream.GetMatching("LINESTRING"))
			{
				nState = 2;
				WKB wkb;
				wkb.byteOrder = wkbNDR;
				wkb.wkbType = wkbLineString;
				Add((unsigned char*)&wkb, sizeof(WKB));
				m_nLinearRing = m_vecData.size();
				break;
			}
			else if (stream.GetMatching("POLYGON"))
			{
				nState = 1;
				WKBPolygon Poly;
				Poly.byteOrder = wkbNDR;
				Poly.wkbType = wkbPolygon;
				Poly.numRings = 0;
				Add((unsigned char*)&Poly, sizeof(WKB) + sizeof(ULONG));
				m_nPolygon = m_vecData.size() - sizeof(WKB) - sizeof(ULONG);
				break;
			}
			else
			{
				throw "CWKBGeometry::Parse Unsupported geometry type";
			}
			break;
		case 1:
			// Look for opening bracket of polygon
			if (stream.GetMatching("("))
			{
				nState = 2;
			}
			else
			{
				throw "Opening bracket expected for next polygon";
			}
			break;
		case 2:
			// Look for opening bracket of coordinate list
			if ( stream.GetMatching("("))
			{
				nState = 3;
				if (Type() == wkbPolygon)
				{
					// increment rings
					WKBPolygon* pPoly = (WKBPolygon*)(m_vecData.data() + m_nPolygon);
					pPoly->numRings++;
					m_nLinearRing = m_vecData.size();
				}
				// Add on point count
				ULONG n = 0;
				Add((unsigned char*)&n, sizeof(ULONG));
			}
			else
			{
				throw "Opening bracket of coordinate list expected";
			}
			break;
		case 3:
			// look for coordinate pair or closing bracket
			{
				GM_POINT Pt;
				Pt.x = stream.GetSignedDouble();
				stream.SkipWhiteSpace();
			    Pt.y = stream.GetSignedDouble();
				stream.SkipWhiteSpace();
				AddPoint(&Pt);
				if (stream.GetMatching(","))
				{
					// another point expected
					if (Type() == wkbPoint)
					{
						throw "Point should not have , in coordinate list";
					}
				}
				else if (stream.GetMatching(")"))
				{
					// End of coordinates
					switch (Type())
					{
					case wkbPoint:
						nState = 5;
						break;
					case wkbLineString:
						nState = 5;
						break;
					case wkbPolygon:
						nState = 4;
						break;
					}
				}
				else
				{
					throw("Badly terminated coordinate list");
				}
			}
			break;
		case 4:
			if (stream.GetMatching(")"))
			{
				// End of polygon
				nState = 5;
			}
			else if (stream.GetMatching(","))
			{
				// Next ring
				nState = 2;
			}
			else
			{
				throw "( or , expected";
			}
			break;
		case 5:
			// Next geometry
			if (stream.GetMatching(","))
			{

			}
			else
			{
				if (stream.AtEOF())
				{
					throw "End of geometry expected";
				}
			}
		}
	}
}
#endif
void CWKBGeometry::Parse(const char* pszWKT)
{
	int nState = 0;
	CInputStream stream;
	stream.Set(pszWKT);
	stream.SkipWhiteSpace();
	if (stream.GetMatching("POINT"))
	{
		nState = 3;
		WKB wkb;
		wkb.byteOrder = wkbNDR;
		wkb.wkbType = wkbPoint;
		Add((unsigned char*)&wkb, sizeof(wkb));
	}
	else if (stream.GetMatching("LINESTRING"))
	{
		nState = 2;
		WKB wkb;
		wkb.byteOrder = wkbNDR;
		wkb.wkbType = wkbLineString;
		Add((unsigned char*)&wkb, sizeof(WKB));
		m_nLinearRing = m_vecData.size();
	}
	else if (stream.GetMatching("POLYGON"))
	{
		nState = 1;
		WKBPolygon Poly;
		Poly.byteOrder = wkbNDR;
		Poly.wkbType = wkbPolygon;
		Poly.numRings = 0;
		Add((unsigned char*)&Poly, sizeof(WKB) + sizeof(ULONG));
		int nPolygon = m_vecData.size() - sizeof(WKB) - sizeof(ULONG);
		stream.SkipWhiteSpace();
		if (stream.GetMatching("("))
		{
			do
			{
				stream.SkipWhiteSpace();
				if (stream.GetMatching("("))
				{
					ULONG n = 0;
					WKBPolygon* pPoly = (WKBPolygon*)(m_vecData.data() + nPolygon);
					pPoly->numRings++;
					int nLS = m_vecData.size();
					Add((unsigned char*)&n, sizeof(ULONG));
					stream.SkipWhiteSpace();
					while (!stream.AtEOF() && !stream.GetMatching(")"))
					{
						GM_POINT Pt;
						Pt.x = stream.GetSignedDouble();
						stream.SkipWhiteSpace();
						Pt.y = stream.GetSignedDouble();
						stream.SkipWhiteSpace();
						Add((unsigned char*)&Pt, sizeof(GM_POINT));
						GM_LinearRing* pLS = (GM_LinearRing*)(m_vecData.data() + nLS);
						pLS->numPoints++;
						if (stream.GetMatching(","))
						{
							// there will be another point
						}
						else
						{
							char ch = stream.PeekChar();
							if (ch != ')')
							{
								throw ") expected after coordinates";
							}
						}
						stream.SkipWhiteSpace();
					}
				}
				else
				{
					throw "Coordinate list must begin with (";
				}
			} while (stream.GetMatching(","));
			if (!stream.GetMatching(")"))
			{
				throw "Polygon must terminate with )";
			}
		}
		else
		{
			throw "Polygon must start with (";
		}
	}
	else
	{
		throw "CWKBGeometry::Parse Unsupported geometry type";
	}
}