#include <iostream>


class Brick
{
public:

	Brick(double l = 0, double w = 0, double h = 0)
		: length(l), width(w), height(h) {}

	void setBrickDimensions()
	{
		std::cout << "Enter the length of the brick: ";
		std::cin >> length;

		std::cout << "Enter the width of the brick: ";
		std::cin >> width;

		std::cout << "Enter the height of the brick: ";
		std::cin >> height;
	}

	double getLength() { return length; }
    double getWidth()  { return width;  }
    double getHeight() { return height; }

private:

	double length, width, height;
};


class Hole
{
public:

	Hole(double w = 0, double h = 0)
		: width(w), height(h) {}

	void setHoleDimensions()
	{
		std::cout << "Enter the width of the hole: ";
		std::cin >> width;

		std::cout << "Enter the height of the hole: ";
		std::cin >> height;
	}

	double getWidth() { return width; }
	double getHeight() { return height; }

private:

	double width, height;
};


bool checkPassageBrickIntoHole(Brick& brick, Hole& hole)
{
	/*
	*  (x <= a && (y <= b || z <= b) ||
	*	y <= a && (x <= b || z <= b) ||
	*	z <= a && (x <= b || y <= b))
	*
	*	a, b    - hole
	*	x, y, z - brick
	*/
	
	return brick.getLength() <= hole.getWidth() && (brick.getWidth()  <= hole.getHeight() || brick.getHeight() <= hole.getHeight()) ||
		   brick.getWidth()  <= hole.getWidth() && (brick.getLength() <= hole.getHeight() || brick.getHeight() <= hole.getHeight()) ||
		   brick.getHeight() <= hole.getWidth() && (brick.getLength() <= hole.getHeight() || brick.getWidth()  <= hole.getHeight());
}


int main()
{
	Brick* b = new Brick;
	b->setBrickDimensions();

	std::cout << std::endl;

	Hole* h = new Hole;
	h->setHoleDimensions();
	
	if (checkPassageBrickIntoHole(*b, *h))
		std::cout << "\nYes.\n" << std::endl;
	else 
		printf("\nNo.\n");

	delete b;
	delete h;
}