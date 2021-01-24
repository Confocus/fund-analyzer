#pragma once

#include <iostream>
#include <vector>
using namespace std;
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

class CContentFilter {
public:
	//该函数用来设置正则表达式匹配
	void SetReg(string reg);

	//该函数将匹配的结果连成一个字符串放在一起
	string filterUrlString(string url);

	//该函数将匹配的结果放到vector中
	vector<string> filterUrlVector(string url);

private:
	boost::regex _reg;
	boost::smatch _what;
};
