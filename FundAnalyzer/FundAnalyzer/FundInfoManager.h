#pragma once
#include "HttpCurl.h"
#include "ContentFilter.h"
#include <Python.h>

typedef struct FUND_INFO
{
	float fund_sourceRate;//ԭʼ����
	float fund_Rate;//�ַ���
	float syl_1n;//��һ������
	float syl_6y;
	float syl_3y;
	float syl_1y;
	std::string fS_code;
	std::string fS_name;
}FUND_INFO, *PFUND_INFO;

class CFundInfoManager:
	public CHttpCurl,
	public CContentFilter
{
public:
	CFundInfoManager();
	~CFundInfoManager();

	bool GetLatestSingleFundBaseInfo(const std::string& fund_id);
	bool GetLatestSingleFundBaseInfo(std::string& fund_id, std::string & base_info);
	bool GetFundName(std::string& fund_id) const;

private:
	bool GetFundNameWithPython(std::string& fund_id) const;
	bool ClearFundInfo();
	bool GetSingleFundBaseInfo(std::string& url, std::string& baseinfo);
	bool GetSingleFundBaseInfoWithPython(std::string& url, std::string& baseinfo);
	bool GetFormatLocalTime(std::string& sLocalTime);

private:
	PyObject* m_pModule;
	PyObject* m_pFuncTest;
	PyObject* m_pGetFundAllInfo;
	PyObject* m_pClear;
	PyObject* m_pGetFundName;
	PyObject* m_pGetFundSourceRate;//ԭ����
	PyObject* m_pGetFundRate;//�ַ���

	std::vector<FUND_INFO> m_vAllFundsInfo;
};

