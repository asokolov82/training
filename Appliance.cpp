#include "Appliance.h"
#include "Device.h"

Appliance::Appliance(int weight) : _weight(weight)
{
}

void Appliance::ShowSpec()
{
}

Cooler::Cooler(int weight, int volume) : Appliance(weight), _volume(volume)
{
}

void Cooler::ShowSpec()
{
	cout << "Холодильник  -  " << "Объем: " << _volume << "кубометра, Вес: " << _weight << "килограмм" << endl;
}

Washing_machine::Washing_machine(int weight, int rpm) : Appliance(weight), _rpm(rpm)
{
}

void Washing_machine::ShowSpec()
{
	cout << "Стиральная машина  -  " << "Скорость отжима: " << _rpm << "оборотов, Вес: " << _weight << "килограмм" << endl;
}

