﻿#include <iostream>
using namespace std;

int main()
{ 
	system("chcp 1251");
	float number1(0), number2(0); // для возможности выпоолнять операции с дробными числами.
	cout << "Введите последовательно два числа, ввод каждого подтверждая кнопкой Enter" << endl << "Допускаются дробные числа с точкой. Битовые операции будут выполнены только над целым числом." << endl;
	cin >> number1 >> number2;
	cout << "Результаты математических и логических операций:" << endl;
	cout << number1 << "+" << number2 << "=" << number1 + number2 << endl << number1 << "-" << number2 << "=" << number1 - number2 << endl;
	cout << number1 << "*" << number2 << "=" << number1 * number2 << endl << number1 << "/" << number2 << "=" << number1 / number2 << endl;
	int num1_bit = number1, num2_bit = number2; // для возможности выполнять битовые операции.
	cout <<  num1_bit << "&" << num2_bit << "=" << (num1_bit & num2_bit) << endl << num1_bit << "|" << num2_bit << "=" << (num1_bit | num2_bit) << endl;
	cout << num1_bit << "^" << num2_bit << "=" << (num1_bit ^ num2_bit) << endl;
		
	return 0;
}
