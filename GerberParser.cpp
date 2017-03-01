#include "GerberParser.h"
#include "GerberState.h"
#include "Rectangle.h"

CGerberParser::CGerberParser()
	: m_pPart(NULL)
	, m_bDoParts(false)
{
}
CGerberParser::~CGerberParser()
{
}
bool CGerberParser::Parse(const char* pszText)
{
	m_stream.Set(pszText);
	CGerberState* pState = new CGerberState();
	while( !m_stream.AtEOF())
	{
		CGerberCommand* pCommand = CGerberCommand::Create(this, pState);
		if (pCommand)
		{
			m_vecCommands.push_back(pCommand);
			if (m_pPart)
			{
				pCommand->Inflate(m_pPart);
			}
		}
	}
	// return true if all ok
	return true;
}
CGerberAperature* CGerberParser::GetAperature(unsigned int nCode)
{
	std::map<unsigned int, CGerberAperature*>::iterator it = m_mapAperatures.find(nCode);
	if (it != m_mapAperatures.end())
	{
		return it->second;
	}
	return NULL;
}
CGerberMacro* CGerberParser::GetMacro(const char* pszName)
{
	std::string str(pszName);
	std::map<std::string, CGerberMacro*>::iterator it = m_mapMacro.find(str);
	if (it != m_mapMacro.end())
	{
		return it->second;
	}
	return NULL;
}
void CGerberParser::AddAperature(unsigned int nCode, CGerberAperature* pAperature)
{
	m_mapAperatures[nCode] = pAperature;
}
void CGerberParser::AddMacro(const char* pszName, CGerberMacro* pMacro)
{
	std::string str(pszName);
	m_mapMacro[str] = pMacro;
}
void CGerberParser::AddLayer(CLayer* pLayer)
{
	m_vecLayers.push_back(pLayer);
}
CLayer* CGerberParser::MatchingLayer(const char* pszComment)
{
	std::vector<CLayer*>::iterator it;
	for (it = m_vecLayers.begin(); it != m_vecLayers.end(); it++)
	{
		if ((*it)->CommentMatch(pszComment))
		{
			return *it;
		}
	}
	return NULL;
}
void CGerberParser::Render(CGerberRender* pRender, double dDeflate)
{
	std::vector<CGerberCommand*>::iterator it;
	for (it = m_vecCommands.begin(); it != m_vecCommands.end(); it++)
	{
		(*it)->Render(pRender, dDeflate);
	}
}
void CGerberParser::Render(CGerberRender* pRender, CLayer* pLayer, double dDeflate)
{
	std::vector<CGerberCommand*>::iterator it;
	for (it = m_vecCommands.begin(); it != m_vecCommands.end(); it++)
	{
		if ((*it)->MatchesLayer(pLayer))
		{
			(*it)->Render(pRender, dDeflate);
		}
	}
}
void CGerberParser::Render(CGerberRender* pRender, CLayer* pLayer)
{
	std::vector<CGerberCommand*>::iterator it;
	for (it = m_vecCommands.begin(); it != m_vecCommands.end(); it++)
	{
		if ((*it)->MatchesLayer(pLayer))
		{
			(*it)->Render(pRender, 0.0);
		}
	}
}
void CGerberParser::RenderParts(CGerberRender* pRender, CLayer* pLayer, double dInflate)
{
	std::map<std::string, CPart*>::iterator it;
	for (it = m_mapParts.begin(); it != m_mapParts.end(); it++)
	{
		it->second->Render(pRender, pLayer, dInflate);
	}
}
void CGerberParser::AddPart(CPart* pPart)
{
	std::string str = pPart->Name();
	m_mapParts[str] = pPart;
}
void CGerberParser::PartCheck(const char* pszComment, CGerberState* pState)
{
	if (m_bDoParts)
	{
		std::string strComment(pszComment);
		std::string strRep("");
		if (regex_search(strComment, m_rgxBefore) && regex_search(strComment, m_rgxAfter))
		{
			std::string str = regex_replace(strComment, m_rgxBefore, strRep);
			str = regex_replace(str, m_rgxAfter, strRep);
			m_pPart = new CPart(str.c_str(), new CGerberState(pState));
			AddPart(m_pPart);
		}
	}
}
void CGerberParser::SetPartRegex(const char* pszBefore, const char* pszAfter)
{
	m_rgxBefore = pszBefore;
	m_rgxAfter = pszAfter;
	m_bDoParts = true;
}
CPart* CGerberParser::GetPart(const char* pszPart)
{
	std::string str(pszPart);
	std::map<std::string, CPart*>::iterator it = m_mapParts.find(str);
	if (it != m_mapParts.end())
	{
		return it->second;
	}
	return NULL;
}
CRectangle* CGerberParser::GetBounds()
{
	CRectangle* pRect = new CRectangle();
	std::vector<CGerberCommand*>::iterator it;
	for (it = m_vecCommands.begin(); it != m_vecCommands.end(); it++)
	{
		(*it)->Inflate(pRect);
	}
	return pRect;
}