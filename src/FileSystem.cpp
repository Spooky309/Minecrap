#include "FileSystem.h"
#include <cstring>
#ifdef WIN
#include <windows.h>
#else
#include <unistd.h>
#include <linux/limits.h>
#endif
FileSystem::FileSystem()
{
	#ifdef WIN
	LPSTR buf = (LPSTR)malloc(MAX_PATH);
	GetModuleFileNameA(nullptr, buf, MAX_PATH);
	char* lastSlash = strrchr(buf, '\\') + 1;
	#else
	char* buf = (char*)malloc(PATH_MAX);
	readlink("/proc/self/exe", buf, PATH_MAX);
	char* lastSlash = strrchr(buf, '/') + 1;
	#endif
	*lastSlash = 0; // null terminate early
	exePath = std::string(buf);
	free(buf);
}

std::string FileSystem::GetAbsPathTo(const std::string& name)
{
	return exePath + name;
}
