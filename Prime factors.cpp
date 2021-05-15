#include <iostream>
#include <vector>

uint64_t& inputNumber()
{
	uint64_t n = 0;
	std::cout << "Enter a number: ";
	std::cin >> n;

	return n;
}

std::vector<uint64_t> calculatePrimeFactorsNumber(uint64_t& pNumber)
{
	std::vector<uint64_t> primeFactors;
	
	if (pNumber % 2 == 0) primeFactors.push_back(2);

	for (uint64_t i = 3; i <= pNumber; i += 2)
	{
		if (pNumber % i == 0)
		{
			bool primnumber = true;
			for (int c = 2; c * c <= i && primnumber; c++)
				if (i % c == 0) primnumber = false;
			if (primnumber) primeFactors.push_back(i);
		}
	}

	return primeFactors;
}

void showFactors(std::vector<uint64_t>& pFactors)
{
	std::cout << "Prime factors: " << std::endl;

	for (auto& factor : pFactors)
		std::cout << factor << " ";
}

int main()
{
	uint64_t number = inputNumber();
	
	std::vector<uint64_t> factors = calculatePrimeFactorsNumber(number);

	showFactors(factors);
}

