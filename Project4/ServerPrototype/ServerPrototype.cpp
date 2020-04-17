/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.0                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //
/////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem/FileSystem.h"
#include "../Converter/Converter.h"
#include "../Display/Display.h"
//#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <chrono>

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

namespace MsgPassComm = MsgPassingCommunication;

//using namespace CodeAnalysis;
//using namespace Convert;
using namespace Repository_;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

void Server::passFile(const Repository_::SearchPath& path)
{
	std::string fullPath = Path::getFullFileSpec(path);;
	ContentConvert cc;
	std::vector<std::string> fileNameList;
	std::string fileName = Path::getName(fullPath);
	fileNameList.push_back(fileName);
	cc.fileToHtml1(fullPath, fileNameList);

	/*std::string fullPath = Path::getFullFileSpec(path);;
	Publisher pub;
	pub.publishFile(fullPath)*/;
}


Files Server::getFiles(const Repository_::SearchPath& path)
{
	std::vector<std::string> result;
	std::string fullpath = Path::getFullFileSpec(path);;
	std::vector<std::string> fileList = Directory::getFiles(path);
	for (auto file : fileList)
	{
		if (file.find(".cpp") != std::string::npos) {
			result.push_back(file);
		}
		if (file.find(".h") != std::string::npos) {
			result.push_back(file);
		}
	}
	return result;
}

Dirs Server::getDirs(const Repository_::SearchPath& path)
{
	return Directory::getDirectories(path);
}

template<typename T>
void show(const T& t, const std::string& msg)
{
	std::cout << "\n  " << msg.c_str();
	for (auto item : t)
	{
		std::cout << "\n    " << item.c_str();
	}
}

std::function<Msg(Msg)> echo = [](Msg msg) {
	Msg reply = msg;
	reply.to(msg.from());
	reply.from(msg.to());
	return reply;
};



std::function<Msg(Msg)> getFiles = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getFiles");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files files = Server::getFiles(searchPath);
		size_t count = 0;
		for (auto item : files)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, item);
		}
	}
	else
	{
		std::cout << "\n  getFiles message did not define a path attribute";
	}
	return reply;
};

std::function<Msg(Msg)> getDirs = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getDirs");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files dirs = Server::getDirs(searchPath);
		size_t count = 0;
		for (auto item : dirs)
		{
			if (item != ".." && item != ".")
			{
				std::string countStr = Utilities::Converter<size_t>::toString(++count);
				reply.attribute("dir" + countStr, item);
			}
		}
	}
	else
	{
		std::cout << "\n  getDirs message did not define a path attribute";
	}
	return reply;
};

std::function<Msg(Msg)> passFile = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("passFile");
	std::string path = msg.value("path");
	if (path != "")
	{
		Server::passFile(path);
		std::string fileName = Path::getName(path) + ".html";

		reply.attribute("file", fileName);
		reply.attribute("status", "Success" );
		std::cout << "\n=========================================================\n";
		std::cout << "Req 6 completed: converting source code in the server and transferring back to the local client. \n  ";
		std::cout << "Open \"Covert File\" to check. \n";
	}
	else
	{
		reply.attribute("status", "Fail");
		std::cout << "\n  getDirs message did not define a path attribute";
	}
	return reply;
};

void title()
{
	std::cout << "\nDemonstrating Project #4 - Remote Code Page Management \n  ";
	std::cout << "\n=======================================================\n";
}

void Req1()
{
	std::cout << "\n=======================================================\n";
	std::cout << "Req 4 completed: provide GUI for client and supoort displaying conversion result. \n  ";
	std::cout << "Req 5 completed: provide message designs for application. \n  ";
}

void Req2()
{
	std::cout << "=======================================================\n";
	std::cout << "Req 7 completed: demonstrate correct operations for two or more concurrent clients. \n  ";
	std::cout << "Req 8 completed. \n  ";
}

int main()
{
	title();
	Server server(serverEndPoint, "ServerPrototype");
	server.start();
	Files files = server.getFiles();
	Dirs dirs = server.getDirs();
	server.addMsgProc("echo", echo);
	server.addMsgProc("getFiles", getFiles);
	server.addMsgProc("getDirs", getDirs);
	server.addMsgProc("passFile", passFile);
	server.addMsgProc("serverQuit", echo);
	server.processMessages();
	Req1();
	server.stop();
	Req2();
	return 0;
}

