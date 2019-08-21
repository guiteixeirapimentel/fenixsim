#pragma once
#include <string>

std::string GetNameFromOpenFileA(const char* c = "All\0*.*\0\0");
std::string GetPathFromOpenFileA(const char* c = "All\0*.*\0\0");
							  	
std::string GetNameFromSaveFileA(const char* c = "All\0*.*\0\0");
std::string GetPathFromSaveFileA(const char* c = "All\0*.*\0\0", const std::string& defNam = "*.*");