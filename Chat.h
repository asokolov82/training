#pragma once
#include "Work_with_file.h"

using namespace std;

class Chat {
public:
	virtual void add() = 0;
	virtual void read() = 0;
	virtual ~Chat() = default;
};

template <typename T1 = string> class Comm_chat : virtual public Chat {
protected:
	T1 _message;
public:
	void read() {};
	void add() {};
	bool check_priv() {};
	Comm_chat(T1 message) : _message(message) {	}
};

template <typename T1 = string> class User : virtual public Chat {
protected:
	T1 _login, _pass, _nick;
public:
	User(T1 nick) : _nick(nick) {}
	User(T1 login, T1 pass) : _login(login), _pass(pass) {}
	User(T1 login, T1 pass, T1 nick) : _login(login), _pass(pass), _nick(nick) {}

	void add() { // добавление новго пользователя с проверкой на уникальность логина
		if (!Work_with_file<>("users.txt", _login, 0).search()) { // если не найден такой же логин - добавляем
			Work_with_file<>("users.txt", _login + ":" + _pass + ":" + _nick, 0).add();
			cout << "user '" << _nick << "' added" << endl;
		} else cout << "user already exists" << endl;
	}

	string login() { // процедура авторизации по логину\паролю
		if (Work_with_file<>("users.txt", _login, 0).search()) { // истина если логин найден
			if (Work_with_file<>("users.txt", _pass, 1).search()) { // истина если пароль найден
				//cout << "login successful" << endl;
				return Work_with_file<>(_login).get_nick(); // получаем ник успешно залогиненого пользователя
			}
			else { 
				cout << "wrong password" << endl; 
				return _nick = "";
			}
		}
		else cout << "login not found" << endl;
		return _nick = "";
	}

	void read() {};
};


template <typename T1 = string> class Message final : public Comm_chat<>, User<> {
public:

	Message(T1 message, T1 nick) : Comm_chat<>(message), User<>(nick) {}
	
	void add() { // добавление сообщений в файл
		if (_message.find(":") != string::npos) { // если найден признак приватного 
			Work_with_file<>(_message.substr(0, _message.find(":")) + ".txt", _nick + ":" + _message, 0).add(); // формируем имя файла для хранения личных сообщений, передаем имя написавшего и само сообщение
		} else
			Work_with_file<>("chat.txt", _nick + ":" + _message, 0).add(); // обычное сообщение в общий чат
	}

	void read() { // чтение всех сообщений из файла 
		Work_with_file<>(_nick + ".txt").read(); // формируем имя файла из котрого надо читать. личного или общего
	}
};

template <typename T1 = string> class Priv_message final : public Comm_chat<> {
private:
	int _flag = 0;
public:
	Priv_message(T1 message) : Comm_chat<>(message) {}
	
	int check_priv() { // проверка является ли сообщение личным
		if (_message.find(":") != string::npos) // если есть символ личного сообщения, проверяем существует ли такой пользователь
		{
			if (Work_with_file<>("users.txt", _message.substr(0, _message.find(":")), 2).search()) {
				return _flag = 2; // сообщение личное и адресат существует
			} else cout << "the specified user was not found" << endl; // сообщение личное но адресат НЕ существует
		}
		else
			return _flag = 0; // сообщение не личное
		return _flag = 1; // сообщение личное но адресат НЕ существует
	}
};