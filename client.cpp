#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream> 
#include <algorithm>
#include <cstdio> 
#include <cstring> 
#include <winsock2.h> 
#pragma comment(lib, "WS2_32.lib")
using namespace std;
string login, pass, s, ip;
int sel(0), ind(0);
bool exit_chat = false;

DWORD WINAPI clientReceive(LPVOID lpParam) { //Получение данных от сервера
	char buffer[1024] = { 0 };
	SOCKET server = *(SOCKET*)lpParam;
	while (true) {
		if (recv(server, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			cout << "recv function failed with error: " << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "-> logout ok") == 0) { //завершаем поток если пользователь вышел
			break;
		}
		cout << buffer ;
		memset(buffer, 0, sizeof(buffer));
	}
	return 1;
}

DWORD WINAPI log_reg(LPVOID lpParam) { // процедуры входа или регистрации
	char buffer[1024] = { 0 };
	SOCKET server = *(SOCKET*)lpParam;
	while (ind == 0) {
		cout << "	Enter '1' to login or '2' to register new user" << endl;
		cin >> sel;
		cin.clear();
		cin.ignore(1024, '\n');
		if (sel == 1) { // вход
			cout << "	Enter login, password" << endl;
			cin >> login >> pass;
			s = "login " + login + ":" + pass; //строка с индикатором login, чтобы сервер понял что логинимся
			strcpy_s(buffer, s.c_str());
			send(server, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(server, buffer, sizeof(buffer), 0);
			cout << buffer << endl;
			if (strcmp(buffer, "-> login ok") == 0) { // если ответ сервера о успешной авторизации, выводим историю чата
				cout << "Type: 'list' - to display a list of users, 'logout' - to leave the chat  or  'exit' - to close chat" << endl << "-=Chat history=-" << endl;
				ind = 1;
				do {
					memset(buffer, 0, sizeof(buffer));
					recv(server, buffer, sizeof(buffer), 0);
					cout << buffer << endl;
				} while (strcmp(buffer, "-> end of history") != 0); // пока сервер не пришлет "конец"
			}
		}
		if (sel == 2) { // регистрация
			cout << "	Enter new login, password" << endl;
			cin >> login >> pass;
			s = "reg " + login + ":" + pass; //строка с индикатором reg, чтобы сервер понял что регистрируемся
			strcpy_s(buffer, s.c_str());
			send(server, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(server, buffer, sizeof(buffer), 0);
			cout << buffer << endl;
		}
	}
	return 1;
}
DWORD WINAPI clientSend(LPVOID lpParam) { //Отправка данных на сервер
	char buffer[1024] = { 0 };
	SOCKET server = *(SOCKET*)lpParam;
		while (true) {
			fgets(buffer, 1024, stdin);
			if (strcmp(buffer, "logout\n") == 0) { // пользователь вышел, конец потока
				send(server, buffer, sizeof(buffer), 0);
				ind = 0; 
				break;
			}
			if (strcmp(buffer, "list\n") == 0) { // запрос списка пользователей
				send(server, buffer, sizeof(buffer), 0);
				memset(buffer, 0, sizeof(buffer));
			}
			if (strcmp(buffer, "exit\n") == 0) { // пользователь закрывает программу, меняем флаг для полного выхода из цикла в main
				send(server, buffer, sizeof(buffer), 0);
				exit_chat = true;
				break;
			}
			send(server, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
		}
	return 1;
}

int main() {
	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN addr;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "Socket creation failed with error: " << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Enter Server IP (127.0.0.1 for localhost): " << endl; 
	cin >> ip;
	addr.sin_addr.s_addr = inet_addr(ip.c_str()); 
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(55555); 
	if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		cout << "Server connection failed with error: " << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Connected to server!" << endl;
	DWORD tid;
	do {
		HANDLE t3 = CreateThread(NULL, 0, log_reg, &server, 0, &tid); // для входа или регистрации
		WaitForSingleObject(t3, INFINITE); // ждем пока корректно не залогинится, тогда поток завершится
		CloseHandle(t3);
		HANDLE t1 = CreateThread(NULL, 0, clientReceive, &server, 0, &tid); // процесс общения в чате
		HANDLE t2 = CreateThread(NULL, 0, clientSend, &server, 0, &tid);// процесс общения в чате
		WaitForSingleObject(t2, INFINITE); // ждем пока пользователь не завершит поток отправки сообщений разлогинившись или закрывая программу
		CloseHandle(t2);
		CloseHandle(t1);
	} while (!exit_chat); // повторяем все с процедуры входа\регистрации пока пользователь не введет exit
	closesocket(server);
	WSACleanup();
	return 0;
}