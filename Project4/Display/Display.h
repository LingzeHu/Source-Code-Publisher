#pragma once
///////////////////////////////////////////////////////////////////////
// Display.h - class based on display used to start process          //
// ver 1.0                                                           //
// Lingze Hu, CSE687 - Object Oriented Design, Spring 2018           //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package based on Fawcett's Prcess.h.
*  What's added are the function that turn absolute path to relative path.
*  Also it open the html files in directory.
*
*  Required Files:
*  ---------------
*  Display.h, Display.cpp, FileSystem.h, StringUtilities.h
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 5 Feb 2019
*  - first release
*/

#include <windows.h>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <limits.h>
#include "../DirExplorer-Naive/FileSystem.h"
#include "../DirExplorer-Naive/StringUtilities.h"
#include "../Converter/Converter.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../Dependencies/Dependencies.h"


using namespace FileSystem;
using namespace Utilities;
using namespace Lexer;
using namespace Utilities;
using namespace Convert;
using namespace Dependencies;
using Demo = Logging::StaticLogger<1>;


using CBP = std::function<void(void)>;

struct TreeWalkResut
{
	std::string type;
	size_t startLine;
	size_t endLine;
};
std::vector<TreeWalkResut> walkRes;

class ContentConvert {
public:
	ContentConvert() {};
	~ContentConvert() {};

	std::string handleComment(const std::string& str);
	std::string handleClassandFunction(const std::string& content, const std::string& ffpath);
	std::string processContentTag(const std::string& content, std::vector<TreeWalkResut> walkres);
	void getClassandFunc(ASTNode* pItem);
	void fileToHtml1(const std::string& ffpath, std::vector<std::string> fileNames_);
	std::string insertButton(const std::string& content);
	std::vector<std::string> statementFilter(const std::vector<std::string> &statememts_, const std::vector<std::string>& fileNames_);

private:

};

class Process
{
public:
	Process();
	virtual ~Process() {}
	bool create(const std::string& path = "");
	void title(const std::string& title);
	void application(const std::string& app);
	void commandLine(const std::string& cmdLine);
	void registerCallback();
	void setCallBackProcessing(CBP cbp);
	void callback();
	const std::string getAbsPath(const std::string& abbPath);
	void displayDir(const std::string& appPath, const std::string& abbPath);

	HANDLE getProcessHandle();
private:
	STARTUPINFO startInfo_;
	PROCESS_INFORMATION procInfo_;
	std::wstring title_;
	std::wstring appName_;
	std::wstring commandLine_;
	static CBP cbp_;
};

CBP Process::cbp_ = []() { std::cout << "\n  --- child process exited ---"; };

//----< Process constructor >------------------------------------------
/*
* - This eventually will have more functionality
*/
inline Process::Process()
{
	GetStartupInfo(&startInfo_);
	startInfo_.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
	startInfo_.dwX = 200;
	startInfo_.dwY = 250;
	startInfo_.dwYSize = 300;
}
//----< helper function to convert strings >---------------------------

std::string wToS(const std::wstring& ws)
{
	std::string conv;
	for (auto ch : ws)
	{
		conv.push_back(static_cast<char>(ch));
	}
	return conv;
}
//----< helper function to convert strings >---------------------------

std::wstring sToW(const std::string& s)
{
	std::wstring conv;
	for (auto ch : s)
	{
		conv.push_back(static_cast<wchar_t>(ch));
	}
	return conv;
}
//----< define application new process runs >--------------------------

inline void Process::application(const std::string& appName)
{
	appName_ = sToW(appName);
}
//----< define commandline passed to new application >-----------------

inline void Process::commandLine(const std::string& cmdLine)
{
	commandLine_ = sToW(cmdLine);
}
//----< define title bar for console window >--------------------------
/*
* - applications like Notepad overwrite this string
*/
inline void Process::title(const std::string& title)
{
	title_ = sToW(title);
	startInfo_.lpTitle = const_cast<LPWSTR>(title_.c_str());
}
//----< start new child process >--------------------------------------

inline bool Process::create(const std::string& appName)
{
	std::wstring app;
	if (appName.size() == 0)
		app = appName_;
	else
	{
		app = sToW(appName);
	}
	LPCTSTR applic = app.c_str();
	applic = NULL;
	LPTSTR cmdLine = const_cast<LPTSTR>(commandLine_.c_str());
	LPSECURITY_ATTRIBUTES prosec = nullptr;
	LPSECURITY_ATTRIBUTES thrdsec = nullptr;
	BOOL inheritHandles = false;
	DWORD createFlags = CREATE_NEW_CONSOLE;  // or CREATE_NO_WINDOW
	LPVOID environment = nullptr;
	LPCTSTR currentPath = nullptr;
	LPSTARTUPINFO pStartInfo = &startInfo_;
	LPPROCESS_INFORMATION pPrInfo = &procInfo_;

	BOOL OK =
		CreateProcess(
			applic, cmdLine, prosec, thrdsec, inheritHandles,
			createFlags, environment, currentPath, pStartInfo, pPrInfo
		);
	return OK;
}

