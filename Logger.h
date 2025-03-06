#pragma once
#include <iostream> 
#include <cstdio> 
#include <cstring> 
#include <string> 
#include <fstream>
#include <thread>
#include <shared_mutex>
#include <chrono>
using namespace std;
class Logger
{
public:
	Logger();
	~Logger();
	void write_log(string& msg);
	string read_log();
	
private:
	fstream log = fstream("log.txt", ios::in | ios::out | ios::app);
	std::shared_mutex shared_mutex;
	string tmp, tmp1, str;
};

