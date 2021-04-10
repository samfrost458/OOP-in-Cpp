// AC Circuits main user interface
// Sam Frost 9907936

#include "ACcircuit.h" // all other includes contained in ACcircuits.h

using namespace std;

// function to ask user for a numerical input (int or double) inside a given range
template <class T> T askNumber(T lower, T higher) {
    T input;
    bool inputValid(false);
    while (!(cin >> input) || cin.fail() || !inputValid) {
        // check input for validity
        if (lower <= input && input <= higher) {
            inputValid = true;
            return input;
        }
        else {
            // input is invalid, ask again
            cout << "Sorry, please enter a number in the range " << lower << " to " << higher << endl;
            cin.clear(); cin.ignore(10000, '\n');
        }
    }
}

// function to compare user string input to vector of string arguments
string askAorB(vector <string> strVec) {
    string input;
    bool inputValid(false);
    while (!(cin >> input) || cin.fail() || !inputValid) {
        // check input for validity
        for (auto strIt = strVec.begin(); strIt < strVec.end(); ++strIt) {
            if (input == *strIt) {
                inputValid = true;
                return *strIt;
            }
        }
        
        // if code reaches here, input is invalid; tell user the acceptable inputs
        cout << "Sorry, please enter '" << strVec[0];
        for (auto strIt = strVec.begin() + 1; strIt < strVec.end() - 1; ++strIt) cout << "', '" << *strIt;
        cout << "' or '" << strVec[strVec.size() - 1] << "'" << endl;
        
        cin.clear(); cin.ignore(10000, '\n');
    }
}

// function to print out component library
void printLibrary(vector < shared_ptr<component> > &compLibrary) {
    cout << "Component library:" << endl;
    int i(1);
    for (auto compIt = compLibrary.begin(); compIt < compLibrary.end(); ++compIt) {
        cout << i++ << " - ";
        (*compIt)->showInfo(0);
    }
}

// add component menu
void addComponentMenu(vector < shared_ptr<component> > &compLibrary, unique_ptr <circuit> &mainCircuit) {
    bool repeat(true);
    while (repeat) {
        // print out current compLibrary (if not empty)
        if (!compLibrary.empty()) printLibrary(compLibrary);
        
        // menu
        cout << "\nAdd component menu:" << endl
             << "r  - resistor" << endl
             << "c  - capacitor" << endl
             << "i  - inductor" << endl
             << "nr - non-ideal resistor" << endl
             << "nc - non-ideal capacitor" << endl
             << "ni - non-ideal inductor" << endl
             << "x  - finished adding components" << endl;
        
        // ask user for choices
        string choice(askAorB(vector <string> {"r", "c", "i", "nr", "nc", "ni", "x"}));
        
        if (choice == "x") { repeat = false; }
        else cout << "Now enter this component's value in SI units" << endl;
        
        // declare variables to hold values, and set upper and lower limits
        double value, pvalue1, pvalue2;
        const double freq(mainCircuit->getFreq()); // component frequency
        const double rliml(0.1e-3), rlimu(10.0e12); // ideal resistor limits
        const double climl(0.1e-12), climu(2.0e3); // ideal capacitor limits
        const double iliml(0.1e-9), ilimu(10.0); // ideal inductor limits
        const double prliml(0.0), prlimu(10.0); // parasitic resistor limits
        const double pcliml(0.0), pclimu(1.0e-9); // parasitic capacitor limits
        const double piliml(0.0), pilimu(1.0e-6); // parasitic inductor limits
        // getImpedance function already has divide by zero checks, so will treat a parasitic component with value 0 as if it were not present
        
        // ideal resistor
        if (choice == "r") {
            value = askNumber <double>(rliml, rlimu);
            compLibrary.push_back(shared_ptr <component>(new resistor(value, freq)));
        }
        
        // ideal capacitor
        if (choice == "c") {
            value = askNumber <double>(climl, climu);
            compLibrary.push_back(shared_ptr <component>(new capacitor(value, freq)));
        }
        
        // ideal inductor
        if (choice == "i") {
            value = askNumber <double>(iliml, ilimu);
            compLibrary.push_back(shared_ptr <component>(new inductor(value, freq)));
        }
        
        // non-ideal resistor
        if (choice == "nr") {
            value = askNumber <double>(rliml, rlimu);
            
            cout << "Now enter the parasitic capacitance" << endl;
            pvalue1 = askNumber <double>(pcliml, pclimu);
            
            cout << "Now enter the parasitic inductance" << endl;
            pvalue2 = askNumber <double>(piliml, pilimu);
            
            compLibrary.push_back(shared_ptr <component>(new nonIdealResistor(value, pvalue1, pvalue2, freq)));
        }
        
        // non-ideal capacitor
        if (choice == "nc") {
            value = askNumber <double>(climl, climu);
            
            cout << "Now enter the parasitic resistance" << endl;
            pvalue1 = askNumber <double>(prliml, prlimu);
            
            cout << "Now enter the parasitic inductance" << endl;
            pvalue2 = askNumber <double>(piliml, pilimu);
            
            compLibrary.push_back(shared_ptr <component>(new nonIdealCapacitor(value, pvalue1, pvalue2, freq)));
        }
        
        // non-ideal inductor
        if (choice == "ni") {
            value = askNumber <double>(iliml, ilimu);
            
            cout << "Now enter the parasitic resistance" << endl;
            pvalue1 = askNumber <double>(prliml, prlimu);
            
            cout << "Now enter the parasitic capacitance" << endl;
            pvalue2 = askNumber <double>(pcliml, pclimu);
            
            compLibrary.push_back(shared_ptr <component>(new nonIdealInductor(value, pvalue1, pvalue2, freq)));
        }
    }
}