///////////////////////////////////////////////////////////////////////
// child process exit callback processing

//----< provide new callable object for callback to invoke >-----------

void Process::setCallBackProcessing(CBP cbp)
{
	cbp_ = cbp;
}
//----< function called when child process exists >--------------------

void Process::callback()
{
	try {
	cbp_();
	}
	catch (const std::bad_function_call& ) {
		//std::cout << e.what() << '\n';
	}
}
//----< Windows API declared function type for callbacks >-------------

void CALLBACK WaitOrTimerCallback(_In_ PVOID lpParameter, _In_ BOOLEAN TimerOrWaitFired)
{
	Process p;
	p.callback();
	return;
}
//----< register callback with Windows API >---------------------------

void Process::registerCallback()
{
	HANDLE hNewHandle;
	HANDLE hProcHandle = procInfo_.hProcess;
	RegisterWaitForSingleObject(
		&hNewHandle, hProcHandle, WaitOrTimerCallback,
		NULL, INFINITE, WT_EXECUTEONLYONCE
	);
}

HANDLE Process::getProcessHandle()
{
	return procInfo_.hProcess;
}

//  take the browser's path and absolute path of directory, open the html files in it.
inline void Process::displayDir(const std::string& appPath, const std::string& abbPath)
{
	std::string cmdLine;
	Process p;
	std::string dirPath = p.getAbsPath(abbPath);
	std::vector<std::string> htmlList = Directory::getFiles(dirPath, "*.html");
	p.application(appPath);
	std::cout << "===================================================================================\n";
	std::cout << "Opening the converted HTML files...";
	for (auto htmlFile : htmlList)
	{
		cmdLine = appPath + " /A " + dirPath + "/" + htmlFile;
		p.commandLine(cmdLine);
		p.create();
		std::cout << "  opening " << htmlFile << ".\n";
		CBP callback = []() {/* std::cout << "\n  --- child process exited with this message ---";*/ };
		p.setCallBackProcessing(callback);
		p.registerCallback();
		WaitForSingleObject(p.getProcessHandle(), INFINITE);  // wait for created process to terminate
	}

}
//  turn the relative path into absolute path.
inline const std::string Process::getAbsPath(const std::string& abbPath)
{
	char abs_path[512];
	_fullpath(abs_path, abbPath.c_str(), 512);
	std::string abs(abs_path);
	return abs;

}


// By inputing string of content, add comment div to comments.
inline std::string ContentConvert::handleComment(const std::string& str)
{
	int n = str.length();
	std::string res;

	// Flags to indicate that single line and multpile line comments 
	// have started or not. 
	bool s_cmt = false;
	bool m_cmt = false;
	for (int i = 0; i < n; i++)	{
		// If single line comment flag is on, then check for end of it 
		if (s_cmt == true && str[i] == '\n')
			//res += "\n </div>", s_cmt = false;
			res += "</div>", s_cmt = false;


		// If multiple line comment is on, then check for end of it 
		else if (m_cmt == true && str[i] == '*' && str[i + 1] == '/')
			res += "*/ </div>", m_cmt = false, i++;

		// Check for beginning of comments and set the approproate flags 
		else if (str[i] == '/' && str[i + 1] == '/' && s_cmt == false)
			res += "<div class = \"Comment\"> //", s_cmt = true, i++;
		else if (str[i] == '/' && str[i + 1] == '*' && m_cmt == false)
			res += "<div class = \"Comment\"> /*", m_cmt = true, i++;

		// If current character is a non-comment character, append it to res 
		else  res += str[i];
	}
	return res;
}


// By inputing a string of source file and its full path, it will add div to content of function and class.
inline std::string ContentConvert::handleClassandFunction(const std::string& content, const std::string& ffpath)
{
	ConfigParseForCodeAnal configure;
	Parser* pParser = configure.Build();
	std::string name;
	std::string res;
	try
	{
		if (pParser){
			name = FileSystem::Path::getName(ffpath);
			if (!configure.Attach(ffpath)){
				std::cout << "\n  could not open file " << name << std::endl;
			}
		}
		else{
			std::cout << "\n\n  Parser not built\n\n";
		}
		Repository* pRepo = Repository::getInstance();
		pRepo->package() = name;
		while (pParser->next())	{
			pParser->parse();
		}
		//std::cout << "\n";
		ASTNode* pGlobalScope = pRepo->getGlobalScope();
		getClassandFunc(pGlobalScope);
		res = processContentTag(content, walkRes);
		return res;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		std::cout << "\n  exception caught at line " << __LINE__ << " ";
		std::cout << "\n  in package \"" << name << "\"";
	}
	return res;
}

