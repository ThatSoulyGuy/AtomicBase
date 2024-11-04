#include <iostream>
#include "AtomicString.hpp"

int main()
{
    AtomicString<char> atomicStr = "Hello";
    AtomicString<wchar_t> otherAtomicStr = L"yelp.com";

    atomicStr += " World";
    atomicStr -= "orl";

    for (char c : atomicStr)
        otherAtomicStr += std::to_string(c);

	std::cout << atomicStr << std::endl;
    std::wcout << otherAtomicStr << std::endl;
    
    return 0;
}