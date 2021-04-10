// Assignment 4
// Galaxy class
// Sam Frost 9907936

// Hubble types: E[0-7], S0, S[a-c], SB[a-c], Irr
// Redshift z in range [0,10]
// Total mass M_tot in range [1e7,1e12] M_sun
// Stellar mass fraction f_* in range [0,0.05]

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class galaxy {
private:
	// declare data types
	string hubbleType;
	double redshift; // in range [0,10]
	double M_tot; // total mass in range [1e7,1e12] M_sun
	double massFrac; // stellar mass fraction in range [0,0.05]
	vector <galaxy> satellites;

public:
	// Constructors
	galaxy() : hubbleType("None"), redshift(0), M_tot(0), massFrac(0) { addAllowedTypes(allowedTypes); } // default constructor
	galaxy(string type, double z, double mass, double frac) {
		// parametized constructor, overloading default one
		addAllowedTypes(allowedTypes);
		bool ok(inputCheckOK(type, z, mass, frac));
		if (ok) {hubbleType = type; redshift = z; M_tot = mass; massFrac = frac;}
		else {
			cerr << "Error: tried to input a galaxy (type '" << type << "') with invalid parameters" << endl
				 << "Default constructor called instead" << endl;
			hubbleType = "None"; redshift = 0; M_tot = 0; massFrac = 0;
		}
	}

	// Destructor
	~galaxy() {
		//cout << "Destroying " << hubbleType << endl;
	}

	// declare functions, defined outside the class
	double stellarMass(); // function to return stellar mass M* = f* M_tot
	void changeType(string newType); // function to change a galaxy's Hubble type
	void setSatellite(galaxy g); // function to add satellite galaxies
	void printGalaxy(); // function to print out an object's data
	bool typeOK(string type); // function to check if type is ok
	bool inputCheckOK(string type, double z, double mass, double frac); // function to check if data is within valid ranges

	// string vector for allowed Hubble types, constructor and function to change type are able to access this without it being an argument since is it public
	vector <string> allowedTypes;
	void addAllowedTypes(vector <string> &allowedTypes) {
		if (allowedTypes.size() == 0) {
			for (int i(0); i < 8; i++) { allowedTypes.push_back("E" + to_string(i)); }
			allowedTypes.push_back("S0");
			vector <string> abc; abc.push_back("a"); abc.push_back("b"); abc.push_back("c");
			for (int i(0); i < abc.size(); i++) { allowedTypes.push_back("S" + abc[i]); }
			for (int i(0); i < abc.size(); i++) { allowedTypes.push_back("SB" + abc[i]); }
			allowedTypes.push_back("Irr");
		}
	}
};

// functions to check if data is within valid ranges
bool galaxy::typeOK(string type) {
	bool ok(true);
	bool foundMatch(false);
	addAllowedTypes(allowedTypes);
	for (int i(0); i < allowedTypes.size(); i++) {if (type == allowedTypes[i]) foundMatch = true;}
	if (!foundMatch) ok = false;
	return ok;
}
bool galaxy::inputCheckOK(string type, double z, double mass, double frac) {
	// checking inputs
	bool ok(true);
	if (0 >= z && z >= 10) ok = false;
	if (1e7 >= mass && mass >= 1e12) ok = false;
	if (0 >= frac && frac >= 0.05) ok = false;
	if (!typeOK(type)) ok = false;
	return ok;
}

// function to return stellar mass M* = f* M_tot
double galaxy::stellarMass() {return massFrac * M_tot;}

// function to change a galaxy's Hubble type
void galaxy::changeType(string newType) {
	if (typeOK(newType)) hubbleType = newType;
	else cerr << "Error: tried to change Hubble type to an invalid type '" << newType << "'" << endl;
}

// function to add satellite galaxies
void galaxy::setSatellite(galaxy g) {satellites.push_back(g);}

// function to print out an object's data
void galaxy::printGalaxy() {
    cout << "Galaxy: [type, z, M_tot, f*] = [" << hubbleType << ", " << redshift << ", " << M_tot << ", " << massFrac << "]";
    
    // print off satellite galaxies if there are any
    cout << ", No. satellites: " << satellites.size() << endl;
    if (satellites.size() != 0) {
        vector <galaxy> :: iterator it;
        for (it = satellites.begin(); it < satellites.end(); it++) {
            cout << "-> Sat:\t["
                 << it -> hubbleType << ", "
                 << it -> redshift << ", "
                 << it -> M_tot << ", "
				 << it -> massFrac << "]"<< endl;
        }
    }
}

int main() {
	// demonstrate constructors, destructors, stellarMass, changeType, satellites
    
    // vector to hold some galaxies, and an iterator to loop through the vector
    vector <galaxy> galaxyList;
    vector <galaxy> :: iterator it;

	// print default galaxy
	galaxyList.push_back( galaxy() );
	cout << "Galaxy with default constructor:" << endl;
	galaxyList[0].printGalaxy();
	cout << endl << endl;
	
	// print intialised galaxies
	galaxyList.push_back( galaxy("E0",1.5,1e10,0.01) );
    galaxyList.push_back( galaxy("S0",6.57,2.5e8,0.046) );
	cout << "Galaxies with parametised constructor:" << endl;
	for (it = galaxyList.begin()+1; it < galaxyList.end(); it++) {
		it -> printGalaxy();
	} cout << endl << endl;

	// attempt to declare a galaxy with invalid propeties
	cout << "Attempting to declare a galaxy with invalid inputs:" << endl;
	galaxyList.push_back( galaxy("asdf", 123, 123, 123) );
	galaxyList[3].printGalaxy();
	cout << endl << endl;
    
    // change one of the galaxy types
	string newType = "E1";
	cout << "Changing one of the galaxy's Hubble types:\nOld galaxy: " << endl; galaxyList[1].printGalaxy();
	galaxyList[1].changeType(newType);
	cout << "New galaxy: " << endl; galaxyList[1].printGalaxy();
	cout << endl << endl;

	// attempting to change another galaxy's type to an invalid type
	newType = "asdf";
	cout << "Attempting to change a galaxy's type to an invalid type:\nOld galaxy: " << endl; galaxyList[2].printGalaxy();
	galaxyList[2].changeType(newType);
	cout << "New galaxy: " << endl; galaxyList[2].printGalaxy();
	cout << endl << endl;

	// demonstrate stellar mass function
	cout << "Calculating stellar mass for a galaxy:" << endl;
	galaxyList[1].printGalaxy();
	cout << "Stellar mass: " << galaxyList[1].stellarMass() << endl;
	cout << endl << endl;
    
    // add satellites to one of the galaxies
    galaxyList[2].setSatellite( galaxy() );
    galaxyList[2].setSatellite( galaxy("Irr",9.8,1e12,0.05) );
    
    // demonstrate satellites
	cout << "Adding some satellites to a galaxy:" << endl;
    for (it = galaxyList.begin(); it < galaxyList.end(); it++) {
        it -> printGalaxy();
    }

    return 0;
}
