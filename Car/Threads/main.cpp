#include <iostream>
#include <thread>

using namespace std::chrono_literals;

bool finish = false;

void plus()
{
	while (!finish) {
		std::cout << "+ ";
		std::this_thread::sleep_for(1s);
	}
}

void minus()
{
	while (!finish)
	{
		std::cout << "- ";
		std::this_thread::sleep_for(1s);
	}
}

int main()
{
	setlocale(0, "");

	/*plus();
	minus();*/
	
	std::thread plus_thread(plus);
	std::thread minus_thread(minus);
	
	std::cin.get();
	finish = true;

	if (plus_thread.joinable())
		plus_thread.join();

	if (minus_thread.joinable())
		minus_thread.join();

	return 0;
}