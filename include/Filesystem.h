#pragma once
#include <string>
class Filesystem
{
public:
	static Filesystem& Instance()
	{
		static Filesystem _inst;
		return _inst;
	}
	std::string GetAbsPathTo(const std::string& name);
private:
	Filesystem();
	~Filesystem() = default;
	Filesystem(const Filesystem&) = delete;
	std::string exePath;
};