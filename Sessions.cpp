#include <iostream>

int main() {
	int hallNumber = 0,
		sessionNumber = 0,
		ticketCost = 0,
		ticketsNumber = 0;

	float discount   = 0,
		  finalCost  = 0;

	std::cout << "1. Red Hall - \"The Chronicles of Narnia\"" << std::endl;
	std::cout << "2. Blue Hall - \"Aliens\"" << std::endl;
	std::cout << "3. Green Hall - \"Avatar\"" << std::endl;

	while (hallNumber < 1 || hallNumber > 3) {
		std::cout << "\nEnter the hall number (1-3): ";
		std::cin >> hallNumber;
	}

	std::cout << "\nSessions For ";
	switch (hallNumber) {
	case 1:
		std::cout << "Red Hall: " << std::endl;
		std::cout << "12 - 25$\n16 - 35$\n20 - 45$\n" << std::endl;
		while (sessionNumber != 12 && sessionNumber != 16 && sessionNumber != 20) {
			std::cout << "Enter a session number: ";
			std::cin >> sessionNumber;
		}
		switch (sessionNumber) {
		case 12: ticketCost = 25; break;
		case 16: ticketCost = 35; break;
		case 20: ticketCost = 45;
		}
		break;
	case 2:
		std::cout << "Blue Hall: " << std::endl;
		std::cout << "10 - 25$\n13 - 35$\n16 - 35$\n" << std::endl;
		while (sessionNumber != 10 && sessionNumber != 13 && sessionNumber != 16) {
			std::cout << "Enter a session number: ";
			std::cin >> sessionNumber;
		}
		switch (sessionNumber) {
		case 10: ticketCost = 25; break;
		case 13: ticketCost = 35; break;
		case 16: ticketCost = 35;
			break;
		case 3:
			std::cout << "Green Hall: " << std::endl;
			std::cout << "10 - 35$\n14 - 45$\n18 - 45$\n" << std::endl;
			while (sessionNumber != 10 && sessionNumber != 14 && sessionNumber != 18) {
				std::cout << "Enter a session number: ";
				std::cin >> sessionNumber;
			}
			switch (sessionNumber) {
			case 10: ticketCost = 35; break;
			case 14: ticketCost = 45; break;
			case 18: ticketCost = 45;
			}
		}
	}
	while (ticketsNumber < 1) {
		std::cout << "\nEnter a number of tickets: ";
		std::cin >> ticketsNumber;
	}

	finalCost = ticketsNumber * ticketCost;
	if (ticketsNumber > 5 && ticketsNumber < 10)
		discount = finalCost / 20;
	else if (ticketsNumber > 10)
		discount = finalCost / 10;

	finalCost -= discount;

	std::cout << "\nSession cost: " << finalCost;
}