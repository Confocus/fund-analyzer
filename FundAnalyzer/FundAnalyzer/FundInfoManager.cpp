#include "FundInfoManager.h"
#define MAX_FUND_NAME_SIZE	0x50
//const std::string url_single_fund_base_info = 000001.js?v=20190304115823";
#define _CRT_SECURE_NO_WARNINGS

typedef struct TEST
{
	double d1;
	double d2;
}TEST;

CFundInfoManager::CFundInfoManager() :
	m_pFuncTest(NULL),
	m_pGetFundAllInfo(NULL),
	m_pClear(NULL),
	m_pGetFundName(NULL)
{
	//初始化Python环境  
	Py_Initialize();

	PyRun_SimpleString("import sys");
	//添加Insert模块路径  
	//PyRun_SimpleString(chdir_cmd.c_str());
	PyRun_SimpleString("sys.path.append('D:\\Coding\\FundAnalyzer\\FundAnalyzer\\python')");

	//D:\Coding\FundAnalyzer\FundAnalyzer\python\fundanalyzer.py
	//导入模块  
	m_pModule = PyImport_ImportModule("fundanalyzer");

	if (!m_pModule)
	{
		cout << "Python get module failed." << endl;
		return;
	}

	cout << "Python get module succeed." << endl;
	m_pFuncTest = PyObject_GetAttrString(m_pModule, "getTest");
	PyObject_CallObject(m_pFuncTest, NULL);

	m_pGetFundAllInfo = PyObject_GetAttrString(m_pModule, "getFundAllInfo");

	m_pClear = PyObject_GetAttrString(m_pModule, "clear");

	m_pGetFundName = PyObject_GetAttrString(m_pModule, "getFundName");

	m_pGetFundSourceRate = PyObject_GetAttrString(m_pModule, "getFundSourceRate");

	m_pGetFundRate = PyObject_GetAttrString(m_pModule, "getFundRate");
	//PyObject* pv = PyObject_GetAttrString(pModule, "_add");
	//if (!pv || !PyCallable_Check(pv))
	//{
	//	cout << "Can't find funftion (_add)" << endl;
	//	return;
	//}
	//cout << "Get function (_add) succeed." << endl;

	////初始化要传入的参数，args配置成传入两个参数的模式  
	//PyObject* args = PyTuple_New(2);
	////将Long型数据转换成Python可接收的类型  
	//PyObject* arg1 = PyLong_FromLong(4);
	//PyObject* arg2 = PyLong_FromLong(3);

	////将arg1配置为arg带入的第一个参数  
	//PyTuple_SetItem(args, 0, arg1);
	////将arg1配置为arg带入的第二个参数  
	//PyTuple_SetItem(args, 1, arg2);

	////传入参数调用函数，并获取返回值  
	//PyObject* pRet = PyObject_CallObject(pv, args);

	//if (pRet)
	//{
	//	//将返回值转换成long型  
	//	long result = PyLong_AsLong(pRet);
	//	cout << "result:" << result << endl;
	//}
}

CFundInfoManager::~CFundInfoManager()
{
	Py_DECREF(m_pModule);
	Py_Finalize();
}

std::wstring ANSIToUnicode(const std::string& str)
{
	std::wstring ret;
	std::mbstate_t state = {};
	const char* src = str.data();
	size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
	if (static_cast<size_t>(-1) != len) {
		std::unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
		len = std::mbsrtowcs(buff.get(), &src, len, &state);
		if (static_cast<size_t>(-1) != len) {
			ret.assign(buff.get(), len);
		}
	}
	return ret;
}

