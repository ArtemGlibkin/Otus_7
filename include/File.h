#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "FileMonitorParameters.h"
#include "Hasher.h"
#include <boost/coroutine/all.hpp>

class File
{
	std::string mPath;
	std::list < std::string> mDigest;
	std::unique_ptr<Hasher> mHasher;
	std::ifstream istream;
	int mBlockSize = 0;
	const std::string empty = "empty";
public:
	File(const std::string& path, HashAlgorithms algo, int block_size = 1) : mPath(path) , mBlockSize(block_size)
	{
		mHasher = getHasher(algo);
	}

	const std::string& getPath() const
	{
		return mPath;
	}

	void getDigest(boost::coroutines::coroutine<std::string>::push_type& sink)
	{
		for (auto it : mDigest)
			sink(it);
		
		std::unique_ptr<char[]> block;
		while (readBlock(block))
		{
			std::string digest = mHasher->getDigest(block, mBlockSize);
			mDigest.push_back(digest);
			sink(digest);
		}
		sink(empty);
	}

	bool readBlock(std::unique_ptr<char[]> & result)
	{
		if (!istream.is_open())
			istream.open(mPath);
		std::unique_ptr<char[]> buffer = std::make_unique<char[]>(mBlockSize);
		istream.read(buffer.get(), mBlockSize);
		auto reading = istream.gcount();
		if (reading == 0)
			return false;
		if (reading < mBlockSize)
		{
			for (auto i = reading; i < mBlockSize; i++)
				buffer[i] = '\0';
		}

		result = std::move(buffer);
		return true;
	}

	bool compareFiles(File& file2)
	{
		boost::coroutines::coroutine<std::string>::pull_type source{std::bind(&File::getDigest, this, std::placeholders::_1)};
		boost::coroutines::coroutine<std::string>::pull_type source2{std::bind(&File::getDigest, &file2, std::placeholders::_1)};
		
		while (source.get() == source2.get())
		{
			bool ind1 = (source.get() == empty);
			bool ind2 = (source2.get() == empty);
			if (ind1 && ind2)
				return true;
			else if (ind1 || ind2)
				return false;

			source();
			source2();
		}
		return source.get() == source2.get();
	}
};

bool operator<(const File& file, const File& file2)
{
	return file.getPath() < file2.getPath();
}

