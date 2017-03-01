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
public:
	CWKBGeometry();
	virtual ~CWKBGeometry();
	void Add(unsigned char* pdata, int nLen);
	unsigned char* Data();
};

