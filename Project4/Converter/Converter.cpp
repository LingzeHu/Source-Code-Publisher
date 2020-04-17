/////////////////////////////////////////////////////////////////////////////
// Converter.cpp - Converter the source files into html file.              //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Lingze Hu, 2019                                             //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2017                             //
// Platform:    Microsoft Surface Pro, Core i5, Windows 10                 //
// Application: Summer Projects, 2012                                      //
// Author:      Lingze Hu, Syracuse University                             //
//              lhu101@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
#include<iostream>
#include "Converter.h"
#include "../DirExplorer-Naive/FileSystem.h"
#include "../DirExplorer-Naive/StringUtilities.h"

using namespace Utilities;
using namespace Convert;
using namespace FileSystem;

void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}

int main(int argc, char *argv[])
{
	std::cout << "  test the function that convert the file into html one ";
	std::string ffpath = "../AbstractSyntaxTree/AbstrSynTree.h";
	std::cout << "  the test file path is: " + ffpath;
	Convert_H convertor;
	convertor.fileToHtml(ffpath);

	//return 0;
}