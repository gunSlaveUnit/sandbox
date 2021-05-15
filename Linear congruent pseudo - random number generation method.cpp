// Linear congruent pseudo - random number generation method

#include <iostream>

double congruential(int& x); 

int main()
{
	int number_numbers = 3; // Number of pseudo-random numbers
	int x0 = 6;                    // Initial value (chosen at random 0 <= x0 < m)

	std::cout << "Pseudo-random numbers:\n";
	for (int i = 0; i < number_numbers; ++i)
		std::cout << congruential(x0) << std::endl; 
}

double congruential(int& x) 
{
	int m = 100,             // Generation of pseudo-random numbers in the range of values ??from 0 to 100 (chosen at random m> 0)
		a = 8,               // Multiplier (chosen randomly 0 <= a <= m)
		inc = 65;            // Incremental value (randomly selected 0 <= inc <= m)

	x = ((a * x) + inc) % m; // Formula of linear congruent method for generating pseudo-random numbers

	return (x / static_cast<double>(m));
}