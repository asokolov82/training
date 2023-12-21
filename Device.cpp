#include "Device.h"

Device::Device(int batteryLife, string color) : _batteryLife(batteryLife), _color(color)
{
}

void Device::ShowSpec()
{
}

Player::Player(int batteryLife, int memory, int weight, string color) : Device(batteryLife, color), _memory(memory), Appliance(weight)
{
}

void Player::ShowSpec()
{
	cout << "Аудиоплеер  -  " << "Время работы: " << _batteryLife << "часов, Объем памяти: " << _memory
		<< "Гб, Вес: " << _weight << "грамм, Цвет: " << _color <<endl;
}

Watch::Watch(int batteryLife, int weight, string color, int size) : Device(batteryLife, color), Appliance(weight), _size(size)
{	
}

void Watch::ShowSpec()
{
	cout << "Часы  -  " << "Время работы: " << _batteryLife << "часов, Вес: " << _weight << "грамм, Размер: "
		<< _size << "мм, Цвет: " << _color << endl;
}
