#pragma once
#include "HttpCurl.h"
#include "ContentFilter.h"
#include <Python.h>

typedef struct FUND_INFO
{
	float fund_sourceRate;//原始费率
	float fund_Rate;//现费率
	float syl_1n;//近一年收益
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
	PyObject* m_pGetFundSourceRate;//原费率
	PyObject* m_pGetFundRate;//现费率

	std::vector<FUND_INFO> m_vAllFundsInfo;
};

