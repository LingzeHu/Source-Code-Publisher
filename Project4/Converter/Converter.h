#pragma once
///////////////////////////////////////////////////////////////////////
// Converter.h - class to process source files into html files       //
// ver 1.0                                                           //
// Lingze Hu, CSE687 - Object Oriented Design, Spring 2018           //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package has three main function to achieve the goal.
*  1. turn the source file content into string.
*  2. add <pre> tag into content string.
*  3. create html files by using webpage template.
*
*  Required Files:
*  ---------------
*  Coverter.h, Converter.cpp, FileSystem.h
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 5 Feb 2019
*  - first release
*/
#include <iostream>
#include <fstream>
#include "../Utilities/Utilities.h"
//#include "../Tokenizer/Toker.h"
//#include "../SemiExpression/Semi.h"
#include "../DirExplorer-Naive/FileSystem.h"
//#include "../Parser/Parser.h"
//#include "../Parser/ActionsAndRules.h"
//#include "../Parser/ConfigureParser.h"


//using namespace Lexer;
using namespace Utilities;
//using Demo = Logging::StaticLogger<1>;

#include "../FileSystem/FileSystem.h"
#include <queue>
#include <string>
#define Util StringHelper
//using namespace CodeAnalysis;
//using namespace Utilities;
using namespace FileSystem;


namespace Convert
{
	

	class Convert_H
	{
	public:
		Convert_H();
		~Convert_H();
		std::string replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);
		std::string ReadFileString(const std::string& ffpath);
		std::string InsterPreTag(const std::string& str);
		void ConvertToHtml(const std::string& fpath, const std::string& fStr);
		void fileToHtml(const std::string& ffpath);

	private:
		
	};

	Convert_H::Convert_H()
	{
	}

	Convert_H::~Convert_H()
	{
	}


	// the function used to replace all the string old_value by string new_value
	inline std::string Convert_H::replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value)
	{
		for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
			if ((pos = str.find(old_value, pos)) != std::string::npos)
				str.replace(pos, old_value.length(), new_value);
			else   break;
		}
		return   str;
	}

	// turn the .h and .cpp file into string
	inline std::string Convert_H::ReadFileString(const std::string& ffpath)
	{
		std::string fStr;
		File in(ffpath);
		in.open(File::in, File::text);
		while (in.isGood())
		{

			fStr = in.readAll(true);
			fStr = replace_all_distinct(fStr, "<", "&lt");
			fStr = replace_all_distinct(fStr, ">", "&gt");
		}
		return fStr;
	}

	// For Pr1, insert the <pre> tag in the content
	inline std::string Convert_H::InsterPreTag(const std::string& str)
	{
		std::string fStr = "<pre>" + str + "</pre>";
		return fStr;
	}

	// For Pr1, insert the string in to HTML template
	inline void Convert_H::ConvertToHtml(const std::string& fpath, const std::string& fStr){
		std::string fName = Path::getName(fpath, true);
		std::string fileOut = "..\\" + fName + ".html";
		//std::string tempPath = "../../../temp.txt";
		std::string tempPath = "../temp.txt";
		std::string fString, header;
		//std::string targetPath = "../../../ConvertedWebpages/";
		std::string targetPath = "../ConvertedWebpages/";
		std::string fPath = Path::getFullFileSpec(targetPath);
		//std::cout << fPath;
		//std::string fString, header;
		std::size_t pos;
		header = "<h3>" + fName + "</h3>";
		std::string dir(targetPath);
		if (!FileSystem::Directory::exists(dir)) // it's always important to verify if the directory already exists
			FileSystem::Directory::create(dir);
		// now you can combine it with filenames
		std::string fileName = fName + ".html";
		std::string filePath = dir + "/" + fileName;
		std::ofstream fileStream(filePath); // directory exists so this will create new file in the directory if the file didn't already exist		

		std::string fullTempPath = Path::getFullFileSpec(tempPath);

		File in(tempPath);
		in.open(File::in, File::text);
		while (in.isGood()){
			fString = in.readAll();
			pos = fString.find("</body>", 0);
			fString = fString.insert(pos - 1, header + fStr);
		}
		fileStream << fString;
		fileStream.close();
		std::cout << "  " + fileName + " converted successful!\n";
	}

	// For Pr1, combine the function
	inline void Convert_H::fileToHtml(const std::string& ffpath)
	{
		std::string content;
		content = ReadFileString(ffpath);
		content = InsterPreTag(content);
		ConvertToHtml(ffpath, content);
	}

	//---------------------the functions above origined from Project 1----------------------------------------//
	


}