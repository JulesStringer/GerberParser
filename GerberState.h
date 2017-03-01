#pragma once

#include "GerberTypes.h"
class CGerberAperature;
class CLayer;

class CGerberState
{
	PRIMARY_INTERPOLATION m_Interpolation;
	bool m_bMultiSegment;
	CGerberAperature* m_pAperature;
	bool m_bDark;
	bool m_bXMirror;
	bool m_bYMirror;
	double m_dRotation;
	double m_dScale;
	CLayer* m_pLayer;
public:
	CGerberState();
	CGerberState(CGerberState* pOther);
	~CGerberState();
	void SetInterpolation(PRIMARY_INTERPOLATION Interpolation);
	PRIMARY_INTERPOLATION GetInterpolation();
	void SetMultiSegment(bool bMultiSegment);
	bool GetMultiSegment();
	void SetAperature(CGerberAperature* pAperature);
	CGerberAperature* GetAperature();
	void SetDark(bool bDark);
	bool GetDark();
	void SetXMirror(bool bXMirror);
	void SetYMirror(bool bYMirror);
	bool GetXMirror();
	bool GetYMirror();
	void SetRotation(double d);
	double GetRotation();
	void SetScale(double d);
	double GetScale();
	void SetLayer(CLayer* pLayer);
	CLayer* GetLayer();
};

