#include "GerberState.h"
#include "GerberAperature.h"

CGerberState::CGerberState()
	: m_Interpolation(INTERPOLATION_LINEAR)
	, m_bMultiSegment(false)
	, m_pAperature(NULL)
	, m_bDark(true)
	, m_bXMirror(false)
	, m_bYMirror(false)
	, m_dRotation(0)
	, m_dScale(1)
	, m_pLayer(NULL)
{
}
CGerberState::~CGerberState()
{
}
CGerberState::CGerberState(CGerberState* pOther)
{
	m_Interpolation = pOther->m_Interpolation;
	m_bMultiSegment = pOther->m_bMultiSegment;
	m_pAperature = pOther->m_pAperature;
	m_bDark = pOther->m_bDark;
	m_bXMirror = pOther->m_bXMirror;
	m_bYMirror = pOther->m_bYMirror;
	m_dRotation = pOther->m_dRotation;
	m_dScale = pOther->m_dScale;
	m_pLayer = pOther->m_pLayer;
}
void CGerberState::SetInterpolation(PRIMARY_INTERPOLATION Interpolation)
{
	m_Interpolation = Interpolation;
}
PRIMARY_INTERPOLATION CGerberState::GetInterpolation()
{
	return m_Interpolation;
}
void CGerberState::SetMultiSegment(bool bMultiSegment)
{
	m_bMultiSegment = bMultiSegment;
}
bool CGerberState::GetMultiSegment()
{
	return m_bMultiSegment;
}
void CGerberState::SetAperature(CGerberAperature* pAperature)
{
	m_pAperature = pAperature;
}
CGerberAperature* CGerberState::GetAperature()
{
	return m_pAperature;
}
void CGerberState::SetDark(bool bDark)
{
	m_bDark = bDark;
}
bool CGerberState::GetDark()
{
	return m_bDark;
}
void CGerberState::SetXMirror(bool bXMirror)
{
	m_bXMirror = bXMirror;
}
void CGerberState::SetYMirror(bool bYMirror)
{
	m_bYMirror = bYMirror;
}
bool CGerberState::GetXMirror()
{
	return m_bXMirror;
}
bool CGerberState::GetYMirror()
{
	return m_bYMirror;
}
void CGerberState::SetRotation(double d)
{
	m_dRotation = d;
}
double CGerberState::GetRotation() { return m_dRotation; }
void CGerberState::SetScale(double d)
{
	m_dScale = d;
}
double CGerberState::GetScale() { return m_dScale; }
void CGerberState::SetLayer(CLayer* pLayer)
{
	m_pLayer = pLayer;
}
CLayer* CGerberState::GetLayer() { return m_pLayer; }