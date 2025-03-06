#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> 
#include <libpq-fe.h>
#include "Logger.h"
#pragma comment(lib, "WS2_32.lib")
using namespace std;
string str_buff, ustr, user, t1, t2, str, tmp, tmp_from, tmp_to;
int rows;
const char* conninfo = "dbname=chat user=postgres password=123 host=localhost port=5432 client_encoding='WIN1251'";
PGconn* conn = PQconnectdb(conninfo);
const char* query;
PGresult* res;
const char* paramValues[3];

// функция вызываемая в потоке (для задания модуль 27)
 void  reader(Logger &l) {
	 cout << endl << "read log from thread -> " << l.read_log() << endl;
}

void request(string s) { //выполнение запроса в базу, старая версия
	query = s.c_str();
	res = PQexec(conn, query);	
}

void req(string qua, string param1, string param2, string param3, int n) { // запрос с помощью параметризированной функции
	paramValues[0] = param1.c_str();
	paramValues[1] = param2.c_str();
	paramValues[2] = param3.c_str();
	res = PQexecParams(conn, qua.c_str(), n, NULL, paramValues, NULL, NULL, 0);
}

bool find_in_db(string s) { // поиск пользователя, старая версия
	query = s.c_str();
	res = PQexec(conn, query);
	rows = PQntuples(res);
	if (rows != 0)
		return true;
	else return false;
}

