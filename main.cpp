#include <iostream>
#include <string>
#include "Device.h"
#include "Appliance.h"
using namespace std;

int main()
{
	system("chcp 1251");
	IElectronic * lot[4]; 
	lot[0] = new Player(10, 500, 150, "белый");
	lot[1] = new Watch(75, 50, "черный", 42);
	lot[2] = new Cooler(60, 2);
	lot[3] = new Washing_machine(50, 900);
	
    bool open = true;
    while (open)
    {
            int choice;
            cout << "Выберите позицию : 1 - Плеер, 2 - Часы, 3 - Холодильник, 4 - Стиральная машина, 0 чтобы выйти" << endl;
            cin >> choice;
            switch (choice)
            {
            case 1:
                lot[0]->ShowSpec();
                break;
            case 2:
                lot[1]->ShowSpec();
                break;
            case 3:
                lot[2]->ShowSpec();
                break;
            case 4:
                lot[3]->ShowSpec();
                break;
            case 0:
                open = false;
                break;
            default:
                cout << "Выберите позицию от 1 до 4 или 0, чтобы выйти" << endl;
                break;
            }
     }
    delete lot[0], lot[1], lot[2], lot[3];
	return 0;
}