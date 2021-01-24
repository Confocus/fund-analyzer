#include "ContentFilter.h"


void CContentFilter::SetReg(string reg) {
	_reg = reg;
}

string CContentFilter::filterUrlString(string url) {
	string::const_iterator begin = url.begin();
	string::const_iterator end = url.end();

	string results;
	while (regex_search(begin, end, _what, _reg)) {

		string result(_what[0].first, _what[0].second);
		results += result;

		begin = _what[0].second;
	}

	return results;
}

vector<string> CContentFilter::filterUrlVector(string url) {
	string::const_iterator begin = url.begin();
	string::const_iterator end = url.end();

	vector<string> results;
	while (regex_search(begin, end, _what, _reg)) {

		string result(_what[0].first, _what[0].second);
		results.push_back(result);

		begin = _what[0].second;
	}

	return results;
}