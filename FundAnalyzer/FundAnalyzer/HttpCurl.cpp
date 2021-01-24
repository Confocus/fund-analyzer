#include "HttpCurl.h"

#pragma comment(lib, "libcurl.lib")
//参考：GetTopicLinks

CHttpCurl::CHttpCurl():
	m_pConn(NULL)
{
}

CHttpCurl::~CHttpCurl() {
	//调用该函数清空句柄
	curl_easy_cleanup(m_pConn);
}

bool CHttpCurl::HttpCurlInit(std::string& context) {
	CURLcode code;

	//1、在主线程中调用curl_global_init(CURL_GLOBAL_ALL)初始化
	code = curl_global_init(CURL_GLOBAL_ALL);
	if (CURLE_OK != code) {
		cout << "Failed to global init default" << "\n";
		return false;
	}
	//2、调用curl_easy_init获取一个句柄；
	m_pConn = curl_easy_init();
	if (NULL == m_pConn) {
		cout << "Failed to create CURL" << "\n";
		return false;
	}

	//3、调用curl_easy_setopt函数设置此次传输的一些基本参数

	//CURLOPT_WRITEFUNCTION 第三个参数表明的函数将在libcurl接收到数据后被调用，因此函数多做数据保存的功能
	//第三个参数回调函数原型为：size_t function( void *ptr, size_t size, size_t nmemb, void *stream ); 
	code = curl_easy_setopt(m_pConn, CURLOPT_WRITEFUNCTION, CHttpCurl::Write);
	if (CURLE_OK != code) {
		cout << "Failed to set write" << "\n";
		return false;
	}
	// CURLOPT_WRITEDATA 用于表明CURLOPT_WRITEFUNCTION函数中的第三个参数stream的来源
	// 因此调用完该函数以后，收到的数据就被放到了context中
	code = curl_easy_setopt(m_pConn, CURLOPT_WRITEDATA, &context);
	if (CURLE_OK != code) {
		cout << "Failed to set write data" << "\n";
		return false;
	}
	return true;
}

bool CHttpCurl::GetHttpResponse() {
	CURLcode code;
	// curl_easy_perform函数完成curl_easy_setopt指定的所有选项
	code = curl_easy_perform(m_pConn);
	if (CURLE_OK != code) {
		cout << "Failed to get response" << "\n";
		return false;
	}
	return true;
}

bool CHttpCurl::SetUrl(string& url) {
	CURLcode code;
	//CURLOPT_URL 用于设置访问的url
	code = curl_easy_setopt(m_pConn, CURLOPT_URL, url.c_str());
	if (CURLE_OK != code) {
		cout << "Failed to set URL" << "\n";
		return false;
	}
	return true;
}