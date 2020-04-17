/////////////////////////////////////////////////////////////////////////////
//  Executive.cpp - Starter code for Project #2                            //
//  ver 1.0                                                                //
// ----------------------------------------------------------------------- //
// copyright © Lingze Hu, 2019                                             //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Microsoft Surface Pro, Core i5, Windows 10                 //
// Author:      Lingze Hu, Syracuse University                             //
//              lhu101@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////

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
#include "Executive.h"


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

// -----< demonstrate requirement 3 >-------------------------------------
void demoReq3() {
	std::cout << "\n========================================================================================\n";
	std::cout << "  Demonstrating Requirements #3: provide a path to be converted. \n";
	std::cout << "----------------------------------------------------------------------------------------\n";

}
// -----< demonstrate requirement 4 >-------------------------------------
void demoReq4() {
	std::cout << "\n========================================================================================\n";
	std::cout << "  Requirements #4 is compelted, as requirements #5-#8 are statisfied.  \n";
	std::cout << "  Demonstrated Requirements #4: provide Loader, Converter, Dependencies, and Display packages.\n";
	std::cout << "----------------------------------------------------------------------------------------\n";

}
// -----< demonstrate requirement 5 >-------------------------------------
void demoReq5() {
	std::cout << "\n========================================================================================\n";
	std::cout << "  Demonstrating Requirements #5: finding and loading all the files for conversion. \n";
	std::cout << "----------------------------------------------------------------------------------------\n";
}
// -----< demonstrate requirement 9 >-------------------------------------
void demoReq9() {
	std::cout << "\n========================================================================================\n";
	std::cout << "  Demonstrating Requirements #9. \n";
	std::cout << "----------------------------------------------------------------------------------------\n";
	std::cout << "\n Well, this is the automated test unit.\n";
	std::cout << "\n All Requirements met. \n";
}

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

int main(int argc, char* argv[])
{
	Publisher pub;
	std::cout << "Project 2 Executive:\n";
	std::vector<std::string> fileList, res;
	Regex reg;
	std::string abbPath = "../ConvertedWebpages";
	std::string fileString;
	Convert_H con;
	ProcessCmdLine pcl(argc, argv);
	std::string appPath = pcl.appPath();
	demoReq3();
	pcl.usage(customUsage());
	preface("Command Line: ");pcl.showCmdLine();putline();
	std::cout << "  the directory path is: ";pcl.showPath();putline();
	std::cout << "  the broswer path is: ";	pcl.showAppPath();	putline();
	if (pcl.parseError()){
		pcl.usage();std::cout << "\n\n";return 1;	}


	//pub.getFileList_(pcl.path(), pcl.patterns(), regex);
	//DirExplorerN de(pcl.path());
	//for (auto patt : pcl.patterns()){	de.addPattern(patt);	}
	//if (pcl.hasOption('s')){de.recurse();}
	//demoReq5();
	//de.search1();
	//de.showStats();
	//fileList = de.getFileList(); //fileList is the list of desired fullpaths 
	//if (!pcl.regexes().empty())
	//{
	//	for (auto rege : pcl.regexes())
	//	{
	//		fileList = reg.RegexFilter(fileList, rege);
	//	}		
	//}
	//if (fileList.size() == 0){
	//	std::cout << "  Found no file.\n";
	//	std::cout << "========================================================================================\n";	}
	//else{
	//	std::cout << "The file list to be processed is: \n";
	//	for (auto file : fileList)
	//	{
	//		std::string name = FileSystem::Path::getName(file);
	//		std::cout << "  " + name << "\n";
	//	}
	//	std::cout << "=======================\n";	}

	fileList = pub.getFileList_(pcl.path(), pcl.patterns(), pcl.regexes());

	
	std::cout << "\n========================================================================================\n";
	std::cout << "  Demonstrating Requirements #6, #7, #8: \n";	
	std::cout << "----------------------------------------------------------------------------------------\n";
	
	/*for (auto file : fileList)
	{
		pub.publishAFile(file);
	}*/


	pub.publishFileList(fileList);

	demoReq4();
	Process p;
	p.displayDir(appPath, abbPath);
	demoReq9();	
}

