#include <iostream>

int main() {
	float p1x = 0, p1y = 0, p2x = 0, p2y = 0, p3x = 0, p3y = 0;

	std::cout << "Enter x of a first point: ";
	std::cin >> p1x;
	std::cout << "Enter y of a first point: ";
	std::cin >> p1x;

	std::cout << std::endl;

	std::cout << "Enter x of a second point: ";
	std::cin >> p2x;
	std::cout << "Enter y of a second point: ";
	std::cin >> p2x;

	std::cout << std::endl;

	std::cout << "Enter x of a third point: ";
	std::cin >> p3x;
	std::cout << "Enter y of a third point: ";
	std::cin >> p3x;

	std::cout << std::endl;

	if (p1y / p1x == p2y / p2x)
		if (p1y / p1x == p3y / p3x)
		{
			std::cout << "All three points are on one line";
			return 0;
		}
	std::cout << "Points are not on one line";
}