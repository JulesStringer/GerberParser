#include "stdafx.h"
#include "Folder.h"

#include "..\GerberParser\GerberParser.h"
#include "..\GerberParser\DrillsParser.h"
#include "..\GerberParser\GerberRender.h"
#include "..\GerberParser\Rectangle.h"
#include "..\GerberParser\GerberState.h"
#include "..\GerberParser\Layer.h"

#include "XMLHelper.h"
#include "TextFile.h"
#include "graphic.h"

CFolder::CFolder()
	: m_nID(0)
{
}
CFolder::CFolder(int nID, const char* pszPath, IXMLDOMNode* pPartsNode)
	: m_nID(nID)
	, m_strPath(pszPath)
	, m_spPartsNode(pPartsNode)
{
}
CFolder::~CFolder()
{
}
#if 0
void CFolder::Process(CGerberRender* pRender, IXMLDOMNode* pDrawing, DEFAULTS* pDefaults)
{
	// X
	double X = CXMLHelper::GetXMLR8(pDrawing, "X", 0.0);
	// Y
	double Y = CXMLHelper::GetXMLR8(pDrawing, "Y", 0.0);
	pRender->SetOffset(X, Y);
	// parts node
	CComPtr<IXMLDOMNode> spParts = CXMLHelper::GetXMLNode(pDrawing, "parts");
	// paste node
	CComPtr<IXMLDOMNode> spPaste = CXMLHelper::GetXMLNode(pDrawing, "paste");
	// drills node
	CComPtr<IXMLDOMNode> spDrills = CXMLHelper::GetXMLNode(pDrawing, "drills");
	// Graphics node
	CComPtr<IXMLDOMNode> spGraphics = CXMLHelper::GetXMLNode(pDrawing, "graphics");
	// outline node
	CComPtr<IXMLDOMNode> spOutline = CXMLHelper::GetXMLNode(pDrawing, "outline");
	// cutline node
	CComPtr<IXMLDOMNode> spCutline = CXMLHelper::GetXMLNode(pDrawing, "cutline");

	double dCutlineInflate = CXMLHelper::GetXMLR8(pDrawing,"cutlineinflate", pDefaults->dCutlineInflate);
	double dInflate = CXMLHelper::GetXMLR8(pDrawing, "inflate", pDefaults->dDefInflate);
	std::string strFile = m_strPath + "\\top_paste_mask.grb";
	CGerberParser parser;
	// Add layer definitions for board outline and parts sections
	// which also contain regex to identify these sections
	std::map<std::string, CLayer*>* pmapLayers = pRender->GetLayers();
	std::map<std::string, CLayer*>::iterator it;
	for (it = pmapLayers->begin(); it != pmapLayers->end(); it++)
	{
		parser.AddLayer(it->second);
	}
	CDrillsParser drills;


	if (m_strPath.length() > 0)
	{
		CTextFile inputGerber;
		if (inputGerber.Open(strFile.c_str()))
		{
			parser.SetPartRegex(" Draw part ", "");
			parser.Parse(inputGerber.Text());
		}

		CTextFile inputDrills;
		std::string strDrills = m_strPath + "\\drill_file.drl";
		if (inputDrills.Open(strDrills.c_str()))
		{
			drills.Parse(inputDrills.Text());
		}
	}
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
	// Update inflate parameters for individual parts
	if (m_spPartsNode.p)
	{
		CComPtr<IXMLDOMNodeList> spComponents = CXMLHelper::GetXMLNodes(m_spPartsNode, "part");
		long nC;
		long nComp;
		spComponents->get_length(&nComp);
		for (nC = 0; nC < nComp; nC++)
		{
			CComPtr<IXMLDOMNode> spComp;
			spComponents->get_item(nC, &spComp);
			std::string strID = CXMLHelper::GetXMLString(spComp, "ID", "");
			double W = CXMLHelper::GetXMLR8(spComp, "W", dInflate);
			double H = CXMLHelper::GetXMLR8(spComp, "H", dInflate);
			CPart* pPart = parser.GetPart(strID.c_str());
			if (pPart)
			{
				pPart->SetInflate(W, H);
			}
		}
	}
	// Render part outlines
	if (spParts.p)
	{
		CLayer* pParts = pRender->GetLayer("Parts");
		// Render using the parts layer and inflating each aperature by 0.5mm
		parser.RenderParts(pRender, pParts, dInflate);
	}
	// Render paste mask
	if (spPaste.p)
	{
		CLayer* pLayer = pRender->GetLayer("Pads");
		double deflate = CXMLHelper::GetXMLR8(spPaste, "deflate", pDefaults->dDefDeflate);
		parser.Render(pRender, pLayer, deflate);
	}
	// Render mounting holes
	if (spDrills.p)
	{
		CLayer* pHoles = pRender->GetLayer("Mounting_Holes");
		double dMinDiameter = CXMLHelper::GetXMLR8(spDrills, "minsize", pDefaults->dDefMinsize);
		drills.Render(pRender, pHoles, dMinDiameter);
	}
	// Render graphics
	if (spGraphics.p)
	{
		CComPtr<IXMLDOMNodeList> spNodes;
		spGraphics->get_childNodes(&spNodes);
		long nLen;
		spNodes->get_length(&nLen);
		long n;
		for (n = 0; n < nLen; n++)
		{
			CComPtr<IXMLDOMNode> spNode;
			spNodes->get_item(n, &spNode);
			CComQIPtr<IXMLDOMElement> spEle = spNode;
			if (spEle.p)
			{
				CComBSTR bstrName;
				spEle->get_nodeName(&bstrName);
				CGraphic* pGraphic = CGraphic::Create(bstrName);
				if (pGraphic)
				{
					pGraphic->Parse(spNode);
					pGraphic->Render(pRender);
				}
			}
		}
	}
	// Render outline
	if (spOutline.p)
	{
		CLayer* pOutline = pRender->GetLayer("Outline");
		parser.Render(pRender, pOutline);
	}
	// Render cut line
	if (spCutline.p)
	{
		// Render board outline
		CLayer* pCutline = pRender->GetLayer("Cut_Line");
		CRectangle* pBounds = parser.GetBounds();
		CGerberState State;
		State.SetLayer(pCutline);
		CRectangle rc(pBounds->MinX() - dCutlineInflate, pBounds->MinY() - dCutlineInflate,
			          pBounds->MaxX() + dCutlineInflate, pBounds->MaxY() + dCutlineInflate);
		pRender->Rectangle(&rc, &State);
	}
	// Label?
}
#endif
void CFolder::RenderGraphics(CGerberRender* pRender, IXMLDOMNode* spGraphics, CLayer* pLayer)
{
	CComPtr<IXMLDOMNodeList> spNodes;
	spGraphics->get_childNodes(&spNodes);
	long nLen;
	spNodes->get_length(&nLen);
	long n;
	for (n = 0; n < nLen; n++)
	{
		CComPtr<IXMLDOMNode> spNode;
		spNodes->get_item(n, &spNode);
		CComQIPtr<IXMLDOMElement> spEle = spNode;
		if (spEle.p)
		{
			CComBSTR bstrName;
			spEle->get_nodeName(&bstrName);
			CGraphic* pGraphic = CGraphic::Create(bstrName);
			if (pGraphic)
			{
				pGraphic->Parse(spNode);
				pGraphic->Render(pRender, pLayer);
			}
		}
	}
}
void CFolder::Process(CGerberRender* pRender, IXMLDOMNode* pDrawing, DEFAULTS* pDefaults)
{
	// X
	double X = CXMLHelper::GetXMLR8(pDrawing, "X", 0.0);
	// Y
	double Y = CXMLHelper::GetXMLR8(pDrawing, "Y", 0.0);
	pRender->SetOffset(X, Y);
	double dCutlineInflate = CXMLHelper::GetXMLR8(pDrawing, "cutlineinflate", pDefaults->dCutlineInflate);
	double dInflate = CXMLHelper::GetXMLR8(pDrawing, "inflate", pDefaults->dDefInflate);
	std::string strFile = m_strPath + "\\top_paste_mask.grb";
	CGerberParser parser;
	// Add layer definitions for board outline and parts sections
	// which also contain regex to identify these sections
	std::map<std::string, CLayer*>* pmapLayers = pRender->GetLayers();
	std::map<std::string, CLayer*>::iterator it;
	for (it = pmapLayers->begin(); it != pmapLayers->end(); it++)
	{
		parser.AddLayer(it->second);
	}
	CDrillsParser drills;
	if (m_strPath.length() > 0)
	{
		CTextFile inputGerber;
		if (inputGerber.Open(strFile.c_str()))
		{
			parser.SetPartRegex(" Draw part ", "");
			parser.Parse(inputGerber.Text());
		}

		CTextFile inputDrills;
		std::string strDrills = m_strPath + "\\drill_file.drl";
		if (inputDrills.Open(strDrills.c_str()))
		{
			drills.Parse(inputDrills.Text());
		}
	}
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
	// Update inflate parameters for individual parts
	if (m_spPartsNode.p)
	{
		CComPtr<IXMLDOMNodeList> spComponents = CXMLHelper::GetXMLNodes(m_spPartsNode, "part");
		long nC;
		long nComp;
		spComponents->get_length(&nComp);
		for (nC = 0; nC < nComp; nC++)
		{
			CComPtr<IXMLDOMNode> spComp;
			spComponents->get_item(nC, &spComp);
			std::string strID = CXMLHelper::GetXMLString(spComp, "ID", "");
			double W = CXMLHelper::GetXMLR8(spComp, "W", dInflate);
			double H = CXMLHelper::GetXMLR8(spComp, "H", dInflate);
			CPart* pPart = parser.GetPart(strID.c_str());
			if (pPart)
			{
				pPart->SetInflate(W, H);
			}
		}
	}
	CComPtr<IXMLDOMNodeList> spNodes;
	pDrawing->get_childNodes(&spNodes);

	if (spNodes.p)
	{
		long nC;
		long nNodes;
		spNodes->get_length(&nNodes);
		for (nC = 0; nC < nNodes; nC++)
		{
			CComPtr<IXMLDOMNode> spNode;
			spNodes->get_item(nC, &spNode);
			CComBSTR bstrName;
			spNode->get_nodeName(&bstrName);
			if (bstrName == L"parts")
			{
				CLayer* pParts = pRender->GetLayer("Parts");
				// Render using the parts layer and inflating each aperature by 0.5mm
				parser.RenderParts(pRender, pParts, dInflate);
			}
			else if (bstrName == L"paste")
			{
				CLayer* pLayer = pRender->GetLayer("Pads");
				double deflate = CXMLHelper::GetXMLR8(spNode, "deflate", pDefaults->dDefDeflate);
				parser.Render(pRender, pLayer, deflate);
			}
			else if (bstrName == L"drills")
			{
				CLayer* pHoles = pRender->GetLayer("Mounting_Holes");
				double dMinDiameter = CXMLHelper::GetXMLR8(spNode, "minsize", pDefaults->dDefMinsize);
				drills.Render(pRender, pHoles, dMinDiameter);
			}
			else if (bstrName == L"graphics")
			{
				RenderGraphics(pRender, spNode);
			}
			else if (bstrName == L"outline")
			{
				CLayer* pOutline = pRender->GetLayer("Outline");
				parser.Render(pRender, pOutline);
			}
			else if (bstrName == L"cutline")
			{
				// Render board outline
				CLayer* pCutline = pRender->GetLayer("Cut_Line");
				CRectangle* pBounds = parser.GetBounds();
				CGerberState State;
				State.SetLayer(pCutline);
				CRectangle rc(pBounds->MinX() - dCutlineInflate, pBounds->MinY() - dCutlineInflate,
					pBounds->MaxX() + dCutlineInflate, pBounds->MaxY() + dCutlineInflate);
				pRender->Rectangle(&rc, &State);
			}
			else if (bstrName == L"include")
			{
				std::string strPath = CXMLHelper::GetXMLString(spNode, "path", "");
				CComPtr<IXMLDOMNode> spDoc = CXMLHelper::Load(strPath.c_str());
				CComPtr<IXMLDOMNode> spGraphics = CXMLHelper::GetXMLNode(spDoc, "graphics");
				std::string strLayer = CXMLHelper::GetXMLString(spNode, "layer", "");
				CLayer* pLayer = NULL;
				if (strLayer.length() > 0)
				{
					pLayer = pRender->GetLayer(strLayer.c_str());

				}
				RenderGraphics(pRender, spGraphics, pLayer);
			}
		}
	}
}
