// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Microsoft Surface Pro, Core i5, Windows 10                 //
// Application: Summer Projects, 2012                                      //
// Author:      Lingze Hu, Syracuse University                             //
//              lhu101@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
#include<iostream>
#include "../Converter/Converter.h"
#include "../DirExplorer-Naive/FileSystem.h"
#include "../DirExplorer-Naive/StringUtilities.h"
#include "Dependencies.h"

using namespace Utilities;
using namespace Convert;
using namespace FileSystem;
using namespace Dependencies;

void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}

int main(int argc, char *argv[])
{
	std::cout << "  test the function that convert the file into html one ";
	std::string ffpath = "../Parser/ActionsAndRules.h";
	std::cout << "  the test file path is: " + ffpath;
	
	Dependency de;
	de.linkDependencies(ffpath);

	return 0;
}