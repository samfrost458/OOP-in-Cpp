// Assignment 8
// 2D and 3D shape class heirarchy
// Sam Frost 9907936

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES // contains pi as M_PI, when including math.h
#include <math.h>

using namespace std;

// (abstract) base shape class
class shape {
public:
	// destructor
	virtual ~shape() {}

	// declare virtual functions
	virtual double perimeter() = 0;
	virtual double area() = 0;
	virtual double volume() = 0;
	virtual string shapeName() = 0;
	virtual bool is2D() = 0;
	virtual bool is3D() = 0;
};

// 2D shape class
class twoDshape : public shape {
public:
	double volume() { cout << shapeName() << " is 2D, so has no volume" << endl; return 0; }
	bool is2D() { return true; }
	bool is3D() { return false; }
};

// 3D shape class
class threeDshape : public shape {
public:
	double perimeter() { cout << shapeName() << " is 3D, so has no perimeter" << endl; return 0; }
	bool is2D() { return false; }
	bool is3D() { return true; }
};

// rectangle class
class rectangle : public twoDshape {
protected:
	double sideA, sideB;
public:
	// constructor
	rectangle(double a, double b) : sideA(a), sideB(b) {}

	// destructor
	~rectangle() { cout << "Rectangle destructor called" << endl; }

	// overridding virtual functions
	double perimeter() { return 2 * (sideA + sideB); }
	double area() { return sideA * sideB; }
	string shapeName() { return "Rectangle"; }
};

// square class (is a rectangle)
class square : public rectangle {
public:
	// constructor
	square(double a) : rectangle(a, a) {}

	// destructor
	~square() { cout << "Square destructor called" << endl; }
	string shapeName() { return "Square"; }
};

// ellipse class
class ellipse : public twoDshape {
protected:
	double semiMajorAxis, semiMinorAxis;
public:
	// constructor
	ellipse(double a, double b) : semiMajorAxis(a), semiMinorAxis(b) {}

	// destructor
	~ellipse() { cout << "Ellipse destructor called" << endl; }

	// overridding virtual functions
	double perimeter() {
		// approximation for ellipse perimeter, could have used first few terms of an infinite series instead
		double h(pow(semiMajorAxis - semiMinorAxis, 2.0) / pow(semiMajorAxis + semiMinorAxis, 2.0));
		return M_PI * (semiMajorAxis + semiMinorAxis) * (1 + 3 * h / (10 + sqrt(4 - 3 * h)));
	}
	double area() { return M_PI * semiMajorAxis * semiMinorAxis; }
	string shapeName() { return "Ellipse"; }
};

// circle class (is an ellipse)
class circle : public ellipse {
public:
	// constructor
	circle(double r) : ellipse(r, r) {}

	// destructor
	~circle() { cout << "Circle destructor called" << endl; }
	string shapeName() { return "Circle"; }

	double perimeter() { return 2 * M_PI * semiMajorAxis; }
};

// cuboid class
class cuboid : public threeDshape {
protected:
	double sideA, sideB, sideC;
public:
	// constructor
	cuboid(double a, double b, double c) : sideA(a), sideB(b), sideC(c) {}

	// destructor
	~cuboid() { cout << "Cuboid destructor called" << endl; }

	// overridding virtual functions
	double area() { return 2 * sideA*sideB + 2 * sideB*sideC + 2 * sideA*sideC; }
	double volume() { return sideA * sideB * sideC; }
	string shapeName() { return "Cuboid"; }
};

// cube class (is a cuboid)
class cube : public cuboid {
public:
	// constructor
	cube(double a) : cuboid(a, a, a) {}

	// destructor
	~cube() { cout << "Cube destructor called" << endl; }
	string shapeName() { return "Cube"; }
};

// ellipsoid class
class ellipsoid : public threeDshape {
protected:
	double semiAxisA, semiAxisB, semiAxisC;
public:
	// constructor
	ellipsoid(double a, double b, double c) : semiAxisA(a), semiAxisB(b), semiAxisC(c) {}

	// destructor
	~ellipsoid() { cout << "Ellipsoid destructor called" << endl; }

	// overridding virtual functions
	double area() {
		// approximate solution, accurate to 1.061%, wikipedia
		const double p(1.6075);
		return 4 * M_PI * pow((pow(semiAxisA*semiAxisB, p) + pow(semiAxisA*semiAxisC, p) + pow(semiAxisB*semiAxisC, p)) / 3, 1 / p);
	}
	double volume() { return 4 * M_PI * semiAxisA * semiAxisB * semiAxisC / 3; }
	string shapeName() { return "Ellipsoid"; }
};

// sphere class (is an ellipsoid)
class sphere : public ellipsoid {
public:
	// constructor
	sphere(double r) : ellipsoid(r, r, r) {}

	// destructor
	~sphere() { cout << "Sphere destructor called" << endl; }

	// overridding virtual functions
	double area() { return 4 * M_PI * pow(semiAxisA, 2.0); } // exact result, unlike ellipsoid area approximation
	string shapeName() { return "Sphere"; }
};

// prism class
class prism : public threeDshape {
protected:
	double length;
	shape* base;
public:
	// constructor
	prism(double l, shape* b) : length(l), base(b) { if (!base->is2D()) { cerr << "Error: prism base is not 2D" << endl; exit(1); } }

	// destructor
	~prism() { delete base; cout << "Prism destructor called" << endl; }

	// overridding virtual functions
	double area() { return 2 * base->area() + base->perimeter() * length; }
	double volume() { return base->area() * length; }
	string shapeName() { return "Prism"; }
};


int main() {
	// impliment an array or vector of at least 8 base class pointers, each pointing to a different shape
	vector <shape*> shapeVector;
	shapeVector.push_back(new rectangle(1, 2));
	shapeVector.push_back(new square(2));
	shapeVector.push_back(new ellipse(1, 2));
	shapeVector.push_back(new circle(2));
	shapeVector.push_back(new cuboid(1, 2, 3));
	shapeVector.push_back(new cube(2));
	shapeVector.push_back(new ellipsoid(1, 2, 3));
	shapeVector.push_back(new sphere(2));
	shapeVector.push_back(new prism(2, new square(2)));

	// output area/volume for each shape
	for (size_t i(0); i < shapeVector.size(); i++) {
		cout << left << setw(17) << (shapeVector[i]->shapeName() + " area") << "= " << shapeVector[i]->area() << endl;
	}
	cout << endl;
	for (size_t i(0); i < shapeVector.size(); i++) {
		cout << left << setw(17) << (shapeVector[i]->shapeName() + " volume") << "= " << shapeVector[i]->volume() << endl;
	}
	cout << endl;

	// delete objects and array when finished
	for (auto shapeVectorIt = shapeVector.begin(); shapeVectorIt < shapeVector.end(); shapeVectorIt++) {
		delete *shapeVectorIt;
	}
	shapeVector.clear();
	cout << endl << "Shape vector now has size " << shapeVector.size() << endl;

	return 0;
}