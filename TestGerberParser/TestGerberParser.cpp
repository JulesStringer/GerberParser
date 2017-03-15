// TestGerberParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\GerberParser\GerberParser.h"
#include "..\GerberParser\GerberRender.h"
#include "..\GerberParser\DrillsParser.h"
#include <windows.h>
#include <string>
#include <vector>
#include "TextFile.h"
#import <msxml3.dll>
#include <atlbase.h>
#include "XMLHelper.h"
#include "Folder.h"

void PasteMaskFromGerber(const char* pszFile, CGerberRender* pRender, double dDeflate)
{
	CTextFile input;
	if ( input.Open(pszFile))
	{
		CGerberParser parser;

		// Parse the paste file
		parser.Parse(input.Text());
		parser.Render(pRender, dDeflate);
	}
}
void AddMountingHoles(const char* pszDrillFile, CGerberRender* pRender, CLayer* pHoles, double dMinDiameter)
{
	CTextFile input;
	if (input.Open(pszDrillFile))
	{
		//
		//  The drills file is parsed to derive a tools and holes
		CDrillsParser parser;
		parser.Parse(input.Text());
		//  Holes are rendered to the holes layer if their diameter exceeds 2.9mm
		parser.Render(pRender, pHoles, dMinDiameter);
	}
}
void PartOutlines(const char* pszInput, CLayer* pParts, CGerberRender* pRender, double dInflate)
{
	CTextFile input;
	if (input.Open(pszInput))
	{
		CGerberParser parser;
		//   Identify parts via comments to generate parts bounds
		//   these to be used for component placement.
		//
		//   This layer is to be turned off when producing paste mask,
		//   but should be turned on to produce part outlines output.
		//
		//   Before and After regex are used to detect comments that contain part numbers.
		//   When a match is detected the regex matches are removed, leaving the part number.
		//   The commands following a part number are used to create a bounding rectangle for the part
		//
		//   For some parts do we need to include a rectangle for part body.
		//
		parser.SetPartRegex(" Draw part ", "");
		parser.Parse(input.Text());
		// Render using the parts layer and inflating each aperature by 0.5mm
		parser.RenderParts(pRender, pParts, dInflate);
	}
}
void RenderFolder(const char* pszFolder, CGerberRender* pRender, double X, double Y)
{
	std::string strFolder(pszFolder);
	std::string strInput = strFolder + "\\top_paste_mask.grb";
	std::string strDrills = strFolder + "\\drill_file.drl";

	CLayer* pHoles = pRender->GetLayer("Mounting_Holes");
	CLayer* pParts = pRender->GetLayer("Parts");
	pRender->SetOffset(X, Y);
	if (strInput.length() > 0)
	{
		PasteMaskFromGerber(strInput.c_str(), pRender, 0.0);
		if (strDrills.length() > 0)
		{
			AddMountingHoles(strDrills.c_str(), pRender, pHoles, 2.9);
		}
		PartOutlines(strInput.c_str(), pParts, pRender, 0.5);
	}
}
void RenderDrawings(CGerberRender* pRender, IXMLDOMNodeList* pDrawings, DEFAULTS* pDefaults, std::map<int, CFolder*>& mapFolders)
{
	long n;
	long nDrawings;
	pDrawings->get_length(&nDrawings);
	for (n = 0; n < nDrawings; n++)
	{
		CComPtr<IXMLDOMNode> spNode;
		pDrawings->get_item(n, &spNode);
		if (spNode.p)
		{
			CComBSTR bstrName;
			spNode->get_nodeName(&bstrName);
			if (bstrName == L"drawing")
			{
				// Get Folder
				CFolder* pFolder = NULL;
				int nID = CXMLHelper::GetXMLInt(spNode, "folder", 0);
				if (nID > 0)
				{
					std::map<int, CFolder*>::iterator it = mapFolders.find(nID);
					if (it == mapFolders.end())
					{
						throw "Invalid Folder";
					}
					pFolder = it->second;
				}
				else
				{
					// All folder not present but still need it.
					pFolder = new CFolder();
				}
				if (pFolder)
				{
					pFolder->Process(pRender, spNode, pDefaults);
				}
			}
			else if (bstrName == L"include")
			{
				std::string strPath = CXMLHelper::GetXMLString(spNode, "path", "");
				CComPtr<IXMLDOMNode> spDoc = CXMLHelper::Load(strPath.c_str());
				CComPtr<IXMLDOMNodeList> spDrawings = CXMLHelper::GetXMLChildNodes(spDoc, "/config/drawings");
				RenderDrawings(pRender, spDrawings, pDefaults, mapFolders);
			}
		}
	}
}
void RenderFromParams(const char* pszInput, CGerberRender* pRender)
{
	std::map<int, CFolder*> mapFolders;

	CComPtr<IXMLDOMDocument> spXMLDocument = CXMLHelper::Load(pszInput);
	if (spXMLDocument.p)
	{
		DEFAULTS defaults;
		defaults.dDefInflate = CXMLHelper::GetXMLR8(spXMLDocument, "//config/inflate", 0.0);
		defaults.dDefDeflate = CXMLHelper::GetXMLR8(spXMLDocument, "//config/deflate", 0.0);
		defaults.dDefMinsize = CXMLHelper::GetXMLR8(spXMLDocument, "//config/minsize", 0.0);
		defaults.dCutlineInflate = CXMLHelper::GetXMLR8(spXMLDocument, "//config/cutline_inflate", 0.0);
		std::string strFolders = CXMLHelper::GetXMLString(spXMLDocument, "//config/folders/path","");
		CComPtr<IXMLDOMNodeList> spFolders;
		if (strFolders.length() > 0)
		{
			CComPtr<IXMLDOMDocument> spFDOC = CXMLHelper::Load(strFolders.c_str());
			if (spFDOC)
			{
				spFolders = CXMLHelper::GetXMLNodes(spFDOC, "//config/folders/folder");
			}
		}
		else
		{
			spFolders = CXMLHelper::GetXMLNodes(spXMLDocument, "//config/folders/folder");
		}
		if (spFolders.p)
		{
			long n;
			long nFolders;
			spFolders->get_length(&nFolders);
			for (n = 0; n < nFolders; n++)
			{
				CComPtr<IXMLDOMNode> spNode;
				spFolders->get_item(n, &spNode);
				if (spNode.p)
				{
					int nID = CXMLHelper::GetXMLInt(spNode, "ID", 0);
					std::string strPath = CXMLHelper::GetXMLString(spNode, "path", "");
					CComPtr<IXMLDOMNode> spParts = CXMLHelper::GetXMLNode(spNode, "parts");
					CFolder* pFolder = new CFolder(nID, strPath.c_str(), spParts);
					mapFolders[nID] = pFolder;
				}
			}
		}
		CComPtr<IXMLDOMNodeList> spDrawings = CXMLHelper::GetXMLChildNodes(spXMLDocument, "//config/drawings");
		if (spDrawings.p)
		{
			RenderDrawings(pRender, spDrawings, &defaults, mapFolders);
		}
	}
}
void CheckLayer(CGerberRender* pRender, const char* pszName, int nColourIndex, const char* pszRegex)
{
	CLayer* pLayer = pRender->GetLayer(pszName);
	if (pLayer == NULL)
	{
		new CLayer(pszName, nColourIndex, pszRegex);
		pRender->AddLayer(pLayer);
	}
}
int main(int ac, char** av)
{
	try
	{
		::CoInitialize(NULL);
		int n;
		std::string strInput;
		std::string strDrills;
		std::string strPaste;
		std::string strFolder;
		std::string strLayers;

		OUTPUT_TYPE ot = OUTPUT_TEST;
		typedef enum
		{
			INPUT_FOLDER = 0,
			INPUT_GERBER = 1,
			INPUT_PARAMS = 2
		}INPUT_TYPE;
		int nLineStringRenderMethod = 0;
		INPUT_TYPE iType = INPUT_FOLDER;
		for (n = 1; n < ac; n++)
		{
			if (_stricmp(av[n], "-folder") == 0)
			{
				n++;
				strFolder = av[n];
				strInput = strFolder + "\\top_paste_mask.grb";
				strDrills = strFolder + "\\drill_file.drl";
				iType = INPUT_FOLDER;
			}
			else if (_stricmp(av[n], "-params") == 0)
			{
				n++;
				strInput = av[n];
				iType = INPUT_PARAMS;
			}
			else if (_stricmp(av[n], "-gerber") == 0)
			{
				n++;
				strInput = av[n];
				iType = INPUT_GERBER;
			}
			else if (_stricmp(av[n], "-test") == 0)
			{
				n++;
				strPaste = av[n];
			}
			else if (_stricmp(av[n], "-dxf") == 0)
			{
				ot = OUTPUT_DXF;
				n++;
				strPaste = av[n];
			}
			else if (_stricmp(av[n], "-png") == 0)
			{
				ot = OUTPUT_PNG;
				n++;
				strPaste = av[n];
			}
			else if (_stricmp(av[n], "-wkt") == 0)
			{
				ot = OUTPUT_WKT;
				n++;
				strPaste = av[n];
			}
			else if (_stricmp(av[n], "-sql") == 0)
			{
				ot = OUTPUT_SQL;
				n++;
				strPaste = av[n];
			}
			else if (_stricmp(av[n], "-Polyline") == 0)
			{
				nLineStringRenderMethod = 1;
			}
			else if (_stricmp(av[n], "-LWPolyline") == 0)
			{
				nLineStringRenderMethod = 2;
			}
			else if (_stricmp(av[n], "-layers") == 0)
			{
				n++;
				strLayers = av[n];
			}
		}
		// Set up renderer
		CGerberRender* pRender = CGerberRender::Create(ot);
		if (strLayers.length() > 0)
		{
			CComPtr<IXMLDOMDocument> spDoc = CXMLHelper::Load(strLayers.c_str());
			if (spDoc.p)
			{
				CComPtr<IXMLDOMNodeList> spLayers = CXMLHelper::GetXMLNodes(spDoc, "/layers/layer");
				if (spLayers.p)
				{
					long nCount;
					long nC;
					spLayers->get_length(&nCount);
					for (nC = 0; nC < nCount; nC++)
					{
						CComPtr<IXMLDOMNode> spNode;
						spLayers->get_item(nC, &spNode);
						if (spNode.p)
						{
							std::string strName = CXMLHelper::GetXMLString(spNode, "name","");
							int nColourIndex = CXMLHelper::GetXMLInt(spNode, "colourindex", 0);
							std::string strRegex = CXMLHelper::GetXMLString(spNode, "regex", "NO MATCH");
							CLayer* pLayer = new CLayer(strName.c_str(), nColourIndex, strRegex.c_str());
							pRender->AddLayer(pLayer);
						}
					}
				}
			}
		}
		// Add layer definitions for board outline and parts sections
		CheckLayer(pRender, "Outline", 4, ".*Draw board outline.*");
		CheckLayer(pRender, "Pads", 3, ".*Draw Parts.*");
		CheckLayer(pRender, "Parts", 5, "NO MATCH");
		CheckLayer(pRender, "Mounting_Holes", 6, "NO MATCH");
		CheckLayer(pRender, "graphics", 8, "NO MATCH");
		CheckLayer(pRender, "Cut_Line", 7, "NO MATCH");
		pRender->SetLineStringRenderMethod(nLineStringRenderMethod);
		pRender->Open(strPaste.c_str());
		switch (iType)
		{
		case INPUT_GERBER:
			pRender->SetOffset(10, 10);
			PasteMaskFromGerber(strInput.c_str(), pRender, 0.0);
			break;
		case INPUT_FOLDER:
			RenderFolder(strFolder.c_str(), pRender, 10, 10);
			break;
		case INPUT_PARAMS:
			RenderFromParams(strInput.c_str(), pRender);
			break;
		}
		pRender->Close();
	}
	catch (const char* psz)
	{
		::MessageBox(NULL, psz, "Exception", MB_OK);
	}
	return 0;
}
// TODO
// Allow new layers to be specified in paste layers for individual components
//    so that different laser parameters can be applied to these.
// Option to merge pads in a row for specific components
//    To deal with pads too close together to laser cut.
//    Merge a set of pads with all the same X or Y
//    and within tolerance, output the bounding box of these instead.
// Try engraving pads with power 25 speed 400 as in video
//   - shrink by 0.002 inch
//   
// Option to output polygons as LWPolyline records
// Finish coding aperature types and testing
// Interpolations other than straight
// Implement regions
// Implement multisegment interpolation
// Implement aperture blocks
// Implement mirror, rotate and scale
// Implement Step and repeat