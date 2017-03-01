#pragma once

//#include "GerberHandler.h"
#include <vector>
#include <map>
#include "InputStream.h"
#include "GerberCommand.h"
#include "GerberAperature.h"
#include "GerberMacro.h"
#include "GerberRender.h"

#include "GerberTypes.h"
#include "Layer.h"
#include "Part.h"
#include <regex>

class CRectangle;

class CGerberParser
{
	CInputStream m_stream;
	std::vector<CGerberCommand*> m_vecCommands;
	std::map<unsigned int, CGerberAperature*> m_mapAperatures;
	std::map<std::string, CGerberMacro*> m_mapMacro;
	std::vector<CLayer*> m_vecLayers;
	std::map<std::string, CPart*> m_mapParts;
	CPart* m_pPart;
	std::regex m_rgxBefore;
	std::regex m_rgxAfter;
	bool m_bDoParts;
public:
	CGerberParser();
	virtual ~CGerberParser();
//	void OnChar(char ch);
	bool Parse(const char* pszText);
	void Render(CGerberRender* pRender, double dDeflate);
	void Render(CGerberRender* pRender, CLayer* pLayer, double dDeflate);
	void Render(CGerberRender* pRender, CLayer* pLayer);
	CInputStream* Stream() { return &m_stream; }
	CGerberAperature* GetAperature(unsigned int nCode);
	CGerberMacro* GetMacro(const char* pszName);
	void AddAperature(unsigned int nCode, CGerberAperature* pAperature);
	void AddMacro(const char* pszName, CGerberMacro* pMacro);
	void AddLayer(CLayer* pLayer);
	CLayer* MatchingLayer(const char* pszComment);
	void AddPart(CPart* pPart);
	void RenderParts(CGerberRender* pRender, CLayer* pPartsLayer, double dInflate);
	void PartCheck(const char* pszComment, CGerberState* pState);
	void SetPartRegex(const char* pszBefore, const char* pszAfter);
	CPart* GetPart(const char* pszPart);
	CRectangle* GetBounds();
};

