#include <iostream>

void PrintNumberFactors(uint64_t FactoringNumber);

int main()
{
	uint64_t Number = 0;
	std::cout << "Enter a number to search factors: ";
	std::cin >> Number;

	PrintNumberFactors(Number);

	return 0;
}

void PrintNumberFactors(uint64_t FactoringNumber)
{
	std::cout << "Factors of " << FactoringNumber << ": " << std::endl;

	for (int64_t Divisor = 2; FactoringNumber != 1; Divisor++)
	{
		while (FactoringNumber % Divisor == 0)
		{
			std::cout << Divisor << " ";
			FactoringNumber /= Divisor;
		}
	}
}
