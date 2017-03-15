#include "stdafx.h"
#include "TextFile.h"
#include <windows.h>


CTextFile::CTextFile()
	: m_pszText(NULL)
{
}
CTextFile::~CTextFile()
{
	if (m_pszText)
	{
		delete[] m_pszText;
	}
}
bool CTextFile::Open(const char* pszFile)
{
	HANDLE hFile = ::CreateFileA(pszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		throw "Unable to open file\r\n";
	}
	else
	{
		DWORD dwHigh;
		DWORD dwLow = ::GetFileSize(hFile, &dwHigh);
		m_pszText = new char[dwLow + 1];
		DWORD dwRead;
		::ReadFile(hFile, m_pszText, dwLow, &dwRead, NULL);
		CloseHandle(hFile);
		m_pszText[dwLow] = 0;
		return dwLow == dwRead;
	}
	return false;
}
const char* CTextFile::Text()
{
	return m_pszText;
}
