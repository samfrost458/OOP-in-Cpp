// AC Circuits header file
// Sam Frost 9907936

#define _USE_MATH_DEFINES // contains pi as M_PI, when including math.h
#include <math.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <memory> // for shared (smart) pointers, to avoid manual garbage collection
#include <cmath>
#include <complex> // to store and manipulate impedances as complex numbers

#ifndef ACclass
#define ACclass

using namespace std;

// (abstract) base component class
class component {
protected:
    complex <double> impedance;
    double freq;
public:
    // default constructor
    component() : impedance(0.0, 0.0), freq(50.0) {} // mains 50 Hz default
    // parameterised constructors
    component(double f) : impedance(0.0, 0.0), freq(f) {}
    component(complex <double> Z) : impedance(Z), freq(50.0) {} // mains 50 Hz default
    component(complex <double> Z, double f) : impedance(Z), freq(f) {}
    
    // destructor
    virtual ~component() {}
    
    // declare (virtual) functions (all non-virtual functions have the same action, so can be inherited instead of overridden)
    virtual void setFreq(double f) = 0;
    virtual double getFreq();
    virtual complex <double> getImpedance(); // resistors, capacitors and inductors can inherit this, but circuits override this function
    virtual double getImpedMag();
    virtual double getPhase();
    virtual void showInfo(int indent) = 0; // function to show component information, printed with a specified indent
    virtual string componentType() = 0; // function to return a string stating the component type
};

// resistor class
class resistor : public component {
protected:
    double resistance;
public:
    // default constructor
    resistor() : component(), resistance(0.0) {}
    // parameterised constructor
    resistor(double R) : component(complex <double>(R, 0.0)), resistance(R) {}
    resistor(double R, double f) : component(complex <double>(R, 0.0), f), resistance(R) {}
    
    // destructor
    ~resistor() { cout << resistance << " Ohm resistor destructor" << endl; }
    
    // functions
    void setFreq(double f);
    void showInfo(int indent);
    double getResistance();
    void setResistance(double R);
    string componentType();
};

// capacitor class
class capacitor : public component {
protected:
    double capacitance;
public:
    // default constructor
    capacitor() : component(), capacitance(0.0) {}
    // parameterised constructors
    capacitor(double C) : component(-complex <double>(0.0, 1.0) / (2.0 * M_PI * 50.0 * C)), capacitance(C) {} // mains 50 Hz default
    capacitor(double C, double f) : component(-complex <double>(0.0, 1.0) / (2.0 * M_PI * f * C), f), capacitance(C) {}
    
    // destructor
    ~capacitor() { cout << capacitance << " F capacitor destructor" << endl; }
    
    // functions
    void setFreq(double f);
    void showInfo(int indent);
    double getCapacitance();
    void setCapacitance(double C);
    string componentType();
};

// inductor class
class inductor : public component {
protected:
    double inductance;
public:
    // default constructor
    inductor() : component(), inductance(0.0) {}
    // parameterised constructors
    inductor(double L) : component(complex <double>(0.0, 1.0) * 2.0 * M_PI * 50.0 * L), inductance(L) {} // mains 50 Hz default
    inductor(double L, double f) : component(complex <double>(0.0, 1.0) * 2.0 * M_PI * f * L, f), inductance(L) {}
    
    // destructor
    ~inductor() { cout << inductance << " H inductor destructor" << endl; }
    
    // functions
    void setFreq(double f);
    void showInfo(int indent);
    double getInductance();
    void setInductance(double L);
    string componentType();
};

// generic circuit class (inherits from component class so that circuits can themselves be used as components and nested into larger circuits)
class circuit : public component {
protected:
    vector < vector < shared_ptr<component> > > componentlist;
public:
    // default constructor
    circuit() : component() {} // mains 50 Hz default included in component constructor
    // parameterised constructors
    circuit(double f) : component(f) {}
    circuit(double f, vector < vector < shared_ptr<component> > > extCompList) : component(f), componentlist(extCompList) {}
    
    // destructor
    ~circuit() {
        cout << getImpedMag() << " Ohm, " << getPhase() << " rad, " << getFreq() << " Hz circuit destructor" << endl;
        componentlist.clear();
    }
    
    // copy constructor
    circuit(circuit &newCircuit);
    
    // functions
    void setFreq(double f);
    complex <double> getImpedance();
    void showInfo(int indent);
    string componentType();
    void addSeries(shared_ptr <component> comp);
    void addParallel(shared_ptr <component> comp);
    void reset(); // function to clear component list and reset impedance to default without calling destructor (does not clear frequency)
};

// non-ideal resistor class
class nonIdealResistor : public circuit {
protected:
    double resistance, pCapacitance, pInductance;
public:
    // default constructor
    nonIdealResistor() : circuit() {
        addParallel(shared_ptr <component> (new resistor()));
        addSeries(shared_ptr <component> (new inductor()));
        addParallel(shared_ptr <component> (new capacitor()));
    }
    // parameterised constructor
    nonIdealResistor(double R, double C, double L, double f) : circuit(f), resistance(R), pCapacitance(C), pInductance(L) {
        addParallel(shared_ptr <component> (new resistor(R, f)));
        addSeries(shared_ptr <component> (new inductor(L, f)));
        addParallel(shared_ptr <component> (new capacitor(C, f)));
        impedance = getImpedance();
    }
    
    // destructor
    ~nonIdealResistor() {
        cout << resistance << " Ohm non-ideal resistor destructor" << endl;
        componentlist.clear();
    }
    
    // functions
    void showInfo(int indent);
    string componentType();
};

// non-ideal capacitor class
class nonIdealCapacitor : public circuit {
protected:
    double capacitance, pResistance, pInductance;
public:
    // default constructor
    nonIdealCapacitor() : circuit() {
        addParallel(shared_ptr <component> (new resistor()));
        addSeries(shared_ptr <component> (new inductor()));
        addSeries(shared_ptr <component> (new capacitor()));
    }
    // parameterised constructor
    nonIdealCapacitor(double C, double R, double L, double f) : circuit(f), capacitance(C), pResistance(R), pInductance(L) {
        addParallel(shared_ptr <component> (new resistor(R, f)));
        addSeries(shared_ptr <component> (new inductor(L, f)));
        addSeries(shared_ptr <component> (new capacitor(C, f)));
        impedance = getImpedance();
    }
    
    // destructor
    ~nonIdealCapacitor() {
        cout << capacitance << " F non-ideal capacitor destructor" << endl;
        componentlist.clear();
    }
    
    // functions
    void showInfo(int indent);
    string componentType();
};

// non-ideal inductor class
class nonIdealInductor : public circuit {
protected:
    double inductance, pResistance, pCapacitance;
public:
    // default constructor
    nonIdealInductor() : circuit() {
        addParallel(shared_ptr <component> (new inductor()));
        addSeries(shared_ptr <component> (new resistor()));
        addParallel(shared_ptr <component> (new capacitor()));
    }
    // parameterised constructor
    nonIdealInductor(double L, double R, double C, double f) : circuit(f), inductance(L), pResistance(R), pCapacitance(C) {
        addParallel(shared_ptr <component> (new inductor(L, f)));
        addSeries(shared_ptr <component> (new resistor(R, f)));
        addParallel(shared_ptr <component> (new capacitor(C, f)));
        impedance = getImpedance();
    }
    
    // destructor
    ~nonIdealInductor() {
        cout << inductance << " H non-ideal inductor destructor" << endl;
        componentlist.clear();
    }
    
    // functions
    void showInfo(int indent);
    string componentType();
};

#endif
