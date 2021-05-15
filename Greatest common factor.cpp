#include <iostream>

int main()
{
	int64_t FirstNumber = -1, SecondNumber = -1;

	while (FirstNumber < 0 || SecondNumber < 0)
	{
		std::cout << "Enter two numbers: ";
		std::cin >> FirstNumber >> SecondNumber;
	}

	while (FirstNumber != SecondNumber)
	{
		if (FirstNumber > SecondNumber)
			FirstNumber -= SecondNumber;
		else
			SecondNumber -= FirstNumber;
	}

	std::cout << "Greatest common factor is " << FirstNumber << std::endl;
}