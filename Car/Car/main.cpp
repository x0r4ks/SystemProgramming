#include <iostream>
#include <conio.h>
#include <thread>

#include <Windows.h>

using namespace std::chrono_literals;


#define MIN_TANK_VOLUME					20
#define MAX_TANK_VOLUME					120

#define MIN_ENGINE_CONSUMPTION			3
#define MAX_ENGINE_CONSUMPTION			30


//#define TANK_CHECK
#define ENGINE


enum KEYS {
	ENTER = 13,
	ESCAPE = 27,
	
	g = 103,
	G = 71,

	F=70,
	f=102,

	W=87,
	w=119,

};

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
		std::cout << "Tank is ready. 0x" << this << "\n";
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
		}
		else {
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
	Car(int consumption = 10, int volume = 60)
		: engine(consumption),
		tank(volume),
		driver_inside(false)
	{
		std::cout << "Your car is ready to go.\n";

	}

	~Car()
	{
		std::cout << "Your car is over.\n";
	}

	void get_in()
	{
		driver_inside = true;
		threads.panel_thread = std::thread(&Car::panel, this);
	}

	void get_out()
	{
		driver_inside = false;
		
		if (threads.panel_thread.joinable()) {
			threads.panel_thread.join();
		}
		system("cls");
		std::cout << "Out of the car.\n";
	}

	void control()
	{
		uint8_t key;

		
		do {

			key = _getch();
			std::cout << key << "\n";

			switch (key) {
			case KEYS::ENTER:
				driver_inside ? get_out() : get_in();
				break;

			case KEYS::g:
			case KEYS::G:
				engine.started() ? stop() : start();
				break;

			case KEYS::F:
			case KEYS::f:
				if (driver_inside) {
					std::cout << "Выйдите из машины!\n\a";
				}
				else {
					double fuel;
					std::cout << "Введите уровень топлива: "; std::cin >> fuel;
					tank.fill(fuel);
				}
				break;




			case KEYS::ESCAPE:
				get_out();
				stop();
				break;
			}

			

		} while (key != KEYS::ESCAPE);

	}

	void panel() const
	{
		while (driver_inside) {
			system("cls");
			std::cout << "Fuel level: " << tank.get_fuel_level() << " litres.\t";
			if (tank.get_fuel_level() < 5) {
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 0xCF);
				std::cout << " Low Fule \a";
				SetConsoleTextAttribute(hConsole, 0x07);

			} std::cout << std::endl;
			std::cout << "Engine is " << (engine.started() ? "started" : "stopped") << std::endl;
			
			std::this_thread::sleep_for(100ms);
		}
	}

	void start()
	{
		if (tank.get_fuel_level() > 0 ) {
			engine.start();
			threads.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}

	void stop()
	{
		if (threads.engine_idle_thread.joinable()) {
			threads.engine_idle_thread.join();
		}
		engine.stop();
	}

	void engine_idle()
	{
		while (engine.started() && tank.give_fuel(engine.get_consumption_per_second()))
		{
			std::this_thread::sleep_for(1s);
		}
		
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
	

	struct Threads {
		std::thread panel_thread;
		std::thread engine_idle_thread;
		
	} threads;

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


	Car bmw(13, 94);
	//bmw.info();

	bmw.control();

	return 0;
}