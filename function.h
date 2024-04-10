#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <set>
using namespace std;
class function
{
public:
	void log();
	void reg();
	void msg();

private:
	map <string, string> users, priv;
	string login, pass, message, tmp_str;
	map <string, string>::iterator it;
	set <string> common;
};

