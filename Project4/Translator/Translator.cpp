/////////////////////////////////////////////////////////////////
// Translator.cpp - Translates managed calls into native calls //
//                                                             //
// Lingze Hu,   CSE687 - Object Oriented Design, Spring 2019   //
/////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ITranslator.h"
#include "../Executive/IExecutive.h"

#include <string>
#include "../MsgPassingComm/IComm.h"
#include "CsMessage.h"
#include "../CommLibWrapper/CommLibWrapper.h"

using namespace System;
using namespace System::Text;

namespace MsgPassingCommunication
{
	public ref class Translater
	{
	public:
		Translater();
		void listen(CsEndPoint^ ep);
		void postMessage(CsMessage^ msg);
		CsMessage^ getMessage();
		CsMessage^ fromMessage(Message& msg);
		Message fromCsMessage(CsMessage^ csMsg);
	private:
		CsEndPoint^ ep_;
		MsgPassingCommunication::CommFactory* pFactory;
		MsgPassingCommunication::IComm* pComm;
	};
	//----< initialize endpoint >----------------------------------------

	Translater::Translater()
	{
		ep_ = gcnew CsEndPoint;
	}
	//----< set client listen endpoint and start Comm >------------------

	void Translater::listen(CsEndPoint^ ep)
	{
		std::cout << "\n  using CommFactory to create instance of Comm on native heap";
		ep_->machineAddress = ep->machineAddress;
		ep_->port = ep->port;
		pFactory = new CommFactory;
		pComm = pFactory->create(Sutils::MtoNstr(ep_->machineAddress), ep_->port);
		pComm->start();
		delete pFactory;
	}
	//----< convert native message to managed message >------------------

	CsMessage^ Translater::fromMessage(Message& msg)
	{
		CsMessage^ csMsg = gcnew CsMessage;
		Message::Attributes attribs = msg.attributes();
		for (auto attrib : attribs)
		{
			csMsg->add(Sutils::NtoMstr(attrib.first), Sutils::NtoMstr(attrib.second));
		}
		return csMsg;
	}
	//----< convert managed message to native message >------------------

	Message Translater::fromCsMessage(CsMessage^ csMsg)
	{
		Message msg;
		auto enumer = csMsg->attributes->GetEnumerator();
		while (enumer.MoveNext())
		{
			String^ key = enumer.Current.Key;
			String^ value = enumer.Current.Value;
			msg.attribute(Sutils::MtoNstr(key), Sutils::MtoNstr(value));
		}
		return msg;
	}
	//----< use Comm to post message >-----------------------------------

	inline void Translater::postMessage(CsMessage^ csMsg)
	{
		std::cout << "\n  posting message in Translater";
		Message msg = this->fromCsMessage(csMsg);
		pComm->postMessage(msg);
	}
	//----< get message from Comm >--------------------------------------

	inline CsMessage^ Translater::getMessage()
	{
		std::cout << "\n  getting message in Translater";
		Message msg = pComm->getMessage();
		return fromMessage(msg);
	}
}

public ref class Translator : ITranslator
{
public:
	Translator()
	{
		pPublisherCpp_ = createPublisherCpp();
	}
	~Translator()
	{
		delete pPublisherCpp_;
		pPublisherCpp_ = nullptr;
	}
	virtual String^ getName(String^ fileSpec) override
	{
		return nToM(pPublisherCpp_->getName(mToN(fileSpec)));
	}
	virtual String^ getFullFileSpec(String^ fileSpec) override
	{
		return nToM(pPublisherCpp_->getFullFileSpec(mToN(fileSpec)));
	}
	virtual List<String^>^ getFileList_(String^ path, List<String^>^ patterns, List<String^>^ regexes) override
	{
		List<String^>^ res = nToM(pPublisherCpp_->getFileList_(mToN(path), mToN(patterns), mToN(regexes)));
		return res;
	}
	virtual void publishFileList(List<String^>^ fileList) override
	{
		pPublisherCpp_->publishFileList(mToN(fileList));
	}

	//virtual void publishAFile(String^ file) override
	//{
	//	pPublisherCpp_->publishAFile(mToN(file));
	//}


private:
	std::string mToN(String^ str);
	String^ nToM(const std::string& str);
	std::vector<double> mToN(List<double>^ listDbls);
	List<double>^ nToM(const std::vector<double>& vecDbls);
	std::vector<std::string> mToN(List<String^>^ listDbls);
	List<String^>^ nToM(const std::vector<std::string>& vecDbls);	

	IPublisher* pPublisherCpp_ = nullptr;
};

ITranslator^ TranslatorFactory::createTranslator()
{
	return gcnew Translator();
}

std::string Translator::mToN(String^ str)
{
	std::string temp;
	for (int i = 0; i < str->Length; ++i)
		temp += char(str[i]);
	return temp;
}

String^ Translator::nToM(const std::string& str)
{
	StringBuilder^ sb = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		sb->Append((wchar_t)str[i]);
	return sb->ToString();
}

std::vector<double> Translator::mToN(List<double>^ dbls)
{
	std::vector<double> temp;
	for (int i = 0; i < dbls->Count; ++i)
		temp.push_back(dbls[i]);
	return temp;
}

std::vector<std::string> Translator::mToN(List<String^>^ dbls)
{
	std::vector<std::string> temp;
	for (int i = 0; i < dbls->Count; ++i)
	{
		std::string str;
		String^ tempStr = dbls[i];
		for (int j = 0; j < tempStr->Length; ++j)
		{
			str += char(tempStr[j]);
		}
		temp.push_back(str);
		str = "";
	}		
	return temp;
}

List<double>^ Translator::nToM(const std::vector<double>& dbls)
{
	List<double>^ temp = gcnew List<double>();
	for (size_t i = 0; i < dbls.size(); ++i)
		temp->Add(dbls[i]);
	return temp;
}


List<String^>^ Translator::nToM(const std::vector<std::string>& dbls)
{
	List<String^>^ temp = gcnew List<String^>();
	for (size_t i = 0; i < dbls.size(); ++i)
	{
		String^ str = gcnew String(dbls[i].c_str());
		temp->Add(str);
	}		
	return temp;
}

