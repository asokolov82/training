#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream> 
#include <cstdio> 
#include <cstring> 
#include <string> 
#include <fstream>
#include <winsock2.h> 
#include <libpq-fe.h>
#pragma comment(lib, "WS2_32.lib")
using namespace std;
string str_buff, ustr, user, t1, t2, str, tmp, tmp_from, tmp_to;
int rows;
const char* conninfo = "dbname=chat user=postgres password=123 host=localhost port=5432 client_encoding='WIN1251'";
PGconn* conn = PQconnectdb(conninfo);
const char* query;
PGresult* res;

void request(string s) { //выполнение запроса в базу
	query = s.c_str();
	res = PQexec(conn, query);	
}

bool find_in_db(string s) { // поиск пользователя
	query = s.c_str();
	res = PQexec(conn, query);
	rows = PQntuples(res);
	//PQclear(res);
	if (rows != 0)
		return true;
	else return false;
}

string list_user() { // получение списка зарегистрированных пользователей
	tmp = "List of Users: ";
	str = "SELECT name FROM users" ;
	query = str.c_str();
	res = PQexec(conn, query);
	rows = PQntuples(res);
	for (int i = 0; i < rows; i++) {
		tmp += PQgetvalue(res, i, 0);
		tmp += "  ";
	}
	PQclear(res);
	tmp += "\n";
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
			request("DELETE FROM session WHERE id = " + to_string(client)); // удаляем из таблицы сессий отключившегося клиента
			break;
		}
		if (strcmp(buffer, "list\n") == 0) { // вывод списка пользователей
			strcpy_s(buffer, list_user().c_str());
			send(client, buffer, sizeof(buffer), 0);
			continue;
		}
		if (strcmp(buffer, "logout\n") == 0) { // выход из чата
			send(client, "-> logout ok", sizeof("-> logout ok"), 0);
			request("SELECT * FROM session WHERE id = " + to_string(client)); //получаем из таблицы session строку с именем пользователя в этой сессии
			tmp = PQgetvalue(res, 0, 1);
			strcpy_s(buffer, ("User \"" + tmp + "\" left the chat.\n").c_str()); //формируем сообщение о выходе пользователя для рассылки
			request("DELETE FROM session WHERE id = " + to_string(client)); //удаляем из таблицы session вышедшего пользователя
			request("SELECT id FROM session");
			rows = PQntuples(res);
			for (int i = 0; i < rows; i++) {
				send(stoi(PQgetvalue(res, i, 0)), buffer, sizeof(buffer), 0); // рассылка сообщения о выходе пользователя всем подключенным 
			}
			PQclear(res);
			continue;
		} 
		str_buff = buffer;
		if (str_buff.find("login") == 0) { // если приняли строку с login значит пройти процедуру авторизации
			//выделяем логин и пароль из полученой от клиента строки и проверяем есть ли такие в базе
			user = str_buff.substr(6, str_buff.find(":") - 6); // сохраняем логин для использования
			if (find_in_db("SELECT * FROM users WHERE name = '" + user + "' AND pwd = '" + str_buff.substr(str_buff.find(":") + 1, str_buff.length() - str_buff.find(":")) + "'")) {
				send(client, "-> login ok", sizeof("-> login ok"), 0);
				request("INSERT INTO session VALUES (" + to_string(client) + ",'" + user + "')"); // добавляем номер клиента  и его логин
				PQclear(res);
				request("SELECT * FROM message"); // далее читаем всю историю
				rows = PQntuples(res);
				for (int i = 0; i < rows; i++) {
					tmp = PQgetvalue(res, i, 0); 
					tmp += PQgetvalue(res, i, 1);
					strcpy_s(buffer, tmp.c_str());
					send(client, buffer, sizeof(buffer), 0);
				}
				PQclear(res);
				send(client, "-> end of history", sizeof("-> end of history"), 0);
				//проверяем наличие личных сообщений
				request("SELECT * FROM priv_msg WHERE to_user = '" + user + "'");
				rows = PQntuples(res);
				if (rows > 0) {
					Sleep(100);
					send(client, "----=Private message=----\n", sizeof("----=Private message=----\n"), 0);
					for (int i = 0; i < rows; i++) {
						tmp = PQgetvalue(res, i, 0);
						tmp += PQgetvalue(res, i, 2);
						strcpy_s(buffer, tmp.c_str());
						Sleep(100);
						send(client, buffer, sizeof(buffer), 0);
					}
					send(client, "----=END of Private message=----\n", sizeof("----=END of Private message=----\n"), 0);
				}
				request("DELETE FROM priv_msg WHERE to_user = '" + user + "'"); //удаляем все личные сообщения из базы, отправленные вошедшему пользователю
			}
			else {
				send(client, "-> login fail", sizeof("-> login fail"), 0);
			}
		}
		else if (str_buff.find("reg") == 0) { // если приняли строку с reg провести регистрацию
			if (!find_in_db("SELECT name FROM users WHERE name = '" + str_buff.substr(4, str_buff.find(":") - 4) + "'")) {
				request("INSERT INTO users VALUES ('" + str_buff.substr(4, str_buff.find(":") - 4) + "', '" + str_buff.substr(str_buff.find(":") + 1, str_buff.length() - str_buff.find(":")) + "')");
				PQclear(res);
				send(client, "-> register ok", sizeof("-> register ok"), 0);
			} else send(client, "-> user exist", sizeof("-> user exist"), 0);
		}
		else if (str_buff.find("^") != string::npos ) { // если признак личного сообщения в формате login: 18.2.2025 1.26-> login^ message
			tmp_to = str_buff.substr(str_buff.find(">") + 2, str_buff.find("^") - str_buff.find(">") - 2); // выделяем логин адресата
			tmp_from = str_buff.substr(0, str_buff.find(":")); // логин отправителя
			tmp = str_buff.substr(str_buff.find("^") + 1, str_buff.length() - str_buff.find("^")); // сообщение
			if (find_in_db("SELECT * FROM session WHERE login = '" + tmp_to + "'")) {//если пользователь сейчас залогинен отправляем ему сообщение
				strcpy_s(buffer, ("Private message from " + tmp_from + " : " + tmp).c_str());
				send(stoi(PQgetvalue(res, 0, 0)), buffer, sizeof(buffer), 0); // номер получателя  запроса выше в условии IF к таблице session
			} //если не залогинен то если вообще существует, просто пишем в базу для последующей отсылке при его входе
			else {
				if (find_in_db("SELECT * FROM users WHERE name = '" + tmp_to + "'")) {
					tmp_from += ": ";
					request("INSERT INTO priv_msg VALUES ('" + tmp_from + "', '" + tmp_to + "', '" + tmp + "')");
				}
				else {
					send(client, "Recipient not found\n", sizeof("Recipient not found\n"), 0);
				}
			}
		}
		else { // выводим сообщение в чат и пишем в базу
				cout << buffer;
				request("SELECT id FROM session");
				rows = PQntuples(res);
				for (int i = 0; i < rows; i++) {
					if (stoi(PQgetvalue(res, i, 0)) != client) send(stoi(PQgetvalue(res, i, 0)), buffer, sizeof(buffer), 0); // рассылка сообщения всем подключенным кроме отправителя
				}
				//из копии принятого буфера добываем имя пользователя и его сообщение для записи в таблицу
				request("INSERT INTO message VALUES ('" + str_buff.substr(0, str_buff.find(":") + 1) + "', '" + str_buff.substr(str_buff.find(":") + 1, str_buff.length() - str_buff.find(":")) + "')");
				PQclear(res);
		}
	}
	return 1;
}

int main() {
	if (PQstatus(conn) != CONNECTION_OK) {
		cout << ("Error while connecting to the database server: %s\n", PQerrorMessage(conn));
		PQfinish(conn);
		exit(1);
	}
	cout << ("Connection to DB established\n");
	request("DELETE FROM session"); // очищаем таблицу текущих сессий
	PQclear(res);
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