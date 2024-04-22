#include <iostream>
#include <conio.h>
#include <windows.h>

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

	~Tank()
	{
		std::cout << "Tank is over: 0x" << this << "\n";
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
public:
	double get_consumption() const
	{
		return CONSUMPTION;
	}
	double get_consumption_per_second() const
	{
		return consumption_per_second;
	}


	Engine(double consumption)
		:CONSUMPTION
		(
			consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
			consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
			consumption
		),
		consumption_per_second(CONSUMPTION * 3e-5), 
		is_started(false)
	{
		//this->consumption_per_second = CONSUMPTION * 3e-5;
		std::cout << "Engine is ready: 0x" << this << std::endl;
	}

	~Engine()
	{
		std::cout << "Engine is over: 0x" << this << std::endl;
	}

	void start() 
	{
		is_started = true;
	}

	void stop()
	{
		is_started = false;
	}

	bool started() const
	{
		return is_started;
	}

	void info() const
	{
		std::cout << "Consumpition: " << CONSUMPTION << " liters/100km" << std::endl;
		std::cout << "Consumpition perseconds: " << consumption_per_second << " liters/sec" << std::endl;
		std::cout << "Engine is " << (is_started ? "started" : "stopped") << std::endl;
	}

private:
	const double CONSUMPTION;
	double consumption_per_second;
	bool is_started;
};

class Car
{
public:
	Car(int consumption=10, int volume=60) 
		: engine(consumption),
		tank(volume),
		driver_inside(false)
	{
		std::cout << "Your car is ready to go.\n";
		this->tank.fill(1550);
	}

	~Car()
	{
		std::cout << "Your car is over.\n";
	}

	void get_in()
	{
		driver_inside = true;
		
	}

	void get_out()
	{
		driver_inside = false;
		std::cout << "Out of the car.\n";
	}

	void control()
	{
		uint8_t key;
		

		do {
			
			key = _getch_nolock();
			std::cout << key << "\n";

			switch (key) {
				case 13: 
					driver_inside ? get_out() : get_in();
					break;

				case 0x47:
					engine.started() ? engine.stop() : engine.start();
					break;
			}

			if (driver_inside)
				display();

			if (engine.started() && tank.get_fuel_level() > 0) {
				tank.give_fuel(engine.get_consumption_per_second());
			}
			else if (tank.get_fuel_level() <= 0) {
				engine.stop();

			}
			Sleep(1000);
			

		} while (key != 27);
		
	}

	void display() const 
	{
		system("cls");
		std::cout << "Fuel level: " << tank.get_fuel_level() << " litres.\n";
		std::cout << "Engine is " << (engine.started() ? "started" : "stopped") << std::endl;
		
	}

	void info() const
	{
		std::cout << "---- Engine info ----\n";
		engine.info();
		std::cout << "----- Tank info -----\n";
		tank.info();
		std::cout << "---------------------\n";
	}

private:
	Engine engine;
	Tank tank;
	bool driver_inside;

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

#ifdef ENGINE_CHECK

	Engine engine(10);
	engine.info();

#endif //ENGINE_CHECK


	Car bmw;
	//bmw.info();
	
	bmw.control();


	return 0;
}