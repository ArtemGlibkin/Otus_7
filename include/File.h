#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "FileMonitorParameters.h"
#include "Hasher.h"

class File
{
	std::string mPath;
	std::string hash;

	std::string readFile()
	{
		std::ifstream istream(mPath);
		if (!istream.is_open())
			throw std::runtime_error("File " + mPath + "is not exist or lack rights");
		std::stringstream sstream;
		for (std::string line; std::getline(istream, line); )
			sstream << line;
		return sstream.str();
	}
public:
	File(const std::string& path, HashAlgorithms algo) : mPath(path)
	{
		hash = getHasher(algo)->getDigest(readFile());
	}

	const std::string& getPath() const
	{
		return mPath;
	}

	const std::string& getDigest() const
	{
		return hash;
	}
};

bool operator<(const File& file, const File& file2)
{
	return file.getDigest() < file2.getDigest();
}

