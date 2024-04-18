#include <iostream>

#define MIN_TANK_VOLUME				20
#define MAX_TANK_VOLUME				120

#define MIN_ENGINE_CONSUMPTION		3
#define MAX_ENGINE_CONSUMPTION		30

//#define TANK_CHECK
#define ENGINE


class Tank 
{
public:
	Tank(int volume) 
		: VOLUME
		(
			volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
			volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
			volume
		)
	{

		this->fuel_level = 0;
		std::cout << "Tank is ready. 0x"<< this << "\n";
	}

	const int get_VOLUME() const
	{
		return VOLUME;
	}

	double get_fuel_level() const 
	{
		return fuel_level;
	}

	void fill(double amount)
	{
		if (amount < 0) {
			return;
		}

		if ((fuel_level + amount) < VOLUME) {
			fuel_level += amount;
		} else {
			fuel_level = VOLUME;
		}
	}

	double give_fuel(double amount)
	{
		if (amount < 0) {
			return fuel_level;
		}

		fuel_level -= amount;
		if (fuel_level < 0) {
			fuel_level = 0;
		}

		return fuel_level;
	}

	void info() const
	{
		std::cout << "Tank: " << VOLUME << " liters.\n";
		std::cout << "Fuel level: " << fuel_level << " liters.\n";
	}

private:
	const int VOLUME;
	double fuel_level;
};


class Engine
{
	const double CONSUMPTION;
	double consumption_per_second;
public:
	double get_consumption()const
	{
		return CONSUMPTION;
	}
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}


	Engine(double consumption)
		:CONSUMPTION
		(
			consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
			consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
			consumption
		)
	{
		this->consumption_per_second = CONSUMPTION * 3e-5;
		std::cout << "Engine is ready: 0x" << this << std::endl;
	}

	~Engine()
	{
		std::cout << "Engine is over: 0x" << this << std::endl;
	}

	void info()const
	{
		std::cout << "Consumpition: " << CONSUMPTION << " liters/100km" << std::endl;
		std::cout << "Consumpition perseconds: " << consumption_per_second << " liters/100km" << std::endl;
	}
};

int main()
{
	setlocale(0, "Rus");

#ifdef TANK_CHECK
	Tank tank(50);
	
	int fuel;
	do {
		std::cout << "Сколько: "; std::cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (fuel > 0);

#endif // TANK_CHECK

	Engine engine(10);
	engine.info();

	return 0;
}