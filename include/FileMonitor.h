#pragma once

#include <list>
#include <string>
#include <iostream>
#include <set>
#include <map>

#include "File.h"
#include <filesystem>
#include "FileMonitorParameters.h"
#include <boost/regex.hpp>

class FileMonitor
{	
	std::list<std::string> mDirectoryPathes;
	std::set<std::filesystem::path> mExcludedPathes;
	bool mNestedSearch;
	std::list<File> mFileSet;
	HashAlgorithms mHashAlgo;
	FilesMasks mFilesMasks;
	unsigned int mMinimalFileSize = 1;
public:
	FileMonitor(const FileMonitorParameters& parameters) : mNestedSearch(parameters.nestedDirectories),
														   mHashAlgo(parameters.algorithm),
														   mMinimalFileSize(parameters.minimalFileSize),
														   mFilesMasks(parameters.filesMasks)
	{
		for (auto it : parameters.excludeDirectories)
		{
			std::filesystem::path path = it;
			mExcludedPathes.insert(std::filesystem::absolute(path));
		}

		for (auto it : parameters.includeDirectories)
		{
			std::filesystem::path path = it;
			mDirectoryPathes.push_back(std::filesystem::absolute(path).string());
		}
	}

	bool isFileMatchMasks(const std::string & fileName)
	{
		if (mFilesMasks.empty())
			return true;

		for (auto maskit : mFilesMasks)
		{
			if (boost::regex_match(fileName.c_str(), boost::regex(maskit.c_str())))
				return true;
		}
		return false;
	}

	void scan()
	{
		for (auto it : mDirectoryPathes)
		{
			std::filesystem::path path = it;

			if (mExcludedPathes.find(std::filesystem::absolute(path)) != mExcludedPathes.end())
				continue;

			for (auto const& entry : std::filesystem::directory_iterator{ path })
			{
				if (entry.is_regular_file() && (std::filesystem::file_size(entry.path().string()) > mMinimalFileSize) && isFileMatchMasks(entry.path().filename().string()))
				{
					//File file(entry.path().string(), mHashAlgo);
					mFileSet.push_back(File(entry.path().string(), mHashAlgo));
				}
				else if (mNestedSearch && entry.is_directory())
					mDirectoryPathes.push_back(std::filesystem::absolute(entry.path()).string());
			}
		}
	}
	
	void result()
	{
		for (auto it = mFileSet.begin(); it != mFileSet.end(); it++)
		{
			bool find = false;
			for (auto it2 = std::next(it, 1); it2 != mFileSet.end(); it2++)
			{
				if (it->compareFiles(*it2))
				{
					if (!find)
					{
						std::cout << it->getPath() << std::endl;
						find = true;
					}
					std::cout << it2->getPath() << std::endl;
					it2 = mFileSet.erase(it2);
				}
			}
			if (find)
				std::cout << std::endl;
		}
	}

};
