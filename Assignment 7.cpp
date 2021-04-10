// Assignment 7
// Vector classes
// Sam Frost 9907936

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

// vector class
class vector {
protected:
    int size;
    double* arr;
    
public:
    // default constructor
    vector() : size(0), arr(nullptr) { }
    // parameterised constructor 1
    vector(int n) {
        size = n; arr = new double[n];
        // set all elements to 0 by default
        for (int i(0); i < n; i++) arr[i] = 0;
    }
    // parameterised constructor 2 (for 3-vector)
    vector(double x, double y, double z) {
        size = 3; arr = new double[3];
        arr[0] = x; arr[1] = y; arr[2] = z;
    }
    // parameterised constructor 3 (for 4-vector)
    vector(double ct, double x, double y, double z) {
        size = 4; arr = new double[4];
        arr[0] = ct; arr[1] = x; arr[2] = y; arr[3] = z;
    }
    
    // destructor
    ~vector() { if (arr != nullptr && sizeof(arr)/sizeof(arr[0]) != 0) delete[] arr; } // if statement to make sure not trying to delete something that has already been deleted
    
    // copying and moving
    vector(const vector &A); // copy constructor
    vector & operator = (const vector &A); // copy assignment operator
    vector(vector &&A); // move constructor
    vector & operator = (vector &&A); // move assignment operator
    
    // declare functions
    void setSize(int n);
    int getSize();
    void setElement(int i, double x);
    double operator [] (int i);
    double modulus();
    double dotProduct(const vector &b);
    friend ostream & operator << (ostream &os, const vector &A);
};

// 4-vector class
class vector4 : public vector {
public:
    // default constructor
    vector4() : vector(4) {}
    // parameterised constructor 1
    vector4(double ct, double x, double y, double z) : vector(ct, x, y, z) {}
    // parameterised constructor 2
    vector4(double ct, vector r) : vector(ct, r[0], r[1], r[2]) {}
    // parametised constructor 3
    vector4(vector r) : vector(r[0], r[1], r[2], r[3]) {}
    
    // destructor
    ~vector4() { delete[] arr; }
    
    // copy and move constructors and assignment operators, accessors and other functions are inherited from vector class
    
    // declare functions
    void setSize(int n); // overriding the setSize function to make 4-vectors unable to change size
    double dotProduct(vector4 &b); // overridden function
    double gamma(vector &beta);
    vector4 lorentzBoost(vector &beta);
};

// particle class
class particle {
protected:
    vector4 position;
    double mass; // in natural energy units
    vector beta;
    
public:
    // default constructor
    particle() : position(), mass(0), beta() {}
    // parameterised constructor
    particle(vector4 r, double m, vector b) : position(r), mass(m), beta(b) {
        if (beta.modulus() >= 1 || beta.modulus() < 0) { cerr << "Error: beta modulus out of range, set to zero by default" << endl; beta = vector(3);}
        if (beta.getSize() != 3) { cerr << "Error: beta is not a 3-vector, set to zero by default"; beta = vector(3); }
    }
    // destructor
    ~particle() {}
    
    // declare functions
    void setPosition(double ct, double x, double y, double z);
    void setMass(double m);
    void setBeta(double vx, double vy, double vz);
    double gamma(); // overloaded function
    double energy();
    vector momentum();
    friend ostream & operator << (ostream &os, const particle &p);
};

// defining copy constructor for vector
vector::vector(const vector &A) {
    arr = 0; size = A.size;
    if (size > 0) {
        arr = new double[size];
        for (int i(0); i < size; i++) arr[i] = A.arr[i];
    }
    //cout << "Copy constructor called" << endl;
}

// defining assignment operator for vector
vector & vector::operator = (const vector &A) {
    if (&A == this) return *this; // self assignment check
    delete[] arr; arr = 0; size = 0; size = A.size;
    if (size > 0) {
        arr = new double[size];
        for (int i(0); i < size; i++) arr[i] = A.arr[i];
    }
    //cout << "Copy assignment operator called" << endl;
    return *this;
}

// defining move constructor for vector
vector::vector(vector &&A) {
    size = A.size;
    arr = A.arr;
    A.size = 0; A.arr = nullptr;
    //cout << "Move constructor called" << endl;
}

// defining move assignment operator for vector
vector & vector::operator = (vector &&A) {
    swap(size, A.size);
    swap(arr, A.arr);
    //cout << "Move assignment operator called" << endl;
    return *this;
}

// function to set the size of a vector
void vector::setSize(int n) {
    vector newVector(n);
    if (n > size) for (int i(0); i < size; i++) newVector.setElement(i, arr[i]);
    if (n < size) for (int i(0); i < n; i++) newVector.setElement(i, arr[i]);
    swap(size, newVector.size); swap(arr, newVector.arr);
}

// function to return the size of a vector
int vector::getSize() { return size; }

// function to set an element of a vector
void vector::setElement(int i, double x) {
    if (i >= 0 && i < size) arr[i] = x;
    else cerr << "Error: element " << i << " is out of range" << endl;
}

