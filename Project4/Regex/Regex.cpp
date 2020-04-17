/////////////////////////////////////////////////////////////////////////////
// Regex.cpp - Implement the regex expression to filter the files.         //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Lingze Hu, 2019                                             //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Microsoft Surface Pro, Core i5, Windows 10                 //
// Author:      Lingze Hu, Syracuse University                             //
//              lhu101@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
#include "Regex.h"

using namespace Utilities;
using namespace FileSystem;
using namespace Reg;

ProcessCmdLine::Usage customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]*";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [/option]* are one or more options of the form:";
	usage += "\n      /s - walk directory recursively";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n";
	return usage;
}

int main(int argc, char *argv[])
{
	std::vector<std::string> fileList, res;
	Regex reg;
	//std::string filter = "(D)(.*).h";
	std::string appPath = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe";  // path to application to start
	std::string abbPath = "../ConvertedWebpages";
	std::string fileString;
	ProcessCmdLine pcl(argc, argv);
	pcl.usage(customUsage());	
	if (pcl.parseError())	{
		pcl.usage();
		std::cout << "\n\n";
		return 1;
	}

	DirExplorerN de(pcl.path());
	for (auto patt : pcl.patterns())	{
		de.addPattern(patt);
	}
	if (pcl.hasOption('s'))	{		de.recurse();	}
	de.search1();
	de.showStats();
	fileList = de.getFileList();
	if (fileList.size() == 0)
	{
		std::cout << "\n  Fail to search the file.";
		std::cout << "===================================================================================";
	}else{
		if (!pcl.regexes().empty())
		{			
			for (auto rege : pcl.regexes())
			{
				fileList = reg.RegexFilter(fileList, rege);
			}
		}
	}
	return 0;

}

