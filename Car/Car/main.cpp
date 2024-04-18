#include <iostream>

#define MIN_TANK_VOLUME		20
#define MAX_TANK_VOLUME		120

int check_voluem(int volume)
{
	if (volume < MIN_TANK_VOLUME) {
		volume = MIN_TANK_VOLUME;
	}
	else if (volume > MAX_TANK_VOLUME) {
		volume = MAX_TANK_VOLUME;
	}
	return volume;
}

class Tank 
{
public:
	Tank(int volume) : VOLUME(check_voluem(volume))
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

int main()
{
	setlocale(0, "Rus");

	Tank tank(50);
	
	int fuel;
	do {
		std::cout << "Сколько: "; std::cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (fuel > 0);

	return 0;
}