// build circuit menu
void buildCircuitMenu(vector < shared_ptr<component> > &compLibrary, unique_ptr <circuit> &mainCircuit) {
    // print component list first (main menu has already checked if library is empty)
    printLibrary(compLibrary);
    cout << endl;
    
    // first component should use addParallel function only, so treat as a special case
    cout << "Please select the first component by number" << endl;
    int compNumber(askNumber <int> (1, compLibrary.size()));
    mainCircuit->addParallel(compLibrary[compNumber - 1]);
    
    // first component added, show info before menu
    mainCircuit->showInfo(0);
    
    // menu
    bool repeat(true);
    while (repeat) {
        cout << "\nBuild circuit menu:" << endl
             << "s - add a component in series with current branch" << endl
             << "p - add a component in a new branch parallel to the other branches" << endl
             << "f - change frequency of circuit" << endl
             << "a - save circuit so far to component library" << endl
             << "x - finished building circuit (reset)" << endl;
        
        // ask user for choices
        string choice(askAorB(vector <string> {"s", "p", "f", "a", "x"}));
        
        // adding components
        if (choice == "s" || choice == "p") {
            // print component list
            printLibrary(compLibrary);
            cout << endl;
            
            cout << "Please select the component number" << endl;
            int compNumber(askNumber <int> (1, compLibrary.size()));
            if (choice == "s") mainCircuit->addSeries(compLibrary[compNumber - 1]);
            if (choice == "p") mainCircuit->addParallel(compLibrary[compNumber - 1]);
            
            // show updated circuit information
            mainCircuit->showInfo(0);
        }
        
        // changing frequency
        if (choice == "f") {
            cout << "Enter a frequency in Hz" << endl;
            double f(askNumber <double> (1e-6, 170e9));
            mainCircuit->setFreq(f);
            for (auto compIt = compLibrary.begin(); compIt < compLibrary.end(); ++compIt) (*compIt)->setFreq(f);
            mainCircuit->showInfo(0);
        }
        
        // saving circuit
        if (choice == "a") {
            compLibrary.push_back(shared_ptr <component> (new circuit(*mainCircuit))); // using copy constructor so that user can keep working on current circuit
            cout << "Circuit has been saved" << endl;
        }
        
        // finished
        if (choice == "x") repeat = false;
    }
    
    // reset main circuit
    mainCircuit->reset();
    cout << "Circuit has been reset" << endl;
}

// main menu
void mainMenu(vector <shared_ptr <component> > &compLibrary) {
    unique_ptr <circuit> mainCircuit(new circuit());
    bool repeat(true);
    while (repeat) {
        cout << "\nMain menu:" << endl
             << "a - add new component to component library" << endl
             << "l - view component library" << endl
             << "f - change frequency of all circuits" << endl
             << "b - build circuit" << endl
             << "x - quit" << endl;
        
        // ask user for choices
        string choice(askAorB(vector <string> {"a", "l", "f", "b", "x"}));
        
        // add components
        if (choice == "a") addComponentMenu(compLibrary, mainCircuit);
        
        // print library
        if (choice == "l") {
            cout << endl;
            if (!compLibrary.empty()) printLibrary(compLibrary);
            else cout << "Component library is empty, please add some components" << endl;
        }
        
        // change frequency
        if (choice == "f") {
            cout << "Enter a frequency in Hz" << endl;
            double f(askNumber <double> (1e-6, 170e9));
            mainCircuit->setFreq(f);
            for (auto compIt = compLibrary.begin(); compIt < compLibrary.end(); ++compIt) (*compIt)->setFreq(f);
        }
        
        // circuit building
        if (choice == "b") {
            if (!compLibrary.empty()) buildCircuitMenu(compLibrary, mainCircuit); // open build circuit menu if compLibrary is not empty
            else cout << "Sorry, please add some components to the library first" << endl;
        }
        
        // quit
        if (choice == "x") repeat = false;
    }
}

int main() {
    // vector of components as base class (shared) pointers, passed by reference to all functions so they can manipulate its contents
    vector < shared_ptr<component> > compLibrary;
    
    // execute program from main menu
    cout << "Circuit impedance calculator" << endl;
    mainMenu(compLibrary);
    
    // smart pointers are used so garbage collection is automatic
    
    return 0;
}
