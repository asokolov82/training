#include "function.h"
int main()
{  
	function chat;
	int select(0);
	
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
			chat.reg();
		}
		if (select == 2) { // процедура входа в чат и обработка сообщений 
			chat.log();			
		}
		if (select == 3) break;
	}
}

