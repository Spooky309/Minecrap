#pragma once
#include <string>
class FileSystem
{
public:
	friend class Engine;
	std::string GetAbsPathTo(const std::string& name);
private:
	FileSystem();
	~FileSystem() = default;
	FileSystem(const FileSystem&) = delete;
	std::string exePath;
};