#pragma once

#ifndef _X86_
#define _X86_
#endif

#include <minwindef.h>
#include "wkb.h"
#include <vector>

class CWKBGeometry
{
	std::vector<unsigned char> m_vecData;
	void IncrementPoints();
	void IncrementRings();
	ULONG m_nLinearRing;
	ULONG m_nPolygon;
	ULONG m_nGeometry;
public:
	CWKBGeometry();
	virtual ~CWKBGeometry();
	void Add(unsigned char* pdata, int nLen);
	void AddPoint(GM_POINT* pPt);
	unsigned char* Data();
	void Parse(const char* pszWKT);
	int Type();
};

