#pragma once
#include "GerberRender.h"
class CGerberRenderDXF :
	public CGerberRender
{
	unsigned int m_Handle;
	unsigned int m_ModelSpaceHandle;
	unsigned int m_PaperSpaceHandle;
protected:
	void OutputTAG(int nTag);
	void OutputTAG(int nTag, const char* pszValue);
	void OutputTAG(int nTag, int nValue);
	void OutputTAG(int nTag, unsigned int nValue);
	void OutputTAG(int nTag, double dValue);
	void OutputHandle(int nTag, unsigned int nValue);
	void BeginSection(const char* pszValue);
	void EndSection();
	void Variable(const char* pszName, int nTag, const char* pszValue);
	void Variable(const char* pszName, int nTag, int nValue);
	void Coordinate(int nTagBase, double X, double Y);
	void Coordinate(int nTagBase, double X, double Y, double Z);
	void HeaderSection(int nMeasurementType);
	unsigned int Handle(int nTag);
	unsigned int BeginTable(const char* pszName, const char* pszSubClass, int MaxEntries, bool bNo330 = false);
	void EndTable();
	unsigned int BeginTableEntry(const char* pszTable, const char* pszName, const char* pszSubClass, unsigned int XREF);
	unsigned int BeginTableEntry(const char* pszTable, const char* pszName, const char* pszSubClass);
	unsigned int BeginLTYPE(const char* pszName, const char* pszDescription, unsigned int XREF, int nElements, int TotalLength);
	unsigned int BeginLAYER(const char* pszName, const char* pszLineType);
	unsigned int BeginSTYLE(const char* pszName, const char* pszFont, unsigned int XREF, double dLastHeightUsed,
		unsigned int Flags = 0, double dFixedFontHeight = 0.0, double dWidthFactor = 1.0,
		double dObliquingAngle = 0, unsigned int GenerationFlags = 0,
		const char* pszBigFont = "");
	unsigned int BeginDIMSTYLE(const char* pszName, unsigned int XREF);
	void TablesSection();
	unsigned int BeginBlock(const char* pszName, const char* pszLayer, unsigned int XREF,
		bool bPaperSpace = false,
		double X = 0.0, double Y = 0.0, double Z = 0.0);
	void EndBlock(const char* pszLayer, unsigned int XREF, bool bPaperSpace = false);
	void BlocksSection();
	void BeginDictionary(unsigned int handle, unsigned int ownerhandle);
	void DictionaryReference(unsigned int handle, const char* pszName);
	void ObjectsSection();
	void BeginEntity(const char* pszType, const char* pszLayer, const char* pszSubClass, unsigned int Colour);
	void Line(const char* pszLayer, unsigned int Colour, double X1, double Y1, double X2, double Y2);
	void HollowPolygon(WKBPolygon* pPoly, CGerberState* pState);
public:
	CGerberRenderDXF();
	virtual ~CGerberRenderDXF();
	virtual void MoveTo(double X, double Y, CGerberState* pState);
	virtual void LineTo(double X, double Y, CGerberState* pState);
	virtual void Polygon(CWKBGeometry* pPoly, CGerberState* pState);
	virtual void Circle(double X, double Y, double dRadius, CGerberState* pState);
	virtual void Open(const char* pszFile);
	virtual void Close();
};

