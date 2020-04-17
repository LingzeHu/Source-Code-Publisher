#pragma once
///////////////////////////////////////////////////////////////
// Executive.h -  Translates managed calls into native calls //
//                                                           //
// Lingze Hu,   CSE687 - Object Oriented Design, Spring 2019 //
///////////////////////////////////////////////////////////////
/*
*  Package Desciption:
*  -------------------
*  This file defines a private class, Publisher, that implements the
*  IPublisher interface.  Clients only need to use the createPublisherCpp
*  factory function and the interface pointer it returns.
*
*  Required Files:
*  ---------------
*  IExecutive.h, Executive.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 13 Apr 2019
*  - first release
*/
#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../DirExplorer-Naive/StringUtilities.h"
#include "../DirExplorer-Naive/CodeUtilities.h"
#include "../Regex/Regex.h"
#include "../Converter/Converter.h"
#include "../Display/Display.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "IExecutive.h"


using namespace Lexer;
using namespace Utilities;
using Demo = Logging::StaticLogger<1>;

#include "../FileSystem/FileSystem.h"
#include <queue>
#include <string>
#define Util StringHelper

using namespace CodeAnalysis;
//using namespace Utilities;
using namespace FileSystem;
using namespace Reg;
using namespace Convert;

class Publisher : public IPublisher
{
public:
	virtual std::string getName(const std::string &fileSpec) override
	{
		return FileSystem::Path::getName(fileSpec);
	}
	virtual std::string getFullFileSpec(const std::string &fileSpec) override
	{
		return Path::getFullFileSpec(fileSpec);
	}

	virtual std::vector<std::string> getFileList_(const std::string &path, std::vector<std::string> patterns, std::vector<std::string> regexes) override
	{
		std::vector<std::string> fileList;
		Regex reg;
		DirExplorerN de(path);
		if (!patterns.empty() && patterns[0] != "") 
		{
			for (auto patt : patterns)
			{
				de.addPattern(patt);
			}
		}		
		de.recurse();
		de.search1();
		fileList = de.getFileList();
		if (!regexes.empty() && regexes[0] != "")
		{
			for (auto rege : regexes)
			{
				fileList = reg.RegexFilter(fileList, rege);
			}
		}
		return fileList;
	}

	virtual void publishFileList(std::vector<std::string> fileList) override
	{
		ContentConvert cc;
		std::vector<std::string> fileNameList;
		for (auto file : fileList)
		{
			std::string fileName = getName(file);
			fileNameList.push_back(fileName);
		}

		for (auto file : fileList)
		{
			cc.fileToHtml1(file, fileNameList);
		}
	}

	// for Pr4, to publish a single file
	virtual void publishFile(const std::string &filePath)
	{
		ContentConvert cc;
		std::vector<std::string> fileNameList;
		std::string fileName = getName(filePath);
		fileNameList.push_back(fileName);
		cc.fileToHtml1(filePath, fileNameList);

	}
};

IPublisher* createPublisherCpp()
{
	return new Publisher();
}