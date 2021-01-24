#pragma once

#include <iostream>
#include <vector>
using namespace std;
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

class CContentFilter {
public:
	//�ú�����������������ʽƥ��
	void SetReg(string reg);

	//�ú�����ƥ��Ľ������һ���ַ�������һ��
	string filterUrlString(string url);

	//�ú�����ƥ��Ľ���ŵ�vector��
	vector<string> filterUrlVector(string url);

private:
	boost::regex _reg;
	boost::smatch _what;
};
