#include <iostream>
#include <cmath>

struct Polynom
{
	double a;
	double b;
	double c;
};

void roots(Polynom p) {
	double x1, x2, D;
	D = p.b * p.b - 4 * p.a * p.c;
	if (p.a == 0)
		if (p.b != 0)
			std::cout << "x = " << p.c / p.b << std::endl;
		else if (p.c == 0)
			std::cout << "x is any number" << std::endl;
		else
			std::cout << "There are no roots" << std::endl;
	else
	{
		if (D == 0)
			std::cout << "The only root is " << -p.b / 2 / p.a << std::endl;
		else if (D > 0)
		{
			x1 = (-p.b + sqrt(D)) / 2 / p.a;
			x2 = (-p.b - sqrt(D)) / 2 / p.a;

			std::cout << "Real roots are: " << std::endl;
			std::cout << "x1 = " << x1 << " x2 = " << x2 << std::endl;
		}
		else
		{
			std::cout << "Complex roots are: " << std::endl;
			std::cout << "x1 = " << -p.b / 2 / p.a << "+" << sqrt(-D) / 2 / p.a << "i" << std::endl;
			std::cout << "x2 = " << -p.b / 2 / p.a << -sqrt(-D) / 2 / p.a << "i" << std::endl;
		}
	}
}

int main()
{
	Polynom P1, P2, P3, P4, P5, P6;
	P1.a = 1; P1.b = -5; P1.c = 6;
	P2.a = 0; P2.b = 2; P2.c = 3;
	P3.a = 0; P3.b = 0; P3.c = 3;
	P4.a = 0; P4.b = 0; P4.c = 0;
	P5.a = 4; P5.b = 4; P5.c = 1;
	P6.a = 1; P6.b = 1; P6.c = 1;

	roots(P1);
	roots(P2);
	roots(P3);
	roots(P4);
	roots(P5);
	roots(P6);

	return 0;
}    