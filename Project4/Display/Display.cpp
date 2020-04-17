/////////////////////////////////////////////////////////////////////////////
// Display.cpp - Display the html in a directory.                          //
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
#include "Display.h"
#include "../Converter/Converter.h"
#include <iostream>
#include <conio.h>


using namespace Convert;

int main()
{
	std::cout << "\n  Demonstrating the html in the directory";
	std::cout << "\n ======================================";
	Process p;
	p.title("Display application");
	std::string appPath = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe";  // path to application to start
	std::string abbPath = "../ConvertedWebpages";
	std::string ffpath = "../Parser/Parser.h";
	std::cout << "  the test file path is: " + ffpath;
	/*ContentConvert cc;
	cc.fileToHtml1(ffpath);
	p.displayDir(appPath, abbPath);*/
	//return 0;
}

//#endif
