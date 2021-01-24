#include "HttpCurl.h"

#pragma comment(lib, "libcurl.lib")
//�ο���GetTopicLinks

CHttpCurl::CHttpCurl():
	m_pConn(NULL)
{
}

CHttpCurl::~CHttpCurl() {
	//���øú�����վ��
	curl_easy_cleanup(m_pConn);
}

bool CHttpCurl::HttpCurlInit(std::string& context) {
	CURLcode code;

	//1�������߳��е���curl_global_init(CURL_GLOBAL_ALL)��ʼ��
	code = curl_global_init(CURL_GLOBAL_ALL);
	if (CURLE_OK != code) {
		cout << "Failed to global init default" << "\n";
		return false;
	}
	//2������curl_easy_init��ȡһ�������
	m_pConn = curl_easy_init();
	if (NULL == m_pConn) {
		cout << "Failed to create CURL" << "\n";
		return false;
	}

	//3������curl_easy_setopt�������ô˴δ����һЩ��������

	//CURLOPT_WRITEFUNCTION ���������������ĺ�������libcurl���յ����ݺ󱻵��ã���˺����������ݱ���Ĺ���
	//�����������ص�����ԭ��Ϊ��size_t function( void *ptr, size_t size, size_t nmemb, void *stream ); 
	code = curl_easy_setopt(m_pConn, CURLOPT_WRITEFUNCTION, CHttpCurl::Write);
	if (CURLE_OK != code) {
		cout << "Failed to set write" << "\n";
		return false;
	}
	// CURLOPT_WRITEDATA ���ڱ���CURLOPT_WRITEFUNCTION�����еĵ���������stream����Դ
	// ��˵�����ú����Ժ��յ������ݾͱ��ŵ���context��
	code = curl_easy_setopt(m_pConn, CURLOPT_WRITEDATA, &context);
	if (CURLE_OK != code) {
		cout << "Failed to set write data" << "\n";
		return false;
	}
	return true;
}

bool CHttpCurl::GetHttpResponse() {
	CURLcode code;
	// curl_easy_perform�������curl_easy_setoptָ��������ѡ��
	code = curl_easy_perform(m_pConn);
	if (CURLE_OK != code) {
		cout << "Failed to get response" << "\n";
		return false;
	}
	return true;
}

bool CHttpCurl::SetUrl(string& url) {
	CURLcode code;
	//CURLOPT_URL �������÷��ʵ�url
	code = curl_easy_setopt(m_pConn, CURLOPT_URL, url.c_str());
	if (CURLE_OK != code) {
		cout << "Failed to set URL" << "\n";
		return false;
	}
	return true;
}