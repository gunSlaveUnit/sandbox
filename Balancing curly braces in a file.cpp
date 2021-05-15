#include <iostream>
#include <fstream>

std::string getFileName()
{
	std::cout << "Enter the name of the file: ";
	std::string name;
	std::cin >> name;

	return name;
}

int main()
{
	std::string fileNameCheckBalance = getFileName();
	
	std::ifstream file(fileNameCheckBalance);

	if (file.is_open())
	{
		int amountOpen = 0, amountLock = 0;
		
		char buf;
		file >> buf;
		while (file)
		{
			if (buf == '{')
				amountOpen += 1;
			else if (buf == '}')
				amountLock += 1;

			file >> buf;
		}

		if (amountOpen == amountLock)
		{
			std::cout << "The parentheses are not out of balance.\nNumber of opening and closing brackets = " 
				<< amountLock << std::endl;
		}
		else if (amountOpen != amountLock)
		{
			std::cout << "The brackets are out of balance." << std::endl;
			std::cout << "Number of opening brackets =  = " << amountOpen << std::endl;
			std::cout << "Number of closing brackets = " << amountLock << std::endl;
		}

		file.close();
	}
	else 
		std::cout << "File wasn't opened.";
}