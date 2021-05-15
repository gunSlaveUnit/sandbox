#include <iostream>

class Point
{
public:

	Point(float X = 0.0, float Y = 0.0)
		: x(X), y(Y)
	{}

	void setPointCoordinates()
	{
		std::cout << "Enter point coordinates:\n";
		std::cout << "x = "; std::cin >> x;
		std::cout << "y = "; std::cin >> y;
	}

	float getXCoordinate() { return x; }
	float getYCoordinate() { return y; }

private:

	float x, y;
};

class Circle
{
public:

	Circle(float Radius = 0.0)
		: radius(Radius)
	{}

	void setRadiusCircle()
	{
		std::cout << "Enter the radius of the circle:\n";
		std::cout << "R = "; std::cin >> radius;
	}

	float getRadius() { return radius; }

private:

	float radius;
};

bool checkPointBelongsCircle(Point& pointObj, Circle& circleObj);

int main()
{
	Point point;
	point.setPointCoordinates();

	Circle circle;
	circle.setRadiusCircle();

	if (checkPointBelongsCircle(point, circle))
		std::cout << "The point belongs to the circle." << std::endl;
	else
		std::cout << "The point does not belong to the circle." << std::endl;
}

bool checkPointBelongsCircle(Point& pointObj, Circle& circleObj)
{
	// (x*x + y*y) <= R*R
	// To not find sqrt
	return pointObj.getXCoordinate() * pointObj.getXCoordinate() + pointObj.getYCoordinate() * pointObj.getYCoordinate() <= 
		circleObj.getRadius() * circleObj.getRadius();
}