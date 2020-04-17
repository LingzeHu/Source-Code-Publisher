#pragma once
///////////////////////////////////////////////////////////////////////
// Regex.h - class to filter the source files by regex experssions   //
// ver 1.0                                                           //
// Lingze Hu, CSE687 - Object Oriented Design, Spring 2018           //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package is mainly to implement the regex to filter the source files.
*
*  Required Files:
*  ---------------
*  Regex.h, Regex.cpp, FileSystem.h
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 5 Feb 2019
*  - first release
*/
#include <vector>
#include <iostream>
#include <iterator>
#include <string>
#include <regex>
#include "../DirExplorer-Naive/FileSystem.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../DirExplorer-Naive/StringUtilities.h"
#include "../DirExplorer-Naive/CodeUtilities.h"
//#include "../DirectoryNavigator/FileSystem/FileSystem.h"

using namespace FileSystem;

namespace Reg
{
	class  Regex
	{
	public:
		using fFilePath = std::vector<std::string>;
		fFilePath RegexFilter(fFilePath& ffPath, const std::string& filter);

		Regex();
		~Regex();

	private:
		fFilePath fFilePath_;
	};

	Regex::Regex()
	{
	}

	Regex::~Regex()
	{
	}

	// ffpath: full file path, filter: regex expression.
	inline std::vector<std::string> Regex::RegexFilter(fFilePath& ffPath, const std::string& filter)
	{
		std::vector<std::string> fileNames, result;
		std::string name;
		std::regex pattern(filter, std::regex::icase);
		std::match_results<std::string::const_iterator> results;

		for (auto fpath : ffPath)
		{
			name = Path::getName(fpath, true);
			if (std::regex_match(name, results, pattern))
			{
				result.push_back(fpath);
			}
		}

		return result;
	}

}