// function to overload [] operator (for general vector)
double vector::operator [] (int i) {
    if (i >= 0 && i < size) return arr[i];
    else { cerr << "Error: trying to access element out of range" << endl; /*return 0;*/ exit(1); }
}

// function to find the modulus of a vector
double vector::modulus() {
    double modSq(0);
    for (int i(0); i < size; i++) modSq += pow(arr[i], 2.0);
    return sqrt(modSq);
}

// function to calculate dot product for general vector
double vector::dotProduct(const vector &b) {
    double product(0);
    if (size == b.size) {
        for (int i(0); i < size; i++) product += arr[i] * b.arr[i];
    }
    else { cerr << "Error: cannot find dot product due to differing vector sizes" << endl; exit(1); }
    return product;
}

// friend function to overload insertion operator for a vector
ostream & operator << (ostream &os, const vector &A) {
    os << "[";
    for (int i(0); i < A.size; i++) {
        if (i != 0) os << ", ";
        os << A.arr[i];
    }
    os << "]";
    return os;
}

// overridden function to not allow resizing of a 4-vector
void vector4::setSize(int n) { cerr << "Error: 4-vector must have size 4" << endl; }


// overridden function to calculate 4-vector dot product
double vector4::dotProduct(vector4 &b) {
    double product(0);
    if (size == b.size && size == 4) {
        product = arr[0] * b[0];
        for (int i(1); i < size; i++) product -= arr[i] * b[i];
    }
    else { cerr << "Error: cannot find dot product due to differing vector sizes" << endl; exit(1); }
    return product;
}

// function to find gamma factor of a velocity 3-vector
double vector4::gamma(vector &beta) {
    if (beta.getSize() == 3) {
        if (beta.modulus() < 1) return 1 / sqrt(1 - pow(beta.modulus(), 2.0));
        else { cerr << "Error: modulus of beta is greater than 1" << endl; /*return 0;*/ exit(1); }
    }
    else { cerr << "Error: gamma function in 4-vector class requires a 3-vector argument" << endl; /*return 0;*/ exit(1); }
}

// function to return Lorentz-boosted 4-vector
vector4 vector4::lorentzBoost(vector &beta) {
    vector position3(arr[1], arr[2], arr[3]); // when calculating dot products here, only referring to regular dot product, so use regular 3-position
    if (beta.getSize() == 3) {
        double additionFactor((gamma(beta) - 1)*position3.dotProduct(beta) / (pow(beta.modulus(), 2.0)) - gamma(beta)*arr[0]);
        
        vector4 boosted;
        boosted.setElement(0, gamma(beta) * (arr[0] - position3.dotProduct(beta)));
        for (int i(1); i < 4; i++) boosted.setElement(i, arr[i] + additionFactor * beta[i - 1]);
        return boosted;
    }
    else { cerr << "Error: Lorentz boost function requires 3-vector argument" << endl; /*return vector4();*/ exit(1); }
}

// function to set the position of a particle
void particle::setPosition(double ct, double x, double y, double z) { position = vector4(ct, x, y, z); }

// function to set the mass of a particle
void particle::setMass(double m) { mass = m; }

// function to set the velocity of a particle
void particle::setBeta(double vx, double vy, double vz) {
    vector temp(vx, vy, vz);
    if (temp.modulus() < 1) beta = temp;
    else cerr << "Error: modulus of beta cannot be greater than 1" << endl;
}

// overloaded gamma function within particle class
double particle::gamma() {
    if (beta.modulus() < 1) return 1 / sqrt(1 - pow(beta.modulus(), 2.0));
    else { cerr << "Error: modulus of beta is greater than 1" << endl; /*return 0;*/ exit(1); }
}

// function to return energy of a particle, gamma*m*c^2, in natural units
double particle::energy() { return gamma() * mass; }

// function to return 4-momentum of a particle (E/c, gamma*m*v) in natural units
vector particle::momentum() {
    vector momentum(3);
    for (int i(0); i < 3; i++) momentum.setElement(i, gamma() * mass * beta[i]);
    return momentum;
}

// friend function to overload insertion operator for a particle
ostream & operator << (ostream &os, const particle &p) {
    os << "4-position: " << p.position << endl
    << "Masss:      " << p.mass << endl
    << "Velocity:   " << p.beta << endl;
    return os;
}


