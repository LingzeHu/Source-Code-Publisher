#pragma once
///////////////////////////////////////////////////////////////
// IExecutive.h - Translates managed calls into native calls //
//                                                           //
// Lingze Hu,   CSE687 - Object Oriented Design, Spring 2019 //
///////////////////////////////////////////////////////////////
/*
*  This package provides an interface for the Publish class
*  and a declaration of an object factory function.  Using these
*  isolates the user from all implementation details in Executive.h.
*
*  The easiest way to use this package is to build it as a static
*  library and the Translator project makes a reference to this
*  project.
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 13 Apr 2019
*  - first release
*/
#include <string>
#include <vector>

struct IPublisher
{
	virtual ~IPublisher() {}
	
	virtual std::string getName(const std::string &fileSpec) = 0;
	virtual std::vector<std::string> getFileList_(const std::string &path, std::vector<std::string> patterns, std::vector<std::string> regexes) = 0;
	virtual void publishFileList(std::vector<std::string> fileList) = 0;
	//virtual void publishAFile(const std::string &file) = 0;
	virtual std::string getFullFileSpec(const std::string &fileSpec) = 0;
};

IPublisher* createPublisherCpp();