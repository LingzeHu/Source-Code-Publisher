#pragma once
///////////////////////////////////////////////////////////////////////
// Dependencies .h -                                                 //
// ver 1.0                                                           //
// Lingze Hu, CSE687 - Object Oriented Design, Spring 2018           //
///////////////////////////////////////////////////////////////////////
/*
*  Package Desciption:
*  -------------------
*  This package support insertion, in each analyzed file, 
*  links to all the files in the directory tree, on which it depends. 
*
*  Required Files:
*  ---------------
*  Dependencies.h, Dependencies.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 8 Mar 2019
*  - first release
*/
#include <iostream>
#include <fstream>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../DirExplorer-Naive/FileSystem.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../Converter/Converter.h"


using namespace Lexer;
using namespace Utilities;
using namespace Convert;
using Demo = Logging::StaticLogger<1>;

#include "../FileSystem/FileSystem.h"
#include <queue>
#include <string>
#define Util StringHelper
using namespace CodeAnalysis;
//using namespace Utilities;
using namespace FileSystem;

namespace Dependencies
{
	class Dependency
	{
	public:
		Dependency();
		~Dependency();

		std::vector<std::string> getStatements(const std::string& ffpath);
		std::string insertStatement(std::string content, std::vector<std::string> statements_);
		void linkDependencies(const std::string& ffpath);

	private:

	};

	Dependency::Dependency(){	}
	Dependency::~Dependency()	{	}

	// By inputing the full file path of a source file , this function return the dependencies of this file.
	inline std::vector<std::string> Dependency::getStatements(const std::string& ffpath)
	{
		ConfigParseForCodeAnal configure;
		Parser* pParser = configure.Build();
		std::vector<Lexer::ITokenCollection*> statements_;
		std::vector<std::string> res;
		std::string name;
		try
		{
			if (pParser){
				name = FileSystem::Path::getName(ffpath);
				if (!configure.Attach(ffpath))
				{
					std::cout << "\n  could not open file " << name << std::endl;

				}
			}else{	std::cout << "\n\n  Parser not built\n\n";	}
			Repository* pRepo = Repository::getInstance();
			pRepo->package() = name;
			while (pParser->next())	{	pParser->parse();	}
			ASTNode* pGlobalScope = pRepo->getGlobalScope();
			statements_ = pGlobalScope->statements_;
			for (auto statement : statements_) {
				std::string s = statement->ToString();
				std::size_t begin = s.find_first_of("\"");
				std::size_t end = s.find_last_of("\"");
				if (end - begin) {
					std::size_t pos = s.find_last_of("/");
					if (pos != 4294967295) // unsigned int
					{
						s = s.substr(pos + 1, end - pos - 1);
					}
					else
					{
						s = s.substr(begin + 1, end - begin - 1);
					}
					res.push_back(s);
				}
			}
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
			std::cout << "\n  exception caught at line " << __LINE__ << " ";
			std::cout << "\n  in package \"" << name << "\"";
		}
		return res;
	}

	// Construct the dependencies link to the statemenets.
	inline std::string Dependency::insertStatement(std::string content, std::vector<std::string> statements_)
	{
		std::string fStr = "<h4>Dependency Link : ";
		if (statements_.empty())
		{
			fStr += "None";
		}
		else {
			for (auto statement : statements_)
			{
				if (statement.rfind("\""))
				{
					size_t pos = statement.rfind("\"");
					std::string s = statement.substr(pos + 1, statement.length());
					fStr += "<a href=\"";
					fStr += s;
					fStr += ".html\" >";
					fStr += s;
					fStr += "</a>&nbsp&nbsp";
				}
				else
				{
					fStr += "<a href=\"";
					fStr += statement;
					fStr += ".html\" >";
					fStr += statement;
					fStr += "</a>&nbsp&nbsp";
				}
				
			}
		}
		fStr += "</h4>";
		fStr += content;
		return fStr;

	}
	
	//Test Function
	inline void Dependency::linkDependencies(const std::string& ffpath)
	{
		std::string content;
		std::vector<std::string> statements_;
		Convert_H coverter;
		content = coverter.ReadFileString(ffpath);
		content = coverter.InsterPreTag(content);
		statements_ = getStatements(ffpath);
		content = insertStatement(content, statements_);
		coverter.ConvertToHtml(ffpath, content);
	}
}