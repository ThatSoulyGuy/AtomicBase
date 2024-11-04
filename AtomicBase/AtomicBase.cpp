#include <iostream>
#include "AtomicString.hpp"

int main()
{
    AtomicString<char> atomicStr = "Hello";

	std::cout << atomicStr << std::endl;
    
    return 0;
}