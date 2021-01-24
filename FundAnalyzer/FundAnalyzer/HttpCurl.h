#pragma once
#include "curl.h"
#include <string>
#include <iostream>

using namespace std;

class CHttpCurl
{
public:
	CHttpCurl();
		
	~CHttpCurl();

	bool HttpCurlInit(std::string& context);

	bool SetUrl(string& url);

	bool GetHttpResponse();

	static size_t Write(void* data, size_t size, size_t nmemb, string& context) {
		long sizes = size * nmemb;
		string temp((char*)data, sizes);
		context += temp;
		return sizes;
	}

private:
	CURL* m_pConn;
};

