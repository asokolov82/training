#pragma once 
#include <iostream>
#include "IElectronic.h"
#include "Appliance.h"

class Device : virtual public IElectronic // объявление virtual для предотвраения ошибки ромбовидного наследования
{
protected:
	int _batteryLife;
	string  _color;
public:
	Device(int batteryLife, string color);
	virtual void ShowSpec() override; 
};
// плеер и часы наследуются от 2 классов
class Player final : public Device, Appliance
{
private:
	int _memory;
public:
	Player(int batteryLife, int memory, int weight, string color);
	void ShowSpec() override;
};

class Watch final : public Device, Appliance
{
private:
	int _size;
public:
	Watch(int batteryLife, int weight, string color, int size);
	void ShowSpec() override;

};