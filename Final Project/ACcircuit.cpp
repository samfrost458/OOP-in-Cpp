// AC Circuits function definitions
// Sam Frost 9907936

#include "ACcircuit.h"
// all other includes contained in ACcircuits.h

using namespace std;

// circuit copy constructor
circuit::circuit(circuit &oldCircuit) {
    freq = oldCircuit.freq;
    componentlist = oldCircuit.componentlist;
    impedance = oldCircuit.impedance;
}

// functions to set the frequency of a component or circuit
void resistor::setFreq(double f) { freq = f; }
void capacitor::setFreq(double f) {
    freq = f;
    impedance = -complex <double> (0.0, 1.0) / (2.0 * M_PI * freq * capacitance); // recalculate impedance
}
void inductor::setFreq(double f) {
    freq = f;
    impedance = complex <double> (0.0, 1.0) * 2.0 * M_PI * freq * inductance; // recalculate impedance
}
void circuit::setFreq(double f) {
    freq = f;
    // change frequency for all components in circuit
    vector < vector < shared_ptr<component> > >::iterator branchIt;
    for (branchIt = componentlist.begin(); branchIt < componentlist.end(); ++branchIt) {
        vector < shared_ptr<component> >::iterator compIt;
        for (compIt = branchIt->begin(); compIt < branchIt->end(); ++compIt) {
            (*compIt)->setFreq(f);
        }
    }
    getImpedance(); // recalculate impedance
}

// functions to return the frequency of a component or circuit
double component::getFreq() { return freq; }

// functions to return the impedance of a component or circuit
complex <double> component::getImpedance() { return impedance; }
complex <double> circuit::getImpedance() {
    // find total impedance of all parallel branches
    complex <double> inverseImped(0.0, 0.0);
    vector < vector < shared_ptr<component> > >::iterator branchIt;
    for (branchIt = componentlist.begin(); branchIt < componentlist.end(); ++branchIt) {
        // find (series) impedance of each branch
        complex <double> seriesImped(0.0, 0.0);
        vector < shared_ptr<component> >::iterator compIt;
        for (compIt = branchIt->begin(); compIt < branchIt->end(); ++compIt) {
            // check if impedance magnitude is infinity before calculation (i.e. ignore components with 0 value in sum)
            if (!isinf((*compIt)->getImpedMag())) seriesImped += (*compIt)->getImpedance();
        }
        // check series impedance is not 0 before calculation (i.e. ignore empty branches in sum)
        if (seriesImped != 0.0) inverseImped += 1.0 / seriesImped;
    }
    
    // set impedance to new calculated value (if circuit is empty impedance remains at zero as it was initialised)
    if (inverseImped != (0.0, 0.0)) impedance = 1.0 / inverseImped;
    return impedance;
}

// functions to return the magnitude of the impedance
double component::getImpedMag() { return abs(impedance); }

// functions to return the phase difference
double component::getPhase() { return arg(impedance); }

