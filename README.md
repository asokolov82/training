Соколов Алексей, Владислав корчак 
********************************************
Идея решения:

отправку личных сообщений решено сделать указанием имени адресата перед сообщением в виде "имя:" , если 
обнаруживается что сообщение личное, оно сохраняется в отдельный файл и выводится когда пользователь
 с именем адресата входит в чат. обычные сообщения сохраняются в файл общего чата и выводятся при входе 
 любого пользователя.

чтобы хранить историю сообщений общего чата и зарегистрированных пользователей используются текстовые файлы chat и users, 
для личных сообщений создается новый файл "имя.txt" на каждого пользователя.

********************************************
Описание пользовательских типов и функций:

Абстрактный класс Chat, от него наследуется Comm_chat  с аргументом _message и User с аргументами _login, _pass, _nick
класс Message наследник Comm_chat и User
класс Priv_message наследник Comm_chat

User: имеет функции add для добавления пользователей при регистрации, login для прохождения процедуры проверки логина пароля при входе в чат
Message: функция add для добавления сообщений и read для чтения сообщений
Priv_message: функция check_priv для проверки является ли сообщение приватным.

отдельный класс Work_with_file для работы с файлами, имеет функции: search для поиска в файлах,
get_nick для получения ника по логину, add для записи в файлы, read для чтения из файлов.

***********************************************
Проверка имеющегося набора:
логинимся под user1 pass имеем ник Alex.  видим сообщение в личку и общем чате, можем написать личное Stas: msg to stas
или в общий чат msg to all
разлогиниваемся, заходим под user2 pass имеем ник Stas и видим личку от Alex

***********************************************
Распределение задач:
совместное обсуждение вариантов решения, обмен идеями по реализации, однако каждый решил сделать свою версию полного кода,
так как мне больше импонировала идея с файлами, а Владислав решил делать на векторах.