/*
from time import perf_counter

def time_measure(derived):
	def wrapper(*args, **kwargs):
		start = perf_counter()
		result = derived(*args, **kwargs)
		end = perf_counter()
		print(f"Done for {end - start}")
	return wrapper


@time_measure
def GCD(first, second):
	"""
	Finds a greatest common divisor for the first and second numbers.
	Uses Euclid's algorithm.
	"""

	while(second != 0):
	remainder = first % second
	first = second
	second = remainder

	return first
*/

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
