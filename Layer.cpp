#include "Layer.h"
#include <regex>

CLayer::CLayer()
	: m_nColourIndex(0)
	, m_bInclude(true)
{
}
CLayer::~CLayer()
{
}
CLayer::CLayer(const char* pszName, unsigned char nColourIndex, const char* pszRegex)
	: m_strName(pszName)
	, m_nColourIndex(nColourIndex)
	, m_strRegex(pszRegex)
	, m_bInclude(true)
{
}
unsigned char CLayer::ColourIndex()
{
	return m_nColourIndex;
}
const char* CLayer::Name()
{
	return m_strName.c_str();
}
bool CLayer::CommentMatch(const char* psz)
{
	// This function can be overridden to return true to set this as the current layer
	// when a comment matching a predetermined pattern is recognised.
	std::string strComment(psz);
	std::regex regex(m_strRegex.c_str());

	return regex_match(strComment.begin(), strComment.end(), regex);
	// For FreePCB try the following
	// Outline ::= "-+ *Draw board outline";
	// Parts :: = "-+ *Draw Parts";
}
bool CLayer::Include() { return m_bInclude; }
void CLayer::SetInclude(bool bInclude)
{
	m_bInclude = bInclude;
}