bool find_in_base(string qua, string param1, string param2, int n) { // поиск пользователя
	paramValues[0] = param1.c_str();
	paramValues[1] = param2.c_str();
	res = PQexecParams(conn, qua.c_str(), n, NULL, paramValues, NULL, NULL, 0);
	rows = PQntuples(res);
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
	Logger logger;
	while (true) { //Цикл работы сервера
		if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			cout << "recv function failed with error " << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "exit\n") == 0) { //клиент отсоединился
			cout << "Client Disconnected." << endl;
		//	request("DELETE FROM session WHERE id = " + to_string(client)); // удаляем из таблицы сессий отключившегося клиента
			req("DELETE FROM session WHERE id = $1", to_string(client), "", "", 1);
			PQclear(res);
			break;
		}
		if (strcmp(buffer, "list\n") == 0) { // вывод списка пользователей
			strcpy_s(buffer, list_user().c_str());
			send(client, buffer, sizeof(buffer), 0);
			continue;
		}
		if (strcmp(buffer, "logout\n") == 0) { // выход из чата
			send(client, "-> logout ok", sizeof("-> logout ok"), 0);
		//	request("SELECT * FROM session WHERE id = " + to_string(client)); //получаем из таблицы session строку с именем пользователя в этой сессии
			req("SELECT * FROM session WHERE id = $1", to_string(client), "", "", 1);
			tmp = PQgetvalue(res, 0, 1);
			PQclear(res);
			strcpy_s(buffer, ("User \"" + tmp + "\" left the chat.\n").c_str()); //формируем сообщение о выходе пользователя для рассылки
		//	request("DELETE FROM session WHERE id = " + to_string(client)); //удаляем из таблицы session вышедшего пользователя
			req("DELETE FROM session WHERE id = $1", to_string(client), "", "", 1);
			PQclear(res);
		//	request("SELECT id FROM session");
			req("SELECT id FROM session", "", "", "", 0);
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
		//	if (find_in_db("SELECT * FROM users WHERE name = '" + user + "' AND pwd = '" + str_buff.substr(str_buff.find(":") + 1, str_buff.length() - str_buff.find(":")) + "'")) {
			if (find_in_base("SELECT * FROM users WHERE name = $1 AND pwd = $2", user, str_buff.substr(str_buff.find(":") + 1, str_buff.length() - str_buff.find(":")), 2)) {
				send(client, "-> login ok", sizeof("-> login ok"), 0);
			//	request("INSERT INTO session VALUES (" + to_string(client) + ",'" + user + "')"); // добавляем номер клиента  и его логин
				req("INSERT INTO session VALUES ($1,$2)", to_string(client), user, "", 2);
				PQclear(res);
			//	request("SELECT * FROM message"); // далее читаем всю историю
				req("SELECT * FROM message", "", "", "", 0);
				rows = PQntuples(res);
				for (int i = 0; i < rows; i++) {
					tmp = PQgetvalue(res, i, 0); 
					tmp += PQgetvalue(res, i, 1);
					strcpy_s(buffer, tmp.c_str());
					send(client, buffer, sizeof(buffer), 0);
				}
				PQclear(res);
				send(client, "-> end of history", sizeof("-> end of history"), 0);
			//	request("SELECT * FROM priv_msg WHERE to_user = '" + user + "'"); //проверяем наличие личных сообщений
				req("SELECT * FROM priv_msg WHERE to_user = $1", user, "", "", 1);
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
					PQclear(res);
					send(client, "----=END of Private message=----\n", sizeof("----=END of Private message=----\n"), 0);
				}
			//	request("DELETE FROM priv_msg WHERE to_user = '" + user + "'"); //удаляем все личные сообщения из базы, отправленные вошедшему пользователю
				req("DELETE FROM priv_msg WHERE to_user = $1", user, "", "", 1);
				PQclear(res);
			}
			else {
				send(client, "-> login fail", sizeof("-> login fail"), 0);
			}
		}
		else if (str_buff.find("reg") == 0) { // если приняли строку с reg провести регистрацию
		//	if (!find_in_db("SELECT name FROM users WHERE name = '" + str_buff.substr(4, str_buff.find(":") - 4) + "'")) {
			if (!find_in_base("SELECT name FROM users WHERE name = $1", str_buff.substr(4, str_buff.find(":") - 4), "", 1)) {
			//	request("INSERT INTO users VALUES ('" + str_buff.substr(4, str_buff.find(":") - 4) + "', '" + str_buff.substr(str_buff.find(":") + 1, str_buff.length() - str_buff.find(":")) + "')");
				req("INSERT INTO users VALUES ($1,$2)", str_buff.substr(4, str_buff.find(":") - 4), str_buff.substr(str_buff.find(":") + 1, str_buff.length() - str_buff.find(":")), "", 2);
				PQclear(res);
				send(client, "-> register ok", sizeof("-> register ok"), 0);
			} else send(client, "-> user exist", sizeof("-> user exist"), 0);
		}
		else if (str_buff.find("^") != string::npos ) { // если признак личного сообщения в формате login: 18.2.2025 1.26-> login^ message
			tmp_to = str_buff.substr(str_buff.find(">") + 2, str_buff.find("^") - str_buff.find(">") - 2); // выделяем логин адресата
			tmp_from = str_buff.substr(0, str_buff.find(":")); // логин отправителя
			tmp = str_buff.substr(str_buff.find("^") + 1, str_buff.length() - str_buff.find("^")); // сообщение
		//	if (find_in_db("SELECT * FROM session WHERE login = '" + tmp_to + "'")) {        //если пользователь сейчас залогинен отправляем ему сообщение
			if (find_in_base("SELECT * FROM session WHERE login = $1", tmp_to, "", 1)) {
				strcpy_s(buffer, ("Private message from " + tmp_from + " : " + tmp).c_str());
				send(stoi(PQgetvalue(res, 0, 0)), buffer, sizeof(buffer), 0); // номер получателя из запроса к таблице session в условии IF 
				PQclear(res);
			} 
			else {        //если не залогинен то если вообще существует, просто пишем в базу для последующей отсылке при его входе
			//	if (find_in_db("SELECT * FROM users WHERE name = '" + tmp_to + "'")) {
				if (find_in_base("SELECT * FROM users WHERE name = $1", tmp_to, "", 1)) {
					tmp_from += ": ";
				//	request("INSERT INTO priv_msg VALUES ('" + tmp_from + "', '" + tmp_to + "', '" + tmp + "')");
					req("INSERT INTO priv_msg VALUES ($1,$2,$3)", tmp_from, tmp_to, tmp, 3);
					PQclear(res);
				}
				else {
					send(client, "Recipient not found\n", sizeof("Recipient not found\n"), 0);
				}
			}
		}
		else { // выводим сообщение в чат и пишем в базу
		
		//	string test = "test string\n";							/*  ----------------БЛОК ДЛЯ ЗАДАНИЯ МОДУЛЯ 27 (потоки и класс Logger)-----------------  */
			// поток для записи сообщения в файл лога								
			std::thread wr(&Logger::write_log, ref(logger), ref(str_buff));
		//	std::thread wr1(&Logger::write_log, ref(logger), ref(test));
			// поток чтения из файла  строки и вывода ее на экран
			std::thread rd(reader, ref(logger));
		//	std::thread rd1(reader, ref(logger));
			wr.join();
		//	wr1.join();
			rd.join();
		//	rd1.join();
			
				cout << buffer;
		//		request("SELECT id FROM session");
				req("SELECT id FROM session", "", "", "", 0);
				rows = PQntuples(res);
				for (int i = 0; i < rows; i++) {
					if (stoi(PQgetvalue(res, i, 0)) != client) send(stoi(PQgetvalue(res, i, 0)), buffer, sizeof(buffer), 0); // рассылка сообщения всем подключенным кроме отправителя
				}
				PQclear(res);
				//из копии принятого буфера добываем имя пользователя и его сообщение для записи в таблицу
		//		request("INSERT INTO message VALUES ('" + str_buff.substr(0, str_buff.find(":") + 1) + "', '" + str_buff.substr(str_buff.find(":") + 1, str_buff.length() - str_buff.find(":")) + "')");
				req("INSERT INTO message VALUES ($1,$2)", str_buff.substr(0, str_buff.find(":") + 1), str_buff.substr(str_buff.find(":") + 1, str_buff.length() - str_buff.find(":")), "", 2);
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

	//req("INSERT INTO session VALUES ($1)", "12", "", 1);

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