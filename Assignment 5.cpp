// Assignment 5
// Complex number class
// Sam Frost 9907936

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class complexNum {
private:
	// declare data types
	double Re;
	double Im;

public:
	// Constructors
	complexNum() : Re(0), Im(0) {} // default constructor
	complexNum(double x) : Re(x), Im(0) {} // if only one argument given, take as real number
	complexNum(double x, double y) : Re(x), Im(y) {} // parametized constructor, overloading default one

	// Destructor
	~complexNum() {}

	// declare functions
	void setRe(double x);
	void setIm(double y);
	double getRe();
	double getIm();
	double modulus();
	double argument();
	complexNum conjugate();
	friend ostream & operator << (ostream &os, const complexNum &z);
	friend istream & operator >> (istream &is, complexNum &z);

	// overloaded operators (within class)
	complexNum & operator + (const complexNum &z) const {
		complexNum sum(Re + z.Re, Im + z.Im);
		return sum;
	}
	complexNum & operator - (const complexNum &z) const {
		complexNum sub(Re - z.Re, Im - z.Im);
		return sub;
	}
	complexNum & operator * (const complexNum &z) const {
		complexNum product(Re*z.Re - Im*z.Im, Re*z.Im + Im*z.Re);
		return product;
	}
	complexNum & operator / (const complexNum &z) const {
		complexNum z1(Re, Im);
		complexNum z2 = z; // have to copy argument to perform conjugation since const is being used
		complexNum numerator(z1 * z2.conjugate());
		double denomenator(z2.modulus() * z2.modulus());
		complexNum fraction(numerator.Re / denomenator, numerator.Im / denomenator);
		return fraction;
	}
};

// function to set real part of complex number
void complexNum::setRe(double x) { Re = x; }

// function to set imaginary part of complex number
void complexNum::setIm(double y) { Im = y; }

// function to return real part of complex number
double complexNum::getRe() { return Re; }

// function to return imaginary part of complex number
double complexNum::getIm() { return Im; }

// function to return modulus of complex number
double complexNum::modulus() { return sqrt(Re*Re + Im*Im); }

// function to return the argument of a complex number
double complexNum::argument() { return atan(Im / Re); }

// function to return the complex conjugate of a complex number
complexNum complexNum::conjugate() {
	complexNum conjugate(Re, -Im);
	return conjugate;
}

// friend function to overload insertion operator 
ostream & operator << (ostream &os, const complexNum &z) {
	os << z.Re;
	if (z.Im >= 0) os << " + " << z.Im << "i";
	if (z.Im < 0) os << " - " << -z.Im << "i";
	return os;
}

// friend function to overload extraction operator 
istream & operator >> (istream &is, complexNum &z) {
	string inStr, reStr, imStr;
	double Re, Im;
	is >> inStr;

	// read in strings
	bool seenIm(false);
	int reSign(1), imSign(1);
	for (int i(0); i < inStr.size(); i++) {
		bool isSign(false);
		if ((i < inStr.size()-1 && inStr[i+1] == 'i') || inStr[i] == 'i') {
			seenIm = true;
			isSign = true;
		}
		if (inStr[i] == '+') {
			isSign = true;
			if (!seenIm) reSign = 1;
			if (seenIm) imSign = 1;
		}
		if (inStr[i] == '-') {
			isSign = true;
			if (!seenIm) reSign = -1;
			if (seenIm) imSign = -1;
		}
		if (!seenIm && !isSign) reStr.push_back(inStr[i]);
		if (seenIm && !isSign) imStr.push_back(inStr[i]);
	}

	// attempt to convert to doubles
	try {
		Re = reSign * stod(reStr); 
		Im = imSign * stod(imStr); 
		z.setRe(Re);
		z.setIm(Im);
	}
	catch (invalid_argument) {
		// error in converting string to double
		cerr << "Error in converting to complex number" << endl;
		z.setRe(0);
		z.setIm(0);
	}
	return is;
}

int main() {
	complexNum a(3, 4);
	complexNum b(1, -2);

	// demonstrating friend function to allow insertion of complex objects into ostream
	cout << "Initial numbers (printed using overloaded << operator):" << endl;
	cout << "a = " << a << endl
		 << "b = " << b << endl;

	// demonstrating functions to return Re and Im parts
	cout << "\nSeparating into real and imaginary parts (using functions to return Re and Im):" << endl
		 << "Re(a) = " << a.getRe() << ", Im(a) = " << a.getIm() << endl
		 << "Re(b) = " << b.getRe() << ", Im(b) = " << b.getIm() << endl;
	
	// demonstrating function to return complex conjugate
	complexNum a_ = a.conjugate();
	cout << "\nConjugating a:" << endl
		 << "a  = " << a << endl
		 << "a* = " << a_ << endl
		 << "b  = " << b << endl;

	// demonstrating function to return modulus
	cout << "\nCalculating moduli:" << endl
		 << "|a|  = " << a.modulus() << endl
		 << "|a*| = " << a_.modulus() << endl
		 << "|b|  = " << b.modulus() << endl;

	// demonstrating function to return argument
	cout << "\nCalculating arguments (in radians):" << endl
		 << "arg(a)  = " << a.argument() << endl
		 << "arg(a*) = " << a_.argument() << endl
		 << "arg(b)  = " << b.argument() << endl;

	// demonstrating functions to overload +,-,*,/ operators
	complexNum sum(a + b);
	complexNum sub(a - b);
    complexNum product(a * b);
    complexNum fraction(a / b);
	cout << "\nPerforming operatations from overloaded operators:" << endl
		 << "a + b = " << sum << endl
		 << "a - b = " << sub << endl
		 << "a * b = " << product << endl
		 << "a / b = " << fraction << endl;

	// demonstrating function to read in complex number from istream
	cout << "\nDemonstration of function to allow insertion of complex number from istream"
		 << "\nPlease enter a non-zero complex number of the form a +/- ib without spaces" << endl;
	bool inputOk(false);
	complexNum numberIn;
	cin >> numberIn;
	if (numberIn.getRe() != 0 && numberIn.getIm() != 0) inputOk = true;
	while (!inputOk) {
		cout << "Please try again: ";
		cin >> numberIn;
		if (numberIn.getRe() != 0 && numberIn.getIm() != 0) inputOk = true;
	}
	cout << "Your complex number was " << numberIn << endl;

	return 0;
}
