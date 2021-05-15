#include <iostream>
#include <vector>

int main() {
	int testsNumber = 0;
	std::cin >> testsNumber;

	std::string* answers = new std::string[testsNumber];
	int number1, number2, number3, number4;
	for (int i = 0; i < testsNumber; ++i) {
		std::cin >> number1 >> number2;
		std::cin >> number3 >> number4;

		if ((number1 == number2 || number1 == number3 || number1 == number4) ||
			number2 == number3 || number2 == number4 || number3 == number4) {
			if (number1 + number3 == number2 && number2 == number4 ||
				number1 + number4 == number3 && number2 == number3 ||
				number2 + number3 == number1 && number1 == number4 ||
				number2 + number4 == number3 && number1 == number3)
				answers[i] = "Yes";
			else
				answers[i] = "No";
		}
	}

	for (int i = 0; i < testsNumber; ++i)
		std::cout << answers[i] << std::endl;
}