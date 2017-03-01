#include "WKBGeometry.h"



CWKBGeometry::CWKBGeometry()
{
}


CWKBGeometry::~CWKBGeometry()
{
}
void CWKBGeometry::Add(unsigned char* pdata, int nLen)
{
	int n = m_vecData.size();
	m_vecData.resize(nLen + n);
	memcpy(m_vecData.data() + n, pdata, nLen);
}
unsigned char* CWKBGeometry::Data() { return m_vecData.data(); }
