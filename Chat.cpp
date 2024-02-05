#include "Chat.h"
#include "Work_with_file.h"

auto main() -> int
{	
	string login, pass, nick, message;
	int select(0), flag(0);
	cin.exceptions(istream::failbit | istream::badbit);
	while (select != 3) { // глобальный цикл чата
		select = 0;
		cout << "press '1' for registration, '2' for login , '3' for exit" << endl;
		try {
		cin >> select;
		}
		catch (istream::failure e) { 
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Incorrect input!" << endl;
		}
		if (select == 1) { // регистрация нового пользователя
			cout << "Enter login, password and username: " << endl;
			cin >> login >> pass >> nick;
			try {
				User<>(login, pass, nick).add();
			}
			catch (string error_message)
			{
				cout << error_message << endl;
			}
		} 
		if (select == 2) { // процедура входа в чат, обработка сообщений пока
			cout << "Enter login, password: " << endl;
			cin >> login >> pass;
			try {
				nick = User<>(login, pass).login(); // логинимся и получаем ник пользователя если вход успешный
				if (!nick.empty()) {
					cout << "User with nickname '" << nick << "' successfully logged in" << endl << endl;
					cout << "		--== Common Chat ==--" << endl;
					Message<>("", "chat").read(); // вывод историю общего чата после успешного входа пользователя
					cout << "		--== Private Messages ==--" << endl;
					Message<>("", nick).read(); // вывод историю приватного чата после успешного входа пользователя
					cout << endl << "If You want send private message - type 'username:' before your message. for example - alex: hello" << endl
						<< "Type '^' for logout" << endl << "Type 'u' for list of nicknames" << endl << endl;
					getline(cin, message);
					while (true) { // цикл обработки сообщений пока не встретится символ ^ выхода пользователя из чата
						getline(cin, message);
						if (message.find("^") != string::npos) break; 
						if (message == "u") {
							Work_with_file<>().get_list_nick(); // выводим список имеющихся ников
						}
						else {
							flag = Priv_message<>(message).check_priv(); // узнаем является ли соообщение личным и есть ли пользователь кому оно адресовано
							if (flag == 2 || flag == 0)  // если пользователь существует или сообщение не личное, передаем сообщение в функцию, которая уже определит личное оно или общее
								Message<>(message, nick).add();
						}
					} 
				}
			} 
			catch (string error_message)
			{
				cout << error_message << endl;
			}
		}
		if (select == 3) break;
	}
return 0;
}