string GbkToUtf8(const char* src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

string Utf8ToGbk(const char* src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

bool CFundInfoManager::GetLatestSingleFundBaseInfo(const std::string& fund_id)
{
	bool bRet = false;
	PyObject* args = PyTuple_New(1);
	PyObject* py_fund_id = PyUnicode_FromString(fund_id.c_str());
	PyTuple_SetItem(args, 0, py_fund_id);
	PyObject* pRet = PyObject_CallObject(m_pGetFundAllInfo, args);

	setlocale(LC_ALL, "");
	FUND_INFO fi;
	char* pFundName = nullptr;
	
	PyArg_ParseTuple(pRet, "sffffff", &pFundName, &fi.fund_sourceRate, &fi.fund_Rate,
		&fi.syl_1n, &fi.syl_6y, &fi.syl_3y, &fi.syl_1y);

	
	//python传回来的是utf-8，而
	//const char* test = "长盛可转债债券C";中，test保存的是gbk，所以printf时可以正常输出
	//所以我们从python中拿到utf-8格式之后也要转一次gbk
	//utf编码和gbk编码的底层二进制代码是不一样的。printf("%s")只具备把gbk准确解释输出的能力

	if (!pFundName)
	{
		goto FALSE_RETURN;
	}
	fi.fS_name = Utf8ToGbk(pFundName);
	fi.fS_code = fund_id;

	m_vAllFundsInfo.push_back(fi);

	//printf("test：%s\n", test);

	printf("基金名称：%s\n", fi.fS_name.c_str());
	printf("原费率：%f\n", fi.fund_sourceRate);
	printf("现费率：%f\n", fi.fund_Rate);
	printf("近1年收益率：%f\n", fi.syl_1n);
	printf("近6月收益率：%f\n", fi.syl_6y);
	printf("近3月收益率：%f\n", fi.syl_3y);
	printf("近1月收益率：%f\n", fi.syl_1y);

	//printf("%f     %f\n", pstRsp->d1, pstRsp->d2);
	/*PyObject* pRetFundName = PyObject_CallObject(m_pGetFundName, NULL);
	PyUnicode_AsWideChar(pRetFundName, wcsFundName, MAX_FUND_NAME_SIZE);

	PyObject* pRetSrcRate = PyObject_CallObject(m_pGetFundSourceRate, NULL);
	double dFundSourceRate = PyFloat_AsDouble(pRetSrcRate);

	PyObject* pRetRate = PyObject_CallObject(m_pGetFundRate, NULL);
	double dFundRate = PyFloat_AsDouble(pRetRate);*/
FALSE_RETURN:
	return bRet;
}

bool CFundInfoManager::GetLatestSingleFundBaseInfo(std::string& fund_id, std::string& base_info)
{

	std::string sLoacalTime;
	GetFormatLocalTime(sLoacalTime);

	std::string url_single_fund_base_info = "http://fund.eastmoney.com/pingzhongdata/";
	url_single_fund_base_info += fund_id;
	url_single_fund_base_info += ".js ? v =";
	url_single_fund_base_info += sLoacalTime;

	GetSingleFundBaseInfo(url_single_fund_base_info, base_info);

	return true;
}


bool CFundInfoManager::GetFundName(std::string& fund_id) const
{
	return true;
}

bool CFundInfoManager::GetFundNameWithPython(std::string& fund_id) const
{
	return true;
}

bool CFundInfoManager::ClearFundInfo()
{
	if (!m_pClear)
	{
		return false;
	}
	PyObject_CallObject(m_pClear, NULL);

	return true;
}

bool CFundInfoManager::GetSingleFundBaseInfo(std::string& url, std::string& baseinfo)
{
	HttpCurlInit(baseinfo);
	SetUrl(url);
	//调用完GetHttpResponse函数以后，响应的body部分就在context中了
	GetHttpResponse();
	return true;
}

bool CFundInfoManager::GetSingleFundBaseInfoWithPython(std::string& url, std::string& baseinfo)
{
	return true;
}

bool CFundInfoManager::GetFormatLocalTime(std::string& sLocalTime)
{
	std::string sTime;
	char szTime[0x20] = { 0 };
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	sprintf_s(szTime, 0x20, "%d%02d%02d%02d%02d%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	sLocalTime = szTime;

	return true;
}
