#pragma once
#include "GerberTypes.h"

#include <windows.h>

#include "WKBGeometry.h"
#include <map>

class CGerberState;
class CLayer;
class CRectangle;

class CGerberRender
{
	HANDLE m_hFile;
protected:
	double m_OffsetX;
	double m_OffsetY;
	double m_X;
	double m_Y;
	int m_nLineStringRenderMethod;
	void Print(const char* pszFormat, ...);
	std::map<std::string, CLayer*> m_mapLayers;
	unsigned char GetColor(CLayer* pLayer);
	const char* GetLayerName(CLayer* pLayer);
	virtual void Polygon(CWKBGeometry* pPoly, CGerberState* pState);
	virtual void LineString(CWKBGeometry* pLS, CGerberState* pState);
	virtual void Point(CWKBGeometry* pPt, CGerberState* pState);
public:
	CGerberRender();
	virtual ~CGerberRender();
	static CGerberRender* Create(OUTPUT_TYPE ot);
	virtual void MoveTo(double X, double Y, CGerberState* pState);
	virtual void LineTo(double X, double Y, CGerberState* pState);
	virtual void Circle(double X, double Y, double dRadius, CGerberState* pState);
	virtual void Geometry(CWKBGeometry* pGeom, CGerberState* pState);
	virtual void Open(const char* pszFile);
	virtual void Close();
	void Rectangle(CRectangle* pRect, CGerberState* pState);
	void AddLayer(CLayer* pLayer);
	std::map<std::string, CLayer*>* GetLayers();
	void SetOffset(double X, double Y);
	CLayer* GetLayer(const char* pszName);
	void SetLineStringRenderMethod(int nLineStringRenderMethod);
};
