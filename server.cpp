#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream> 
#include <cstdio> 
#include <cstring> 
#include <string> 
#include <fstream>
#include <winsock2.h> 
#pragma comment(lib, "WS2_32.lib")
using namespace std;
string str_buff, ustr, user, t1, t2;
int num(0), clients[10];
fstream user_file = fstream("users.txt", ios::in | ios::out | ios::app);
fstream mess_file = fstream("mess.txt", ios::in | ios::out | ios::app);

bool find_login_pass(string s) { // поиск сочетания логин\пароль
	user_file.clear();
	user_file.seekg(0, ios::beg);
	while (getline(user_file, ustr)) {
		if ((s.substr(6, s.find(":") - 6) == ustr.substr(0, ustr.find(":"))) && (s.substr(s.find(":"), sizeof(s) - s.find(":")) == ustr.substr(ustr.find(":"), sizeof(ustr) - ustr.find(":")))) {
			return true;
		}
	} return false;
}

bool find_login(string s) { // поиск пользователя
	user_file.seekg(0, ios::beg);
	while (getline(user_file, ustr)) {
		if ((s.substr(4, s.find(":") - 4) == ustr.substr(0, ustr.find(":")))) {
			return true;
		}
	} return false;
}

void add_user(string s) { // добавление пользователя при регистрации
	user_file.clear();
	user_file.seekp(0, ios::end);
	user_file << s.substr(4, sizeof(s) - 4) << endl;
}

string list_user() { // получение списка зарегистрированных пользователей
	string tmp("List of Users: ");
	user_file.seekg(0, ios::beg);
	while (getline(user_file, ustr)) {
		tmp += ustr.substr(0, ustr.find(":")) + "   ";
	}
		return tmp;
}

DWORD WINAPI Server(LPVOID lpParam) { 
	char buffer[1024] = { 0 }; //Буфер для данных
	SOCKET client = *(SOCKET*)lpParam; 
	while (true) { //Цикл работы сервера
		if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			cout << "recv function failed with error " << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "exit\n") == 0) { //клиент отсоединился
			cout << "Client Disconnected." << endl;
			clients[num] = 0; num--; // исключаем клиента
			break;
		}
		if (strcmp(buffer, "list\n") == 0) { // вывод списка пользователей
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, list_user().c_str());
			send(client, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
		}
		if (strcmp(buffer, "logout\n") == 0) { // выход из чата
			send(client, "-> logout ok", sizeof("-> logout ok"), 0);
		} 
		str_buff = buffer;
		if (str_buff.find("login") == 0) { // если приняли строку с login значит пройти процедуру авторизации 
			if (find_login_pass(str_buff)) {
				send(client, "-> login ok", sizeof("-> login ok"), 0);
				user = str_buff.substr(6, str_buff.find(":") - 6);
				memset(buffer, 0, sizeof(buffer));
				mess_file.clear();
				mess_file.seekg(0, ios::beg);
				while (getline(mess_file, ustr)) { // вывод истории чата
					strcpy_s(buffer, ustr.c_str());
					send(client, buffer, sizeof(buffer), 0);
					memset(buffer, 0, sizeof(buffer));
				}
				send(client, "-> end of history", sizeof("-> end of history"), 0);
				memset(buffer, 0, sizeof(buffer));
			}
			else {
				send(client, "-> login fail", sizeof("-> login fail"), 0);
			}
		}
		else if (str_buff.find("reg") == 0) { // если приняли строку с reg провести регистрацию
			if (!find_login(str_buff)) {
				add_user(str_buff);
				memset(buffer, 0, sizeof(buffer));
				send(client, "-> register ok", sizeof("-> register ok"), 0);
			} else send(client, "-> user exist", sizeof("-> user exist"), 0);
		}
		else {
			if (buffer[0] != 0 && buffer[0] != '\n' && strcmp(buffer, "logout\n") != 0) { // если буфер не пустой и не выход пользователя, сформировать строку сообщения для чата
				cout << user << ": " << buffer ;
				strcpy_s(buffer,(user + ": " + buffer).c_str());
			}
			if (strcmp(buffer, "logout\n") == 0) { // если пользователь выходит, сформировать оповещение
				strcpy_s(buffer, ("User \"" + user + "\" left the chat.").c_str());
			}
				for (int i = 0; i < num; i++) {
					if (clients[i] !=client && clients[i] != 0) send(clients[i], buffer, sizeof(buffer), 0); // рассылка сообщения всем подключенным кроме отправителя и отключившихся
				}
			mess_file.clear();
			mess_file.seekp(0, ios::end);
			mess_file << buffer ; // записать в файл истории чата
			memset(buffer, 0, sizeof(buffer));	
			
		}
	}
	return 1;
}

int main() {
	if (!user_file)
		user_file = fstream("users.txt", ios::in | ios::out | ios::trunc); // если файлов нет - создаем
	if (!mess_file)
		mess_file = fstream("mess.txt", ios::in | ios::out | ios::trunc);
	WSADATA WSAData;
	SOCKET server, client; 
	SOCKADDR_IN serverAddr, clientAddr; 
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0); 
	if (server == INVALID_SOCKET) {
		cout << "Socket creation failed with error:" << WSAGetLastError() << endl;
		return -1;
	}
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(55555);
	if (bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cout << "Bind function failed with error: " << WSAGetLastError() << endl;
		return -1;
	}

	if (listen(server, 0) == SOCKET_ERROR) { 
		cout << "Listen function failed with error:" << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Listening for incoming connections...." << endl;
	char buffer[1024]; 
	int clientAddrSize = sizeof(clientAddr); 
	while ((client = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
		cout << "Client connected!" << endl;
		clients[num] = client; num++; // сохраняем id клиента, увеличиваем число подключений
		DWORD tid; 
		HANDLE t1 = CreateThread(NULL, 0, Server, &client, 0, &tid); //поток сервера
		if (t1 == NULL) {
			cout << "Thread Creation Error: " << WSAGetLastError() << endl;
		}
	}
	closesocket(client); 
	if (closesocket(server) == SOCKET_ERROR) { 
		cout << "Close socket failed with error: " << WSAGetLastError() << endl;
		return -1;
	}
	WSACleanup();
	
}