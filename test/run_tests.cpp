#include <chrono>
#include <iostream>
#include <thread>
#include "AtomicString.hpp"

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
	auto midpoint = astr.begin();
	std::advance(midpoint, astr.Length() / 2);

	std::ostringstream buffer;

	for (int i = 0; i < 15; ++i) 
	{
		for (auto it = midpoint; it != astr.end(); ++it)
		{
			*it = '1';
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
		}

		buffer << astr << '\n';
	}

	std::cout << buffer.str();
}

void to_nines(AStr& astr)
{
	auto midpoint = astr.begin();
	std::advance(midpoint, astr.Length() / 2);

	std::ostringstream buffer;

	for (int i = 0; i < 15; ++i) 
	{
		for (auto it = astr.begin(); it != midpoint; ++it) 
		{
			*it = '9';
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		buffer << astr << '\n';
	}

	std::cout << buffer.str();
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

	std::cout << "... iterator test complete!  Output should be uniformly 1s or 9s. If you see parts of the original string leaking into the newly formatted one during iterator tests, that is NOT ThreadSafeIterator's fault, rather just the way std::cout handles printing." << std::endl;
	std::cout << "std::cout is not thread-safe when accessed from multiple threads. Even if ThreadSafeIterator is managing concurrent iteration and modification safely, simultaneous calls to std::cout << astr << std::endl; from two threads can result in interleaved or partially overwritten output, which leads to visible artifacts (e.g., partial remnants like \"LDHO\")" << std::endl;

	return 0;
}