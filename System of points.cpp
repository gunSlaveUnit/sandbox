#include <iostream>
#include <vector>


class Point
{
public:

	Point(float xc = 0, float yc = 0, float zc = 0, float wght = 0)
		: x(xc), y(yc), z(zc), weight(wght) {}

	void setPointParameters()
	{
		std::cin >> x >> y >> z >> weight;
	}

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	float getW() { return weight; }

private:

	float x, y, z;
	float weight;
};


class SystemPoints
{
public:

	SystemPoints() 
	{
		pointsInSystem = {};
		amountPointsInSystem = 0;

		center.xs = center.ys = center.zs = 0;
	}
	
	SystemPoints(std::vector<Point> points)
		: pointsInSystem(points) 
	{
		amountPointsInSystem = points.size();

		center.xs = center.ys = center.zs = 0;
	}

	void setAmountPoints()
	{
		std::cout << "Enter a number of the points in the system: ";
		std::cin >> amountPointsInSystem;
	}

	void setPointsParameters()
	{
		std::cout << "\nFor each point separated by a space, specify coordinates and weight:\n" << std::endl;
		
		Point* pnt = new Point;
		for (int i = 0; i < amountPointsInSystem; ++i)
		{ std::cout<<"#"<< i + 1 << " point: ";
			pnt->setPointParameters();
			pointsInSystem.push_back(*pnt);
		}
		delete pnt;
	}

	void calculateCoordinatesCenterSystem()
	{
		float sumX = 0, sumY = 0, sumZ = 0, sumW = 0;

		for (Point& p : pointsInSystem)
		{
			sumX += p.getX() * p.getW();
			sumY += p.getY() * p.getW();
			sumZ += p.getZ() * p.getW();
			sumW += p.getW();
		}

		center.xs = sumX / sumW;
		center.ys = sumY / sumW;
		center.zs = sumZ / sumW;
	}

	void showCenterSystem()
	{
		std::cout << "\nCoordinates of the center: " 
			<< center.xs << " " << center.ys << " " << center.zs << std::endl;
	}

private:

	struct CenterSystem
	{
		float xs, ys, zs;
	} center;

	std::vector<Point> pointsInSystem;
	int amountPointsInSystem;
};


int main()
{
	SystemPoints* system = new SystemPoints;
	system->setAmountPoints();
	system->setPointsParameters();
	system->calculateCoordinatesCenterSystem();
	system->showCenterSystem();

	delete system;
}
	