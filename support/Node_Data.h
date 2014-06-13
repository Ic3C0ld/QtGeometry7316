#pragma once

#include "AVLTree.h"
#include <iostream>


//example object for use in AVLTree class
class Point 
{
public:
	Point(double x_ = 0, double y_ = 0) :x(x_), y(y_)
	{
		
	}

	Point(const Point& p){
		x = p.x;
		y = p.y;
	}

	Point& operator=(const Point& rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	bool operator<( const Point& rhs)
	{
			if (y < rhs.y)
			{
				return true;
			}
			else if (y == rhs.y)
			{
				if (x >= rhs.x)
				{
					return false;
				}
				return true;
			}
			else
			{
				return false;
			}
	}
	bool operator>(const Point& rhs)
	{
		bool result = operator<(rhs)||operator==(rhs);
		return !result;
	}

	bool operator==(const Point& rhs)
	{
		return  (x == rhs.x && y == rhs.y);
	}
	//this actually not so useful
	//will be dropped
	std::string toString()
	{
		using namespace std;

		stringstream stream;
		string str;

		stream << "x:" << x << " " << "y:" << y;

		str = stream.str();
		return stream.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const Point& p)
	{
		os << "x:" << p.x << " " << "y:" << p.y;
		return os;
	}

	// DATA
	double x, y;

};

