#include "Logger.h"

using namespace std;

	Logger::Logger() {
		if (!log)
			log = fstream("log.txt", ios::in | ios::out | ios::trunc);
		log.open("log.txt"); // открываем файл в конструкторе (по заданию)
	}
	Logger::~Logger() {
		log.close(); // закрываем в деструкторе (по заданию)
	}
	void Logger::write_log(string& msg) { // метод класса для записи (по заданию)
		shared_mutex.lock();
		log.clear();
		log.seekp(0, ios::end);
		log << msg;
		//	for(int i=0; i<10; i++)       /* смотрим что никто не вмешивается в работу залоченого потока */
		//		std::cout << "Thread write ID: " << std::this_thread::get_id() << std::endl;
		//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
		shared_mutex.unlock();
	}
	string Logger::read_log() { // метод класса для чтения строки (по заданию)
		
		shared_mutex.lock_shared();
		log.clear();
		log.seekp(0, ios::beg);
		while (getline(log, str)) {
			tmp1 = tmp;
			tmp = str;
		}
		//	for (int i = 0; i < 10; i++)
		//		std::cout << "Thread read ID: " << std::this_thread::get_id() << std::endl;
		shared_mutex.unlock_shared();
		return tmp1;
	}

