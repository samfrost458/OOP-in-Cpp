// Assignment 1
// Single electron Bohr transition calculator
// Sam Frost 9907936

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath> // includes pow() function
#include <algorithm> // includes remove_if() and transform()

using namespace std;

// function to strip string of white space and remove uppercase
string noSpaceOrLowercase(string str) {
	// from http://www.cplusplus.com/forum/beginner/9557/
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	// from https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

// function to ask user for an integer
int askInteger() {
	int input;
	// check input for validity
	while (!(cin >> input) || cin.get() != '\n' || cin.fail()) {
		// cin.get() gets remainder after first numerical characters are extracted to 'input'
		// cin.clear() clears error flag
		// cin.ignore(10000, '\n') ignores up to 10000 characters (arbitrarily large) or until '\n'
		cout << "Error: please enter an integer ";
		cin.clear(); cin.ignore(10000, '\n');
	}
	return input;
}

// function to compare user string input to string arguments
string askAorB(string A, string B) {
	string input;
	while (!(cin >> input) || cin.fail() || !(noSpaceOrLowercase(input) == A || noSpaceOrLowercase(input) == B)) {
		// check if modified (without spaces or uppercase) input is equal to the strings A or B
		cout << "Error: please enter '" << A << "' or '" << B << "' ";
		cin.clear(); cin.ignore(10000, '\n');
	}

	if (noSpaceOrLowercase(input) == A) {
		return A;
	}
	if (noSpaceOrLowercase(input) == B) {
		return B;
	}
}

// function to find the energy of a particular level in eV or J
double energy(int Z, int n, string units) {
	double energy;
	const double eV_to_J{ 1.602e-19 };
	if (units == "ev") {
		energy = -13.6 * pow(Z, 2) / pow(n, 2); // in eV
	}
	if (units == "j") {
		energy = -13.6 * pow(Z, 2) / pow(n, 2) * eV_to_J; // in J
	}
	return energy;
}

int main() {
	// declare variables
	double delta;
	string units;
	string repeat("y");
	cout << "This program calculates the single-electron Bohr model energy of a photon-emitting atomic transition" << endl;

	while (repeat == "y") {
		// initialise integers to 0 to avoid errors in the while loops, these will then necessarily be changed by the user
		// initialise inside this while loop to reset them to 0 if the user wants another calculation
		int n_i{ 0 }, n_f{ 0 }, Z{ 0 };

		// ask the user for the atomic number
		cout << "Enter the atomic number: ";
		
		// check that Z is not greater than 118 (Og) or less than 0
		while (Z > 118 || Z < 1) {
			Z = askInteger();
			if (Z > 118) {
				cout << "Error: atomic number is larger 118, the highest of any known element, please re-enter Z ";
			}
			if (Z < 1) {
				cout << "Error: atomic number must be greater than 0, please re-enter Z ";
			}
		}

		// ask the user for the quantum states
		while (n_i <= n_f) {
			// ask the user for the initial state
			cout << "Enter the principal quantum number for the initial state: ";
			while (n_i <= 1) {
				n_i = askInteger();
				if (n_i <= 1) {
					cout << "Error: initial state must be greater than 1, please re-enter n_i ";
				}
			}
			
			// ask the user for the final state
			cout << "Enter the principal quantum number for the final state: ";
			while (n_f <= 0) {
				n_f = askInteger();
				if (n_f <= 0) {
					cout << "Error: final state must be greater than 0, please re-enter n_f ";
				}
			}

			if (n_i <= n_f) {
				cout << "Error: final state cannot be greater than initial state" << endl;
				// reset n_i and n_f
				n_i = 0; n_f = 0;
			}
		}
		

		
		// ask the user to indicate which units they want their answer in
		cout << "Indicate whether you want the energy in eV or J (eV/J): ";
		units = askAorB("ev", "j");

		// calculate the transition energy in eV or J
		delta = energy(Z, n_i, units) - energy(Z, n_f, units);

		// tell the user the answer
		cout << "The transition energy from n = " << n_i << " to n = " << n_f
			<< " (for Z = " << Z << ") is " << setprecision(3) << delta
			<< " " << units << " (to 3sf)" << endl;
	
		// ask the user if they want to repeat the calculation
		cout << "Do you want to repeat this calculation? (y/n) ";
		repeat = askAorB("y", "n");

		// 
	}

	return 0;
}
