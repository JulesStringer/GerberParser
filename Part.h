#pragma once

#include <string>

class CGerberRender;
class CGerberState;
class CGerberCommand;
class CLayer;

class CPart
{
	std::string m_strName;
	double m_xmin;
	double m_ymin;
	double m_xmax;
	double m_ymax;
	bool m_bFirst;
	CGerberState* m_pState;
	bool m_bInflated;
	double m_dInflateW;
	double m_dInflateH;
public:
	CPart();
	CPart(const char* pszName, CGerberState* pState);
	virtual ~CPart();
	void Inflate(double X, double Y);
	//void Inflate(CGerberCommand* pCommand);
	const char* Name();
	void Render(CGerberRender* pRender, CLayer* pLayer, double dInflate);
	void SetInflate(double W, double H);
};

