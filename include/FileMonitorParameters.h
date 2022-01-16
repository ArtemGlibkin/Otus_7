#pragma once
#include <vector>
#include <string>
#include <list>

using Directories = std::vector<std::string>;
using FilesMasks = std::vector<std::string>;

enum class HashAlgorithms
{
	MD5,
	SHA1
};

struct FileMonitorParameters
{
	Directories includeDirectories;
	Directories excludeDirectories;
	bool nestedDirectories;
	HashAlgorithms algorithm;
	FilesMasks filesMasks;
	int minimalFileSize;
	int blockSize;
};