// functions to print the information of a component or circuit, indented wih a number of half-tabs (4 spaces)
string numTabs(int indent) {
    string output;
    for (int i(0); i < indent; ++i) output += "    ";
    return output;
}
void resistor::showInfo(int indent) {
    cout << resistance << " Ohm resistor (" << getFreq() << " Hz)" << endl
         << numTabs(indent) << "    -> " << getImpedMag() << " Ohm impedance magnitude" << endl
         << numTabs(indent) << "    -> " << getPhase() << " rad phase" << endl;
}
void capacitor::showInfo(int indent) {
    cout << capacitance << " F capacitor (" << getFreq() << " Hz)" << endl
         << numTabs(indent) << "    -> " << getImpedMag() << " Ohm impedance magnitude" << endl
         << numTabs(indent) << "    -> " << getPhase() << " rad phase" << endl;
}
void inductor::showInfo(int indent) {
    cout << inductance << " H inductor (" << getFreq() << " Hz)" << endl
         << numTabs(indent) << "    -> " << getImpedMag() << " Ohm impedance magnitude" << endl
         << numTabs(indent) << "    -> " << getPhase() << " rad phase" << endl;
}
void circuit::showInfo(int indent) {
    // all component frequencies and impedances should have been updated as the circuit was built, so no need to recalculate here
    cout << "Circuit: (" << getFreq() << " Hz)" << endl
         << numTabs(indent) << "    -> " << getImpedMag() << " Ohm impedance magnitude" << endl
         << numTabs(indent) << "    -> " << getPhase() << " rad phase" << endl;
    
    // print all component information
    cout << numTabs(indent + 1);
    if (indent == 0) cout << "*"; // mark out start of main circuit with star (not for nested circuits)
    cout << "--------" << endl; // mark where every parallel branch starts
    
    vector < vector < shared_ptr<component> > >::iterator branchIt;
    for (branchIt = componentlist.begin(); branchIt < componentlist.end(); ++branchIt) {
        vector < shared_ptr<component> >::iterator compIt;
        for (compIt = branchIt->begin(); compIt < branchIt->end(); ++compIt) {
            cout << numTabs(indent + 1);
            cout << " - ";
            (*compIt)->showInfo(indent + 1);
        }
        cout << numTabs(indent + 1);
        if (indent == 0 && branchIt == componentlist.end() - 1) cout << "*"; // mark out very end of main circuit with a star
        cout << "--------" << endl; // mark where every parallel branch ends
    }
}
void nonIdealResistor::showInfo(int indent) {
    cout << resistance << " Ohm non-ideal resistor (" << getFreq() << " Hz)" << endl
         << numTabs(indent) << "    -> " << getImpedMag() << " Ohm impedance magnitude" << endl
         << numTabs(indent) << "    -> " << getPhase() << " rad phase" << endl
         << numTabs(indent) << "    -> " << pCapacitance << " F parasitic capacitance" << endl
         << numTabs(indent) << "    -> " << pInductance << " H parasitic inductance" << endl;
}
void nonIdealCapacitor::showInfo(int indent) {
    cout << capacitance << " F non-ideal capacitor (" << getFreq() << " Hz)" << endl
         << numTabs(indent) << "    -> " << getImpedMag() << " Ohm impedance magnitude" << endl
         << numTabs(indent) << "    -> " << getPhase() << " rad phase" << endl
         << numTabs(indent) << "    -> " << pResistance << " Ohm parasitic resistance" << endl
         << numTabs(indent) << "    -> " << pInductance << " H parasitic inductance" << endl;
}
void nonIdealInductor::showInfo(int indent) {
    cout << inductance << " H non-ideal inductor (" << getFreq() << " Hz)" << endl
         << numTabs(indent) << "    -> " << getImpedMag() << " Ohm impedance magnitude" << endl
         << numTabs(indent) << "    -> " << getPhase() << " rad phase" << endl
         << numTabs(indent) << "    -> " << pResistance << " Ohm parasitic resistance" << endl
         << numTabs(indent) << "    -> " << pCapacitance << " F parasitic capacitance" << endl;
}

// functions to get the pure resistances, capacitances or inductances of components
double resistor::getResistance() { return resistance; }
double capacitor::getCapacitance() { return capacitance; }
double inductor::getInductance() { return inductance; }

// functions to set the resistances, capacitances or inductances of components
void resistor::setResistance(double R) { resistance = R; impedance = getImpedance(); }
void capacitor::setCapacitance(double C) { capacitance = C; impedance = getImpedance(); }
void inductor::setInductance(double L) { inductance = L; impedance = getImpedance(); }

// functions to return string of component type
string resistor::componentType() { return "resistor"; }
string capacitor::componentType() { return "capacitor"; }
string inductor::componentType() { return "inductor"; }
string circuit::componentType() { return "circuit"; }
string nonIdealResistor::componentType() { return "nonIdealResistor"; }
string nonIdealCapacitor::componentType() { return "nonIdealCapacitor"; }
string nonIdealInductor::componentType() { return "nonIdealInductor"; }

// functions to add compenents to a circuit
void circuit::addSeries(shared_ptr <component> comp) {
    if (componentlist.empty()) addParallel(comp); // if component list is empty, addParallel will add a branch first
    else componentlist[componentlist.size() - 1].push_back(comp); // add in series with latest branch
    getImpedance(); // recalculate circuit impedance
}
void circuit::addParallel(shared_ptr <component> comp) {
    // declaring new parallel branch vector
    vector < shared_ptr<component> > newBranch;
    // add the component
    componentlist.push_back(newBranch);
    addSeries(comp);
}

// function to clear component list and reset impedance to default without calling destructor
void circuit::reset() {
    // clear internal component list (without deleting pointers from external component list)
    componentlist.clear();
    
    // reset impedance (but remember frequency)
    impedance = (0.0, 0.0);
}
