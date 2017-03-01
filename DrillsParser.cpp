#include "DrillsParser.h"
#include "DrillTool.h"
#include "DrillHole.h"
#include "GerberState.h"
#include "Layer.h"

CDrillsParser::CDrillsParser()
	: m_pTool(NULL)
{
}
CDrillsParser::~CDrillsParser()
{
}
void CDrillsParser::SetDecimals(int nDefault)
{
	char ch = m_stream.GetChar();
	double dFactor = 1;
	if (ch == '0')
	{
		int nBefore = 0;
		while (ch == '0')
		{
			nBefore++;
			ch = m_stream.GetChar();
		}
		int nAfter = 0;
		if (ch == '.')
		{
			ch = m_stream.GetChar();
		}
		while (ch == '0')
		{
			dFactor *= 0.1;
			nAfter++;
			ch = m_stream.GetChar();
		}
	}
	else
	{
		while (nDefault > 0)
		{
			dFactor *= 0.1;
			nDefault--;
		}
		std::string str;
		while (!m_stream.AtEOL())
		{
			str += ch;
			if (!m_stream.AtEOL())
			{
				ch = m_stream.GetChar();
			}
		}
		if (str == "LZ")
		{

		}
		else if (str == "TZ")
		{
			throw "Trailing zeros not supported";
		}
	}
	m_stream.SetXFactor(dFactor);
	m_stream.SetYFactor(dFactor);
}
bool CDrillsParser::Parse(const char* pszText)
{
	m_stream.Set(pszText);
	bool bResult = false;
	bool bRewound = false;
	double X = 0;
	double Y = 0;
	while (!m_stream.AtEOF() && !bResult)
	{
		char ch = m_stream.GetChar();
		switch (ch)
		{
		case ';':
			// Comment
			m_stream.SkipToNextLine();
			break;
		case '%':
			// Rewind
			bRewound = true;
			m_pTool = NULL;
			m_stream.SkipToNextLine();
			break;
		case 'T':
			// Tool definition
			{
				int nTool = m_stream.GetInt();
				if (bRewound)
				{
					// Get tool definition and add tool to map 
					std::map<int, CDrillTool*>::iterator it = m_mapTools.find(nTool);
					if (it == m_mapTools.end())
					{
						throw "Tool not defined";
					}
					else
					{
						m_pTool = it->second;
					}
				}
				else
				{
					// Set current tool
					m_pTool = new CDrillTool(nTool);
					m_mapTools[nTool] = m_pTool;
					m_pTool->Parse(&m_stream);
				}
				m_stream.SkipToNextLine();
			}
			break;
		case 'I':
			// could be INCH command
			{
				std::string str;
				while (ch != ',' && !m_stream.AtEOL())
				{
					str += ch;
					ch = m_stream.GetChar();
				}
				if (str == "INCH")
				{
					m_stream.SetUnits(25.4);
					// if not at end of line get format
					if (ch == ',')
					{
						SetDecimals(4);
					}
				}
				m_stream.SkipToNextLine();
			}
			break;
		case 'M':
			// could be METRIC or M command
			{
				char ch1 = m_stream.PeekChar();
				if (!isdigit(ch1))
				{
					std::string str;
					while (ch != ',' && !m_stream.AtEOL())
					{
						str += ch;
						ch = m_stream.GetChar();
					}
					if (str == "METRIC")
					{
						m_stream.SetUnits(1);
						if (ch == ',')
						{
							SetDecimals(2);
						}
					}
				}
				else
				{
					int nCode = m_stream.GetInt();
					switch (nCode)
					{
					case 0:
					case 30:  // End of file
						bResult = true;
						break;
					case 48:  // beginning of header
						break;
					case 95:  // End of header
						bRewound = true;
						break; 
					}
				}
				m_stream.SkipToNextLine();
			}
			break;
		case 'G':
			// G commmand
			// G05 means start of drills
			// G90 means absolute positioning
			{
				int nCode = m_stream.GetInt();
				switch (nCode)
				{
				case 5:  // Drill mode
					break;
				case 90: // Absolute mode
					break;
				case 91: // Incremental mode
					throw "Incremental mode not supported";
				default:
					throw "Mode not supported";
				}
			}
			m_stream.SkipToNextLine();
			break;
		case 'X':
			// Coordinate - in practice this is a hole when combined with a Y
			X = m_stream.GetXCoordinate();
			ch = m_stream.GetChar();
			// If Y not specified use old one
			if (ch == 'Y')
			{
				Y = m_stream.GetYCoordinate();
			}
			{
				CDrillHole* pHole = new CDrillHole(X, Y, m_pTool);
				m_vecHoles.push_back(pHole);
			}
			m_stream.SkipToNextLine();
			break;
		case 'Y':
			// Y coordinate only use previous X
			Y = m_stream.GetYCoordinate();
			{
				CDrillHole* pHole = new CDrillHole(X, Y, m_pTool);
				m_vecHoles.push_back(pHole);
			}
			m_stream.SkipToNextLine();
			break;
		}
	}
	return bResult;
}
void CDrillsParser::Render(CGerberRender* pRender, CLayer* pLayer, double dMinDiameter)
{
	CGerberState* pState = new CGerberState();
	pState->SetLayer(pLayer);

	std::vector<CDrillHole*>::iterator it;
	for (it = m_vecHoles.begin(); it != m_vecHoles.end(); it++)
	{
		if ((*it)->Diameter() >= dMinDiameter)
		{
			(*it)->Render(pRender, pState);
		}
	}
}
