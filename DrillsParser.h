#pragma once

#include "InputStream.h"
class CGerberRender;
class CDrillTool;
class CDrillHole;
class CLayer;

#include <map>
#include <vector>
#include <string>

class CDrillsParser
{
	CInputStream m_stream;
	std::map<int, CDrillTool*> m_mapTools;
	std::vector<CDrillHole*> m_vecHoles;
	CDrillTool* m_pTool;
	void SetDecimals(int nDefault);
public:
	CDrillsParser();
	virtual ~CDrillsParser();
	bool Parse(const char* pszText);
	void Render(CGerberRender* pRender, CLayer* pLayer, double dMinDiameter);
};

