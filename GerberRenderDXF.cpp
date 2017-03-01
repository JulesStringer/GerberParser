#include "GerberRenderDXF.h"
#include "Layer.h"
#include "GerberState.h"

CGerberRenderDXF::CGerberRenderDXF()
	: m_Handle(0)
{
}
CGerberRenderDXF::~CGerberRenderDXF()
{
}
void CGerberRenderDXF::OutputTAG(int nTag, const char* pszValue)
{
	Print("  %d\r\n", nTag);
	Print("%s\r\n", pszValue);
}
void CGerberRenderDXF::OutputTAG(int nTag, int nValue)
{
	Print("  %d\r\n", nTag);
	Print("%d\r\n", nValue);
}
void CGerberRenderDXF::OutputTAG(int nTag, unsigned int nValue)
{
	Print("  %d\r\n", nTag);
	Print("%d\r\n", nValue);
}
void CGerberRenderDXF::OutputTAG(int nTag, double dValue)
{
	Print("  %d\r\n", nTag);
	Print("%f\r\n", dValue);
}
void CGerberRenderDXF::OutputHandle(int nTag, unsigned int nValue)
{
	Print("  %d\r\n", nTag);
	Print("%x\r\n", nValue);
}
void CGerberRenderDXF::BeginSection(const char* pszValue)
{
	OutputTAG(0, "SECTION");
	OutputTAG(2, pszValue);
}
void CGerberRenderDXF::EndSection()
{
	OutputTAG(0, "ENDSEC");
}
unsigned int CGerberRenderDXF::Handle(int nTag)
{
	m_Handle++;
	OutputHandle(nTag, m_Handle);
	return m_Handle;
}
void CGerberRenderDXF::Variable(const char* pszName, int nTag, const char* pszValue)
{
	OutputTAG(9, pszName);
	OutputTAG(nTag, pszValue);
}
void CGerberRenderDXF::Variable(const char* pszName, int nTag, int nValue)
{
	OutputTAG(9, pszName);
	OutputTAG(nTag, nValue);
}
void CGerberRenderDXF::Coordinate(int nTagBase, double X, double Y)
{
	OutputTAG(nTagBase + 10, X);
	OutputTAG(nTagBase + 20, Y);
}
void CGerberRenderDXF::Coordinate(int nTagBase, double X, double Y, double Z)
{
	OutputTAG(nTagBase + 10, X);
	OutputTAG(nTagBase + 20, Y);
	OutputTAG(nTagBase + 30, Z);
}
void CGerberRenderDXF::HeaderSection(int nMeasurementType)
{
	BeginSection("HEADER");
	Variable("$ACADVER", 1, "AC1014");
	Variable("$HANDSEED", 5, "FFFF");
	Variable("$MEASUREMENT", 70, nMeasurementType);
	EndSection();
}
unsigned int CGerberRenderDXF::BeginTable(const char* pszName, const char* pszSubClass, int nMaxEntries, bool bNo330)
{
	OutputTAG(0, "TABLE");
	OutputTAG(2, pszName);
	unsigned int h = Handle(5);
	if (!bNo330)
	{
		OutputHandle(330, 0); // Does this vary from 0?
	}
	OutputTAG(100, pszSubClass);
	OutputTAG(70, nMaxEntries);
	return h;
}
void CGerberRenderDXF::EndTable()
{
	OutputTAG(0, "ENDTAB");
}
unsigned int CGerberRenderDXF::BeginTableEntry(const char* pszTable, const char* pszName, const char* pszSubClass, unsigned int XREF)
{
	OutputTAG(0, pszTable);
	unsigned int h = Handle(5);
	OutputHandle(330, XREF);
	OutputTAG(100, "AcDbSymbolTableRecord");
	OutputTAG(100, pszSubClass);
	OutputTAG(2, pszName);
	return h;
}
unsigned int CGerberRenderDXF::BeginTableEntry(const char* pszTable, const char* pszName, const char* pszSubClass)
{
	OutputTAG(0, pszTable);
	unsigned int h = Handle(5);
	OutputTAG(100, "AcDbSymbolTableRecord");
	OutputTAG(100, pszSubClass);
	OutputTAG(2, pszName);
	return h;
}
unsigned int CGerberRenderDXF::BeginLTYPE(const char* pszName, const char* pszDescription, unsigned int XREF, int nElements, int TotalLength)
{
	unsigned int h = BeginTableEntry("LTYPE", pszName, "AcDbLinetypeTableRecord", XREF);
	OutputTAG(70, 0);
	OutputTAG(3, pszDescription);
	OutputTAG(72, 65);
	OutputTAG(73, nElements);
	OutputTAG(40, 0);
	return h;
}
unsigned int CGerberRenderDXF::BeginLAYER(const char* pszName, const char* pszLineType)
{
	unsigned int h = BeginTableEntry("LAYER", pszName, "AcDbLayerTableRecord");
	OutputTAG(70, 0);
	OutputTAG(6, pszLineType);
	return h;
}
unsigned int CGerberRenderDXF::BeginSTYLE(const char* pszName, const char* pszFont, unsigned int XREF, double dLastHeightUsed,
	unsigned int Flags, double dFixedFontHeight, double dWidthFactor, double dObliquingAngle, unsigned int GenerationFlags,
	const char* pszBigFont)
{
	unsigned int h = BeginTableEntry("STYLE", pszName, "AcDbTextStyleTableRecord", XREF);
	OutputTAG(70, Flags);
	OutputTAG(40, dFixedFontHeight);
	OutputTAG(41, dWidthFactor);
	OutputTAG(42, dLastHeightUsed);
	OutputTAG(50, dObliquingAngle);
	OutputTAG(71, GenerationFlags);
	OutputTAG(3, pszFont);
	OutputTAG(4, pszBigFont);
	return h;
}
unsigned int CGerberRenderDXF::BeginDIMSTYLE(const char* pszName, unsigned int XREF)
{
	unsigned int h = BeginTableEntry("DIMSTYLE", pszName, "AcDbDimStyleTableRecord", XREF);
	OutputTAG(70, 0);  // Flags
	OutputTAG(3, "");  // DIMPOST
	OutputTAG(4, "");  // DIMAPOST
	OutputTAG(5, "");  // DIMBLK (obsolete)
	OutputTAG(6, "");  // DIMBLK1 (obsolete)
	OutputTAG(7, "");  // DIMBLK2 (obsolete)
	OutputTAG(40, 1);		// DIMSCALE
	OutputTAG(41, 2.5);		// DIMASZ
	OutputTAG(42, 0.625);	// DIMEXO
	OutputTAG(43, 3.75);	// DIMLI
	OutputTAG(44, 1.25);	// DIMEXE
	OutputTAG(45, 0);		// DIMRND
	OutputTAG(46, 0);		// DIMDLE
	OutputTAG(47, 0);		// DIMTP
	OutputTAG(48, 0);		// DIMTM
	OutputTAG(140, 2.5);	// DIMTXT
	OutputTAG(141, 2.5);	// DIMCEN
	OutputTAG(142, 0);		// DIMTSZ
	OutputTAG(143, 0.0393700787);	// DIMALTF
	OutputTAG(144, 1);		// DIMLFAC
	OutputTAG(145, 0);		// DIMTVP
	OutputTAG(146, 1);		// DIMTFAC
	OutputTAG(147, 0.625);	// DIMGAP
	OutputTAG(71, 0);		// DIMTOL
	OutputTAG(72, 0);		// DIMLIM
	OutputTAG(73, 0);		// DIMTIH
	OutputTAG(74, 0);		// DIMTOH
	OutputTAG(75, 0);		// DIMSE1
	OutputTAG(76, 0);		// DIMSE2
	OutputTAG(77, 1);		// DIMTAD
	OutputTAG(78, 8);		// DIMZIN
	OutputTAG(170, 0);		// DIMALT
	OutputTAG(171, 3);		// DIMALTD
	OutputTAG(172, 1);		// DIMTOFL
	OutputTAG(173, 0);		// DIMSAH
	OutputTAG(174, 0);		// DIMTIX
	OutputTAG(175, 0);		// DIMSOXD
	OutputTAG(176, 0);		// DIMCLRD
	OutputTAG(177, 0);		// DIMCLRE
	OutputTAG(178, 0);		// DIMCLRT
	OutputTAG(270, 2);		// DIMUNIT (obsolete)
	OutputTAG(271, 2);		// DIMDEC
	OutputTAG(272, 2);		// DIMTDEC
	OutputTAG(273, 2);		// DIMALTU
	OutputTAG(274, 3);		// DIMALTTD
	OutputTAG(340, 11);		// DIMTXSTY
	OutputTAG(275, 0);		// DIMAUNIT
	OutputTAG(280, 0);		// DIMJUST
	OutputTAG(281, 0);		// DIMSD1
	OutputTAG(282, 0);		// DIMSD2
	OutputTAG(283, 0);		// DIMTOLJ
	OutputTAG(284, 8);		// DIMTZIN
	OutputTAG(285, 0);		// DIMALTZ
	OutputTAG(286, 0);		// DIMALTTZ
	OutputTAG(287, 3);		// DIMFIT (obsolete)
	OutputTAG(288, 0);		// DIMUPT
	return h;
}
void CGerberRenderDXF::TablesSection()
{
	BeginSection("TABLES");
	// Viewport
	unsigned int nVportHandle = BeginTable("VPORT", "AcDbSymbolTable", 4);
	unsigned int h = BeginTableEntry("VPORT", "*ACTIVE", "AcDbViewportTableRecord", nVportHandle);
	OutputTAG(70, 0);
	Coordinate(0, 0.0, 0.0);		// Viewport lower left
	Coordinate(1, 1.0, 1.0);		// Viewport upper right
	Coordinate(2, 210.0, 148.5);    // View point center
	Coordinate(3, 0.0, 0.0);        // Snap point base
	Coordinate(4, 10.0, 10.0);      // Snap spacing
	Coordinate(5, 10.0, 10.0);      // Grid Spacing
	Coordinate(6, 0.0, 0.0, 1.0);   // View direction
	Coordinate(7, 0.0, 0.0, 0.0);   // View target point
	OutputTAG(40, 341.0);			// View height
	OutputTAG(41, 1.24);            // Viewport aspect ratio
	OutputTAG(42, 50.0);            // Lens length
	OutputTAG(43, 0.0);             // Front clipping plane
	OutputTAG(44, 0.0);             // Back clipping plane
	OutputTAG(50, 0.0);             // Snap rotation angle
	OutputTAG(51, 0.0);             // View twist angle
	OutputTAG(71, 0);               // View mode
	OutputTAG(72, 100);             // Zoom circle percent
	OutputTAG(73, 1);               // Fast zoom setting
	OutputTAG(74, 3);               // UCSICON setting
	OutputTAG(75, 0);               // Snap on/off
	OutputTAG(76, 0);               // Grid on/off
	OutputTAG(77, 0);               // Snap style
	OutputTAG(78, 0);               // Snap isopair
	EndTable();
	// LTYPE - Line styles
	unsigned int LtypeHandle = BeginTable("LTYPE", "AcDbSymbolTable", 1);
	BeginLTYPE("BYBLOCK", "", LtypeHandle, 0, 0);
	BeginLTYPE("BYLAYER", "", LtypeHandle, 0, 0);
	BeginLTYPE("CONTINUOUS", "Solid line", LtypeHandle, 0, 0);
	EndTable();
	// Layers

	unsigned int LayerHandle = BeginTable("LAYER", "AcDbSymbolTable", 1 + m_mapLayers.size(), true);
	BeginLAYER("0", "CONTINUOUS");
	std::map<std::string, CLayer*>::iterator it;
	for (it = m_mapLayers.begin(); it != m_mapLayers.end(); it++)
	{
		BeginLAYER(it->first.c_str(), "CONTINUOUS");
	}
	EndTable();
	// STYLE
	unsigned int StyleHandle = BeginTable("STYLE", "AcDbSymbolTable", 1);
	BeginSTYLE("STANDARD", "txt", StyleHandle, 2.5);
	EndTable();
	// VIEW
	unsigned int ViewHandle = BeginTable("VIEW", "AcDbSymbolTable", 0);
	EndTable();
	// UCS
	unsigned int UCSHandle = BeginTable("UCS", "AcDbSymbolTable", 0);
	EndTable();
	//	APPID
	unsigned int AppIDHandle = BeginTable("APPID", "AcDbSymbolTable", 2);
	h = BeginTableEntry("APPID", "ACAD", "AcDbReqAppTableRecord", AppIDHandle);
	OutputTAG(70, 0);
    EndTable();
	// DIMSTYLE
	unsigned int DimStyleHandle = BeginTable("DIMSTYLE", "AcDbSymbolTable", 1);
	h = BeginDIMSTYLE("ISO-25", DimStyleHandle);
	EndTable();
	// BLOCK_RECORD
	unsigned int BlockRecordHandle = BeginTable("BLOCK_RECORD", "AcDbSymbolTable", 1);
	m_ModelSpaceHandle = BeginTableEntry("BLOCK_RECORD", "*MODEL_SPACE", "AcDbBlockTableRecord", BlockRecordHandle);
	m_PaperSpaceHandle = BeginTableEntry("BLOCK_RECORD", "*PAPER_SPACE", "AcDbBlockTableRecord", BlockRecordHandle);
	EndTable();
	EndSection();
}
unsigned int CGerberRenderDXF::BeginBlock(const char* pszName, const char* pszLayer, unsigned int XREF, bool bPaperSpace,
	double X, double Y, double Z)
{
	OutputTAG(0, "BLOCK");
	unsigned int h = Handle(5);
	OutputHandle(330, XREF);
	OutputTAG(100, "AcDbEntity");
	if (bPaperSpace)
	{
		OutputTAG(67, 1);
	}
	OutputTAG(8, pszLayer);
	OutputTAG(100, "AcDbBlockBegin");
	OutputTAG(2, pszName);        // block name
	if (!bPaperSpace && X == 0 && Y == 0 && Z == 0)
	{
		OutputTAG(70, 0);			  // Flags
		Coordinate(0, X, Y, Z);       // Base point
		OutputTAG(3, pszName);		  // block name
	}
	OutputTAG(1, "");             // XRef path
	return h;
}
void CGerberRenderDXF::EndBlock(const char* pszLayer, unsigned int XREF, bool bPaperSpace)
{
	OutputTAG(0, "ENDBLK");
	unsigned int h = Handle(5);
	OutputHandle(330, m_ModelSpaceHandle);
	OutputTAG(100, "AcDbEntity");
	if (bPaperSpace)
	{
		OutputTAG(67, 1);      // model or paper space
	}
	OutputTAG(8, pszLayer);
	OutputTAG(100, "AcDbBlockEnd");
}
void CGerberRenderDXF::BlocksSection()
{
	BeginSection("BLOCKS");
	BeginBlock("*MODEL_SPACE", "0", m_ModelSpaceHandle);
	EndBlock("0", m_ModelSpaceHandle);
	BeginBlock("*PAPER_SPACE", "0", m_PaperSpaceHandle, true);
	EndBlock("0", m_PaperSpaceHandle, true);
	EndSection();
}
void CGerberRenderDXF::BeginDictionary(unsigned int handle, unsigned int ownerhandle)
{
	OutputTAG(0, "DICTIONARY");
	OutputTAG(5, handle);
	OutputHandle(330, ownerhandle);
	OutputTAG(100, "AcDbDictionary");
}
void CGerberRenderDXF::DictionaryReference(unsigned int handle, const char* pszName)
{
	OutputTAG(3, pszName);
	OutputHandle(350, handle);
}
void CGerberRenderDXF::ObjectsSection()
{
	BeginSection("OBJECTS");
	// sort out handles for dictionaries
	m_Handle++;
	unsigned int MasterHandle = m_Handle;
	m_Handle++;
	unsigned int AcadGroupHandle = m_Handle;
	m_Handle++;
	unsigned int AcadGroupHandle1 = m_Handle;
	m_Handle++;
	unsigned int StandardHandle = m_Handle;
	m_Handle++;
	unsigned int SubStandardHandle = m_Handle;
	BeginDictionary(MasterHandle, 0);
	DictionaryReference(AcadGroupHandle, "ACAD_GROUP");
	DictionaryReference(StandardHandle, "ACAD_MLINESTYLE");
	BeginDictionary(AcadGroupHandle, MasterHandle);
	BeginDictionary(AcadGroupHandle1, MasterHandle);
	BeginDictionary(StandardHandle, MasterHandle);
	DictionaryReference(SubStandardHandle, "STANDARD");
	BeginDictionary(SubStandardHandle, MasterHandle);
	EndSection();
}
void CGerberRenderDXF::Open(const char* pszFile)
{
	CGerberRender::Open(pszFile);
	// Write DXF header sections
	HeaderSection(1); // units are metric
	TablesSection();
	BlocksSection();

	BeginSection("ENTITIES");
}
void CGerberRenderDXF::Close()
{
	EndSection(); // End of entities section
	ObjectsSection();
	CGerberRender::Close();
}
void CGerberRenderDXF::BeginEntity(const char* pszType, const char* pszLayer, const char* pszSubClass, unsigned int Colour)
{
	OutputTAG(0, pszType);
	Handle(5);
	OutputTAG(100, "AcDbEntity");
	OutputTAG(8, pszLayer);
	OutputTAG(62, Colour);
	OutputTAG(100, pszSubClass);
}
void CGerberRenderDXF::Line(const char* pszLayer, unsigned int Colour, double X1, double Y1, double X2, double Y2)
{
	BeginEntity("LINE", pszLayer, "AcDbLine", Colour);
	Coordinate(0, X1 + m_OffsetX, Y1 + m_OffsetY, 0.0);
	Coordinate(1, X2 + m_OffsetX, Y2 + m_OffsetY, 0.0);
}
// DXF etc derived from D2DRenderTarget ?
// No because D2DGeometry does not allow you to get at points
void CGerberRenderDXF::MoveTo(double X, double Y, CGerberState* pState)
{
	m_X = X;
	m_Y = Y;
}
void CGerberRenderDXF::LineTo(double X, double Y, CGerberState* pState)
{
	CLayer* pLayer = pState->GetLayer();
	if (pLayer->Include())
	{
		std::string strLayer = GetLayerName(pLayer);
		unsigned char color = GetColor(pLayer);
		Line(strLayer.c_str(), color, m_X, m_Y, X, Y);
	}
	m_X = X;
	m_Y = Y;
}
void CGerberRenderDXF::HollowPolygon(WKBPolygon* pPoly, CGerberState* pState)
{
	CLayer* pLayer = pState->GetLayer();
	std::string strLayer = GetLayerName(pLayer);
	unsigned char color = GetColor(pLayer);

	if (pLayer->Include())
	{
		unsigned char n;
		GM_POINT* pLast = pPoly->rings[0].Points;
		for (n = 1; n < pPoly->rings[0].numPoints;n++)
		{
			GM_POINT* pPt = pPoly->rings[0].Points + n;
			Line(strLayer.c_str(), color, pLast->x, pLast->y, pPt->x, pPt->y);
			pLast = pPt;
		}
	}
}
void CGerberRenderDXF::Polygon(CWKBGeometry* pGeom, CGerberState* pState)
{
	WKBPolygon* pPoly = (WKBPolygon*)pGeom->Data();
	HollowPolygon(pPoly, pState);
	// If hollow draw a set of line segments
	// TODO for laser cutter we want outline if cutting or etching
	// For rendering solid may be more appropriate
}
void CGerberRenderDXF::Circle(double X, double Y, double dRadius, CGerberState* pState)
{
	CLayer* pLayer = pState->GetLayer();
	std::string strLayer = GetLayerName(pLayer);
	unsigned char color = GetColor(pLayer);

	if (pLayer->Include())
	{
		BeginEntity("CIRCLE", strLayer.c_str(), "AcDbCircle", color);
		Coordinate(0, X + m_OffsetX, Y + m_OffsetY, 0.0);
		OutputTAG(40, dRadius);
	}
}
