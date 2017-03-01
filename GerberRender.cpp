#include "GerberRender.h"
#include <stdio.h>
#include <stdarg.h>

#include "GerberRenderPNG.h"
#include "GerberRenderDXF.h"

#include "Layer.h"
#include "Rectangle.h"

CGerberRender::CGerberRender()
	: m_X(0)
	, m_Y(0)
	, m_hFile(INVALID_HANDLE_VALUE)
{
}
CGerberRender::~CGerberRender()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
	}
}
CGerberRender* CGerberRender::Create(OUTPUT_TYPE ot)
{
	switch (ot)
	{
	case OUTPUT_TEST:
		return new CGerberRender();
	case OUTPUT_DXF:
		return new CGerberRenderDXF();
	case OUTPUT_PNG:
		return new CGerberRenderPNG();
	case OUTPUT_WKT:
		break;
	case OUTPUT_SQL:
		break;
	}
	throw "Unimplemented GerberRender type";
}
void CGerberRender::Open(const char* pszFile)
{
	m_hFile = ::CreateFile(pszFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		throw "Unable to create output file";
	}
}
void CGerberRender::Close()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}
void CGerberRender::Print(const char* pszFormat, ...)
{
	va_list args;
	va_start(args, pszFormat);
	char sz[2048];
	int n = vsprintf_s(sz, pszFormat, args);
	DWORD dwWritten;
	::WriteFile(m_hFile, sz, n, &dwWritten, NULL);
	va_end(args);
}
void CGerberRender::MoveTo(double X, double Y, CGerberState* pState)
{
	
	Print("MoveTo %f %f\r\n", X + m_OffsetX, Y + m_OffsetY);
	m_X = X;
	m_Y = Y;
}
void CGerberRender::LineTo(double X, double Y, CGerberState* pState)
{
	Print("LineTo %f %f\r\n", X + m_OffsetX, Y + m_OffsetY);
	m_X;
	m_Y;
}
void CGerberRender::Polygon(CWKBGeometry* pGeom, CGerberState* pState)
{
	WKBPolygon* pPoly = (WKBPolygon*)pGeom->Data();
	unsigned char n;
	Print("Polygon %d Vertices:\r\n", pPoly->rings[0].numPoints);
	for (n = 0; n < pPoly->rings[0].numPoints;n++)
	{
		GM_POINT* pPt = pPoly->rings[0].Points + n;
		Print("%f %f\r\n", pPt->x + m_OffsetX, pPt->y + m_OffsetY);
	}
}
void CGerberRender::Circle(double X, double Y, double dRadius, CGerberState* pState)
{
	Print("Circle radius %f (%f,%f)", dRadius, X + m_OffsetX, Y + m_OffsetY);
}
void CGerberRender::AddLayer(CLayer* pLayer)
{
	std::string strName = pLayer->Name();
	m_mapLayers[strName] = pLayer;
}
unsigned char CGerberRender::GetColor(CLayer* pLayer)
{
	if (pLayer)
	{
		return pLayer->ColourIndex();
	}
	return 0;
}
const char* CGerberRender::GetLayerName(CLayer* pLayer)
{
	if (pLayer)
	{
		return pLayer->Name();
	}
	return "0";
}
CLayer* CGerberRender::GetLayer(const char* pszName)
{
	std::string str(pszName);
	std::map<std::string, CLayer*>::iterator it = m_mapLayers.find(str);
	if (it != m_mapLayers.end())
	{
		return it->second;
	}
	return NULL;
}
std::map<std::string, CLayer*>* CGerberRender::GetLayers()
{
	return &m_mapLayers;
}
void CGerberRender::SetOffset(double X, double Y)
{
	m_OffsetX = X;
	m_OffsetY = Y;
}
void CGerberRender::Rectangle(CRectangle* pRect, CGerberState* pState)
{
	CWKBGeometry geom;
	WKBPolygon poly;
	poly.wkbType = wkbPolygon;
	poly.byteOrder = wkbNDR;
	poly.numRings = 1;
	poly.rings[0].numPoints = 5;
	poly.rings[0].Points[0].x = pRect->MinX();
	poly.rings[0].Points[0].y = pRect->MinY();
	geom.Add((unsigned char*)&poly, sizeof(WKBPolygon));
	GM_POINT Pt;
	Pt.x = pRect->MinX();
	Pt.y = pRect->MaxY();
	geom.Add((unsigned char*)&Pt, sizeof(Pt));
	Pt.x = pRect->MaxX();
	geom.Add((unsigned char*)&Pt, sizeof(Pt));
	Pt.y = pRect->MinY();
	geom.Add((unsigned char*)&Pt, sizeof(Pt));
	Pt.x = pRect->MinX();
	geom.Add((unsigned char*)&Pt, sizeof(Pt));
	Polygon(&geom, pState);
}