// Get Class and Function's information by parsering
inline void ContentConvert::getClassandFunc(ASTNode* pItem)
{
	TreeWalkResut res;
	if (pItem->type_ == "class" || pItem->type_ == "function")
	{
		res.type = pItem->type_;
		res.startLine = pItem->startLineCount_;
		res.endLine = pItem->endLineCount_;
		walkRes.push_back(res);
	}
	auto iter = pItem->children_.begin();
	while (iter != pItem->children_.end())
	{
		getClassandFunc(*iter);
		++iter;
	}
}

// By having content of source file and its class/function information, insert div tag.
inline std::string ContentConvert::processContentTag(const std::string& content, std::vector<TreeWalkResut> walkres)
{
	std::string res = content;
	bool funFlag = false;
	bool classFlag = false;

	for (auto walk : walkres){
		int linecount = 0;
		for (size_t i = 0; i < res.length(); i++){
			if (res[i] == '\n')	{
				linecount++;
				if (linecount == walk.startLine-1){
					for (size_t j = i; j < res.length(); j++){
						if (res[j] == '{' && walk.type == "function" && funFlag == false)
						{
							res.insert(j, "<div class = \"function\"> ");
							funFlag = true;
							break;
						}else if (res[j] == '{'  && walk.type == "class" && classFlag == false)
						{
							res.insert(j, "<div class = \"class\"> ");
							classFlag = true;
							break;
						}
					}
				}			

				if (linecount == walk.endLine-1 && walk.type == "function" && funFlag == true){
					for (size_t j = i; j < res.length(); j++){
						if (res[j] == '}' && res[j+1] == '\n' )
						{
							res.insert(j+1, "</div>");
							funFlag = false;
							break;
						}
					}
				}
				if (linecount == walk.endLine - 2 && walk.type == "class" && classFlag == true) {
					for (size_t j = i; j < res.length(); j++) {
						// consideration of this predicate is from that the enline of walk is after "};"
						if (res[j] == '}' && res[j + 1] == ';' && walk.type == "class")
						{
							res.insert(j + 2, "</div>");
							classFlag = false;
							break;
						}
					}
				}
			}

		}
	}
	return res;
}

// Insert button
inline std::string ContentConvert::insertButton(const std::string& content)
{
	std::string res = content;
	//std::string button;
	//button += "<button onclick=\"showHideClass('Comment')\">Show or Hide Comment Contents</button>";
	//button += "<button onclick=\"showHideClass('class')\">Show or Hide Class Contents</button>";
	//button += "<button onclick=\"showHideClass('function')\">Show or Hide Function Contents</button>";

	std::string checkbox = "<span class = \"absolute\">" ;
	checkbox += "<label><input type = \"checkbox\" id = \"hideF\" value = \"hidefuncs\" checked = \"true\" onclick = \"toggleVisibility('function')\">Functions</label><br>";
	checkbox += "<label><input type = \"checkbox\" id = \"hideC\" value = \"hideclass\" checked = \"true\" onclick = \"toggleVisibility('class')\">Classes</label><br>";
	checkbox += "<label><input type = \"checkbox\" id = \"hidec\" value = \"hidecomms\" checked = \"true\" onclick = \"toggleVisibility('Comment')\">Comments</label><br>";
	checkbox += "</span>";
		
		
		
	size_t pos = content.find("<h4>");
	res = res.insert(pos, checkbox);

	return res;

}

// For Pr2
inline void ContentConvert::fileToHtml1(const std::string& ffpath, std::vector<std::string> fileNames_)
{
	std::string content;
	std::vector<std::string> statements_;
	Convert_H converter;
	Dependency den;
	std::string fileName = FileSystem::Path::getName(ffpath) + ".html";
	content = converter.ReadFileString(ffpath);	

	content = handleClassandFunction(content, ffpath);
	content = handleComment(content);
	content = converter.InsterPreTag(content);
	//std::cout << "  Display package has applied to" + fileName + " successfully. \n";

	statements_ = den.getStatements(ffpath);
	statements_ = statementFilter(statements_, fileNames_);
	content = den.insertStatement(content, statements_);
	//std::cout << "  Dependencies package has applied to" + fileName + " successfully. \n";

	content = insertButton(content);
	converter.ConvertToHtml(ffpath, content);
	//std::cout << "------------------\n";
}

inline std::vector<std::string> ContentConvert::statementFilter(const std::vector<std::string> &statememts_, const std::vector<std::string>& fileNames_)
{
	std::vector<std::string> res;
	for (std::vector<std::string>::const_iterator nIterator = statememts_.begin(); nIterator != statememts_.end(); nIterator++)
	{
		if (std::find(fileNames_.begin(), fileNames_.end(), *nIterator) != fileNames_.end())
			res.push_back(*nIterator);
	}
	return res;
}