int main() {
    // demonstrating base vector class
    cout << "\nGeneral vector of length 0 using default constructor" << endl;
    vector example1;
    cout << "example1 = " << example1 << endl;
    cout << "General vector of length 6 using paramerised constructor" << endl;
    vector example2(6);
    cout << "example2 = " << example2 << endl;
    cout << "Setting some of the elements in this vector to something else" << endl;
    example2.setElement(1, 3); example2.setElement(2, 4);
    cout << "example2 = " << example2 << endl;
    cout << "Finding particular elements using accessors" << endl;
    cout << "example2[1] = " << example2[1] << endl;
    cout << "Changing the size of this vector" << endl;
    example2.setSize(10);
    cout << "example2 = " << example2 << endl;
    cout << "Finding modulus of this vector" << endl;
    cout << "Modulus = " << example2.modulus() << endl;
    cout << "Creating a new vector using copy constuctor" << endl;
    vector example3 = example2;
    cout << "example3 = " << example3 << endl;
    cout << "Taking the dot product of the two" << endl;
    cout << "example2.example3 = " << example2.dotProduct(example3) << endl;
    
    // this should call an exit when running
    /*cout << "\nAttempting to access an element out of range" << endl;
     cout << "example2[-1] = " << example2[-1] << endl;
     cout << "Attempting to take a dot product of example1 and example2" << endl;
     cout << "example1.example2 = " << example1.dotProduct(example2) << endl;*/
    
    // demonstrating copy and move constructors and assignment operators of the base class
    /*cout << "\nCreating a new vector from example2 with move constructor" << endl;
     vector example4(move(example2));
     cout << "example2 = " << example2 << endl;
     cout << "example4 = " << example4 << endl;
     cout << "Creating a new vector from example4 using copy constructor" << endl;
     vector example5(example4);
     cout << "example4 = " << example4 << endl;
     cout << "example5 = " << example5 << endl;
     cout << "Moving example4 to example 1 using move assignment operator" << endl;
     example1 = move(example4);
     cout << "example4 = " << example4 << endl;
     cout << "example1 = " << example1 << endl;
     cout << "Copying example1 to example4 using copy assignment operator" << endl;
     example4 = example1;
     cout << "example4 = " << example4 << endl;
     cout << "example1 = " << example1 << endl;*/
    
    // demonstrating derived vector4 class
    cout << "\nCreating a blank 4-vector with default constructor" << endl;
    vector4 example41;
    cout << "example41 = " << example41 << endl;
    cout << "Setting some of the elements in this vector to something else" << endl;
    example41.setElement(1, 3); example41.setElement(2, 4);
    cout << "example41 = " << example41 << endl;
    cout << "Finding particular elements using accessors" << endl;
    cout << "example41[1] = " << example41[1] << endl;
    /*cout << "Attempting to access an element out of range" << endl;
     cout << "example41[-1] = " << example41[-1] << endl;*/
    
    cout << "\nCreating a 4-vector with first parameterised constructor" << endl;
    vector4 example42(0.1, 2, 3, 4);
    cout << "example42 = " << example42 << endl;
    cout << "Creating a 4-vector with second parameterised constructor" << endl;
    vector vec3(6, 7, 8);
    cout << "vec3 = " << vec3 << endl;
    vector4 example43(0.5, vec3);
    cout << "example43 = " << example43 << endl;
    cout << "Creating a velocity vector" << endl;
    vector beta(0.9, 0, 0);
    cout << "beta = " << beta << endl;
    
    cout << "\nFinding dot product of the 4-vectors" << endl;
    cout << "example42 = " << example42 << endl;
    cout << "example43 = " << example43 << endl;
    cout << "example42.example43 = " << example42.dotProduct(example43) << endl;
    cout << "Finding example42 Lorentz boosted by beta" << endl;
    cout << "example42.lorentzBoost(beta) = " << example42.lorentzBoost(beta) << endl;
    
    /*cout << "\nTrying to change a 4-vector size" << endl;
     cout << "example41 = " << example41 << endl;
     cout << "Setting example41 (a 4-vector) to size 10" << endl;
     example41.setSize(10);
     cout << "example41 = " << example41 << endl;*/
    
    // demonstrating (inherited) copy and move constructors and assignment operators for derived vector4 class
    cout << "\nCopy and move constructors and assignment operators etc are inherited from vector class" << endl;
    cout << "example42 = " << example42 << endl;
    cout << "Creating a new 4-vector from example42 using copy constuctor" << endl;
    vector example44(example42);
    cout << "example42 = " << example42 << endl;
    cout << "example44 = " << example44 << endl;
    cout << "Creating a new 4-vector from example42 with copy assignment operator" << endl;
    vector example45 = example42;
    cout << "example42 = " << example42 << endl;
    cout << "example45 = " << example45 << endl;
    cout << "Creating a new vector from example42 using move constructor" << endl;
    vector example46(move(example42));
    cout << "example42 = " << example42 << endl;
    cout << "example46 = " << example46 << endl;
    cout << "Moving example46 back to example42 using move assignment operator" << endl;
    example42 = move(example46);
    cout << "example46 = " << example46 << endl;
    cout << "example42 = " << example42 << endl;
    
    // demonstrating particle class
    cout << "\nCreating a particle with position " << example42 << " and velocity " << beta << endl;
    particle particle1(example42, 10, beta);
    cout << particle1
    << "Gamma:      " << particle1.gamma() << endl
    << "Energy:     " << particle1.energy() << endl
    << "Momentum:   " << particle1.momentum() << endl;
    
    cout << "\nChanging properties of this particle" << endl;
    particle1.setPosition(5, 4, 2, 0);
    particle1.setMass(10);
    particle1.setBeta(0.75, 0, 0);
    cout << particle1
    << "Gamma:      " << particle1.gamma() << endl
    << "Energy:     " << particle1.energy() << endl
    << "Momentum:   " << particle1.momentum() << endl;
    
    return 0;
}
