#pragma once

class CInputStream;

class CDrillTool
{
	int m_nTool;
	double m_dDiameter;
	double m_dFeedRate;
	double m_dSpeed;
public:
	CDrillTool();
	CDrillTool(int nTool);
	virtual ~CDrillTool();
	void Parse(CInputStream* pStream);
	double Diameter();
};

