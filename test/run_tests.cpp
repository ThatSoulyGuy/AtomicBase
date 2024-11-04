#include <chrono>
#include <iostream>
#include <thread>


#include "../AtomicBase/Include/AtomicString.hpp"

using AStr = AtomicString<char>;

void lower_modify(AStr& astr)
{
	for (int i = 0; i < 15; ++i)
	{
		astr.ToLower();
		std::cout << astr << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void upper_modify(AStr& astr)
{
	for (int i = 0; i < 15; ++i)
	{
		astr.ToUpper();
		std::cout << astr << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void to_ones(AStr& astr)
{
	for (int i = 0; i < 15; ++i)
	{
		for (auto it = astr.begin(); it != astr.end(); ++it)
		{
			*it = '1';
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
		}
		std::cout << astr << std::endl;
	}
}

void to_nines(AStr& astr)
{
	for (int i = 0; i < 15; ++i)
	{
		auto it = astr.end();
		--it;
		for (; ; --it)
		{
			*it = '9';
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			if (it == astr.begin())
				break;
		}
		std::cout << astr << std::endl;
	}
}



int main()
{
    AtomicString<char> astr = "HELLOWORLDHOWAREYOUDOING";

	std::cout << "Starting upper/lower test ... " << std::endl;

	std::thread t1{ lower_modify, std::ref(astr) };
	std::thread t2{ upper_modify, std::ref(astr) };

	t1.join();
	t2.join();

	std::cout << "... upper/lower test complete!  Output should be uniformly upper or lower case " << std::endl;
	std::cout << std::endl;


	std::cout << "Starting iterator test ... " << std::endl;

	std::thread t3{ to_ones, std::ref(astr) };
	std::thread t4{ to_nines, std::ref(astr) };

	t3.join();
	t4.join();

	std::cout << "... iterator test complete!  Output should be uniformly 1s or 9s " << std::endl;

	return 0;
}