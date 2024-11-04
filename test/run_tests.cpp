#include <chrono>
#include <iostream>
#include <thread>


#include "../AtomicBase/Include/AtomicString.hpp"

using AStr = AtomicString<char>;

void t1_modify(AStr& astr)
{
	for (int i=0; i < 15; ++i)
	{
		astr.ToLower();
		std::cout << astr << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void t2_modify(AStr& astr)
{
	for (int i=0; i < 15; ++i)
	{
		astr.ToUpper();
		std::cout << astr << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}



int main()
{
    AtomicString<char> astr = "HELLOWORLDHOWAREYOUDOING";

	std::cout << "Starting test... " << std::endl;

	std::thread t1{ t1_modify, std::ref(astr) };
	std::thread t2{ t2_modify, std::ref(astr) };

	t1.join();
	t2.join();

	std::cout << "... test complete!  Output should be uniformly upper or lower case " << std::endl;

	return 0;
}