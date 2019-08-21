#include "WinWrappers.h"
#include <Windows.h>

std::string GetNameFromOpenFileA(const char* c)
{
	OPENFILENAMEA ofn;       // common dialog box structure
	CHAR szFileTitle[512] = { 0 }; // if using TCHAR macros
//	szFileTitle[0] = '\0';
									// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = NULL;
	ofn.nMaxFile = 0;
	ofn.lpstrFilter = c;// "All\0*.*\0Text\0*.TXT\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		return  ofn.lpstrFileTitle;
	}
	return "";
}

std::string GetPathFromOpenFileA(const char* c)
{
	OPENFILENAMEA ofn;       // common dialog box structure
	CHAR szFile[512] = { 0 }; // if using TCHAR macros

								   // Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = c;//"All\0*.*\0Text\0*.TXT\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		return  std::string(ofn.lpstrFile);
	}
	return "";
}

std::string GetNameFromSaveFileA(const char* c)
{
	OPENFILENAMEA ofn;       // common dialog box structure
	CHAR szFileTitle[512] = { 0 }; // if using TCHAR macros

								   // Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = NULL;
	ofn.nMaxFile = 0;
	ofn.lpstrFilter = c;//"All\0*.*\0Text\0*.TXT\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileNameA(&ofn) == TRUE)
	{
		return  ofn.lpstrFileTitle;
	}
	return "";
}

std::string GetPathFromSaveFileA(const char* c, const std::string& defNam)
{
	OPENFILENAMEA ofn;       // common dialog box structure
	CHAR szFile[512] = { 0 }; // if using TCHAR macros

	memcpy(szFile, defNam.c_str(), defNam.size());
							  // Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = c;//"All\0*.*\0Text\0*.TXT\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileNameA(&ofn) == TRUE)
	{
		return  ofn.lpstrFile;
	}

	return "";
}