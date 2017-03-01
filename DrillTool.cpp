#include "DrillTool.h"

#include "InputStream.h"

CDrillTool::CDrillTool()
	: m_nTool(0)
{
}
CDrillTool::CDrillTool(int nTool)
	: m_nTool(nTool)
{

}
CDrillTool::~CDrillTool()
{
}
void CDrillTool::Parse(CInputStream* pStream)
{
	// C drill diameter
	// F Feed rate
	// S drill speed
	while (!pStream->AtEOL())
	{
		char ch = pStream->GetChar();
		switch (ch)
		{
		case 'C':
			m_dDiameter = pStream->GetSize();
			break;
		case 'F':
			m_dFeedRate = pStream->GetSignedDouble();
			break;
		case 'S':
			m_dSpeed = pStream->GetInt() * 1000;
			break;
		}
	}
}
double CDrillTool::Diameter()
{
	return m_dDiameter;
}
