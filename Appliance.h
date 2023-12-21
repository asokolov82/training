#pragma once 
#include <iostream>
#include "IElectronic.h"

class Appliance : virtual public IElectronic // объявление virtual для предотвраения ошибки ромбовидного наследования
{
protected:
	int _weight;
public:
	Appliance(int weight);
	void ShowSpec() override;
};
// обычное наследование
class Cooler final : public  Appliance
{
private:
	int _volume;
public:
	Cooler(int weight, int volume);
	void ShowSpec() override;
};

class Washing_machine final : public  Appliance
{
private:
	int _rpm;
public:
	Washing_machine(int weight, int rpm);
	void ShowSpec() override;
};