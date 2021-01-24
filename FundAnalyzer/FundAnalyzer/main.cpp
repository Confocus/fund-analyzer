#include <stdio.h>
#include "FundInfoManager.h"

std::wstring s2ws(const std::string& s)
{
	size_t i;
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const char* _source = s.c_str();
	size_t _dsize = s.size() + 1;
	wchar_t* _dest = new wchar_t[_dsize];
	wmemset(_dest, 0x0, _dsize);
	mbstowcs_s(&i, _dest, _dsize, _source, _dsize);
	std::wstring result = _dest;
	delete[] _dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

wstring string2wstring(string str)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	wchar_t* buffer = new wchar_t[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

class Basic
{
public:
	Basic()
	{
		m = 1;
	}
	~Basic()
	{
		printf("This is basic des..\n");
	}

	void print()
	{
		printf("This is basic..\n");
	}

	void printm()
	{
		printf("Basic:%d..\n", m);
	}

public:
	int m;
};


class Basic2
{
public:
	Basic2()
	{
		m = 3;
	}
	~Basic2()
	{
		printf("This is Basic2 des..\n");
	}

	void print()
	{
		printf("This is Basic2..\n");
	}

	void printm()
	{
		printf("Basic2:%d..\n", m);
	}

public:
	int m;
};

class Derived:public Basic
{
public:
	Derived()
	{
		m = 2;
	}
	~Derived()
	{
		printf("This is Derived des..\n");
	}

	void print()
	{
		printf("This is Derived..\n");
	}

	void printm()
	{
		printf("Derived:%d..\n", m);
	}

public:
	int m;
};

class VBasic
{
public:
	VBasic()
	{
		m = 1;
	}
	virtual ~VBasic()
	{
		printf("This is VBasic des..\n");
	}

	virtual void print()
	{
		printf("This is VBasic..\n");
	}

	void printm()
	{
		printf("VBasic:%d..\n", m);
	}

public:
	int m;
};

class VDerived :public VBasic
{
public:
	VDerived()
	{
		m = 2;
	}
	~VDerived()
	{
		printf("This is VDerived des..\n");
	}

	void print()
	{
		printf("This is VDerived..\n");
	}

	void printm()
	{
		printf("VDerived:%d..\n", m);
	}

public:
	int m;
};

int main()
{
	Basic* pb = new Derived();
	Basic* pb2 = reinterpret_cast<Basic*>(new Basic2());
	Derived* pd = new Derived();

	pb2->print();
	pb2->printm();

	pb->print();
	pb->printm();

	Derived td = *pd;
	pd->print();
	pd->printm();

	VBasic* pvb = new VDerived();
	pvb->print();
	pvb->printm();

	delete pvb;

	CFundInfoManager FuncInfo;
	FuncInfo.GetLatestSingleFundBaseInfo("003511");
	/*std::string context;
	std::string url = "http://fund.eastmoney.com/pingzhongdata/000001.js?v=20190304115823";
	std::string base_info;

	FuncInfo.SetReg("var fS_name = [\u4e00-\u9fa5_a-zA-Z0-9]+");
	FuncInfo.GetSingleFundBaseInfo(url, base_info);

	string test = "var fS_name = aaa;";
	std::string name = FuncInfo.filterUrlString(test);*/
	return true;
}