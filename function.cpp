#include "function.h"

void function::msg()
{
	while (true) { // цикл обработки сообщений пока не встретится символ ^ выхода пользователя из чата
		getline(cin, message);
		if (message.find("^") != string::npos) break; // выход пользователя
		if (message == "u") { // вывод списка зареганых пользователей чата
			for (it = users.begin(); it != users.end(); it++) cout << "Nickname: " << it->first << "    ";
			cout << endl;
		}else if (message.find(":") != string::npos) { // определение является ли личным
			tmp_str = message.substr(0, message.find(":"));
			it = users.find(tmp_str);
			if (it != users.end()) { // существует ли адресат
				priv[tmp_str] += "from->" + login + " to->" + message + '\n'; 
			}
			else {
				cout << "no recipient" << endl;
			}
		}
		else {
			common.insert(login + ": " + message);
		}
	}
}



void function::log()
{
	cout << "Enter login, password: " << endl;
	cin >> login >> pass;
	it = users.find(login);
	if (it != users.end()) {//есть ли такой пользователь
		it = users.find(pass);
		if (it != users.end()) { // совпадает ли пароль
			cout << endl << "login successful" << endl << "If You want send private message - type 'username:' before your message. for example - alex: hello" << endl
				<< "Type '^' for logout" << endl << "Type 'u' for list of nicknames" << endl << endl;
			if (!common.empty()) { // выводим общий чат если есть
				cout << "		--== Common Chat ==--" << endl;
				for (string s : common) cout << s << endl;
				if (!priv.empty()) { 
					it = priv.find(login);
					if (it != priv.end()) { // выводим личные если есть для залогиневшегося
						cout << "		--== Private Messages ==--" << endl << it->second << endl;
					}
				}
			}
			msg();
		}
		else {
			cout << "wrong pass" << endl;
		}
	}
	else {
		cout << " no such user" << endl;
	}
}

void function::reg()
{
	cout << "Input login, pass" << endl;
	cin >> login >> pass;
	it = users.find(login);
	if (it == users.end()) {
		users.emplace(make_pair(login, pass));
		cout << "User '" << login << "' successfully registered" << endl;
	}
	else {
		cout << "User already exists" << endl;
	}
}

