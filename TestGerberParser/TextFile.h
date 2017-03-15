#pragma once

#include <string>

class CTextFile
{
	char* m_pszText;
public:
	CTextFile();
	~CTextFile();
	bool Open(const char* pszFile);
	const char* Text();
};

