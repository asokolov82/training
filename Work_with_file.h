#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <exception>

using namespace std;
template <typename T1 = string, typename T2 = int> class Work_with_file {
private:
	T1 _string1, _string2, _string3;
	T2 _flag;
	ofstream of;
	ifstream inf;
public:
	Work_with_file(T1 file_name, T1 text, T2 flag) : _string1(file_name), _string3(text), _flag(flag) {} // flag == 0 ищем login, 1 - password , 2 - nick.  конструктор для поиска и записи в файлы. в лучае записи _flag не важен, отдельный конструктор решил не делать.
	Work_with_file(T1 text) : _string3(text) {} // конструктор для get_nick и read
	Work_with_file() = default;
	~Work_with_file() { }

	bool search() { // поиск переданного текста в нужном файле (в нашем случае ищем только в users но может пригодится передача имени файла)
		inf.open(_string1);
		if (inf.is_open()) {
			switch (_flag)
			{
			case 0: // ищем логин
				while (getline(inf, _string2)) {
					if (_string2.substr(0, _string2.find(":")) == _string3) { inf.close(); return true; }
				}
			case 1: // ищем пароль
				while (getline(inf, _string2)) {
					if (_string2.substr(_string2.find(":") + 1, (_string2.rfind(":") - _string2.find(":") - 1)) == _string3) { inf.close(); return true; }
				}
			case 2: // ищем ник
				while (getline(inf, _string2)) {
					if (_string2.substr(_string2.rfind(":") + 1, (_string2.length() - _string2.rfind(":"))) == _string3) { inf.close(); return true; }
				}
			default:
				inf.close();
				return false;
			}
		} throw "error open file " + _string1; 
		return false;
	}

	string get_nick() { // возвращает ник по логину
		inf.open("users.txt");
		if (inf.is_open()) {
			while (getline(inf, _string2)) {
				if (_string2.substr(0, _string2.find(":")) == _string3) 	return (_string2.substr(_string2.rfind(":") + 1, (_string2.length() - _string2.rfind(":"))));
			}
		} throw " error open file 'users.txt' ";
		inf.close();
	}

	void get_list_nick() { // вывод всех имеющихся ников
		inf.open("users.txt");
		if (inf.is_open()) {
			while (getline(inf, _string2)) {
				cout << (_string2.substr(_string2.rfind(":") + 1, (_string2.length() - _string2.rfind(":")))) << endl;
			}
		}
		inf.close();
	}

	void add() { // добавляет переданный текст в переданный файл (тут уже и users и chat) 
		of.open(_string1, ios::app);
		if (of.is_open()) {
			of << _string3 << endl;
		}
		of.close();
	}

	void read() { // вывод историю чата из файла
		inf.open(_string3); 
		if (inf.is_open())
			while (getline(inf, _string1))
			{
				cout << _string1 << endl;
			}
		inf.close();
	}
};