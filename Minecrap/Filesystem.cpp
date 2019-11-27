#include "Filesystem.h"
#include <cstring>
Filesystem::Filesystem()
{
	LPSTR buf = (LPSTR)malloc(MAX_PATH);
	GetModuleFileNameA(nullptr, buf, MAX_PATH);
	char* lastSlash = strrchr(buf, '\\') + 1;
	*lastSlash = 0; // null terminate early
	exePath = std::string(buf);
	free(buf);
}

std::string Filesystem::GetAbsPathTo(const std::string& name)
{
	return exePath + name;
}