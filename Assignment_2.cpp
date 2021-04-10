// Assignment 2
// Program to read in data and calculate mean, std dev. & std error on the mean
// Sam Frost 9907936

#include <iostream>
#include <iomanip>
#include <fstream> // for reading/writing files
#include <string>
#include <cmath> // includes pow(), sqrt()

using namespace std;

// function to calculate mean of values in an array, dataArray, with (numPoints) values
double calculate_mean(int numPoints, double& dataArray) {
    // μ = 1/N ∑x_i
    double mean; // mean
    double sum{ 0 };
    
    for (int i = 0; i < numPoints; i++) {
        sum += *(&dataArray+i);
    }
    mean = sum / numPoints;
    return mean;
}

// function to calculate standard deviation of values in an array, dataArray, with (numPoints) values and mean 'mean'
double calculate_stdDev(int numPoints, double& dataArray, double mean = 0) {
    // σ = sqrt( 1/(N-1) ∑(x_i - μ)^2 )
    double stdDev; // standard deviation
    double sum{ 0 };
    
    if (mean == 0) {
        // if no mean argument given, calculate mean from scratch
        mean = calculate_mean(numPoints, dataArray);
    }
    
    // now that we have numPoints and mean (either specified in argument or calculated), calculate stdDev
    for (int i = 0; i < numPoints; i++) {
        sum += pow( *(&dataArray+i) - mean, 2 );
    }
    stdDev = sqrt( sum / (numPoints - 1) );
    return stdDev;
}

// function to calculate standard error on the mean of values in an array, dataArray, with (numPoints) values and standard deviation 'stdDev'
double calculate_stdErr(int numPoints, double& dataArray, double stdDev = 0) {
    // σ_μ = σ/sqrt(N)
    double stdErr; // standard error on mean
    
    if (stdDev == 0) {
        // no stdDev argument given, calculate stdDev from scratch
        stdDev = calculate_stdDev(numPoints, dataArray);
    }
    
    // now that we have numPoints and stdDev (either specified in argument or calculated), calculate stdErr
    stdErr = stdDev / sqrt(numPoints);
    return stdErr;
}

// function to read in a file, find the number of lines that can be converted to double, change the argument numPoints to this number and print to an array if wanted
void readFileToDouble(string inFileName, int& numPoints, double& dataArray, bool readToArray = true) {
    // open 'inFileName' and call the filestream 'dataFile'
    ifstream dataFile(inFileName);
    
    // check if file state is good
    if (!dataFile.good()) {
        // file input failed, print error message and exit
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    
    // file successfully opened, read in line by line (as string) until end of file
    string line; // variable to hold each file line
    double dataPoint; // variable to hold 'line' converted to double
    int lineCount{ 0 }; // variable to keep track of which line in the file is being read
    int successes{ 0 }; // variable to keep track of how many lines were successfully read in
    
    while (!dataFile.eof()) {
        bool error = false; // error == true if line fails to convert to double
        
        // get a line in the file and copy it to string 'line'
        getline(dataFile, line);
        
        // attempt to convert string 'line' to double with stod()
        // if stod() fails, it throws an invalid_argument
        try {
            dataPoint = stod(line);
        }
        catch (invalid_argument) {
            // error in converting string 'line' to double
            error = true;
            if (readToArray) {
                // only display error message if trying to read to a file to the dataArray
                cerr << "Error: cannot convert line " << lineCount + 1 << " of "
                << inFileName << " (\"" << line << "\") to a double" << endl;
            }
        }
        
        if (!error) {
            // conversion worked, put dataPoint into dataArray
            successes++;
            if (readToArray) {
                *(&dataArray+successes-1) = dataPoint;
            }
        }
        lineCount++;
    }
    // reached end of file, close file
    dataFile.close();
    numPoints = successes; // changes numPoints outside the function, since it is passed by reference
}


int main() {
    // declare variables
    int numPoints;
    double mean, stdDev, stdErr;
    bool readToArray;
    string inFileName;
    
    // ask user for a file name, assuming file is in same directory as code
    cout << "Please enter a file name" << endl;
    cin >> inFileName;
    
    // read in file initially to find number of data points, 'numPoints'
    double* temp = new double[]; // declared temporary array to satisfy readFileToDouble arguments while finding numPoints
    readFileToDouble(inFileName, numPoints, *temp, readToArray = false);
    delete[] temp; // release memory held by temp
    
    // declare a pointer of (numPoints) doubles called 'dataArray' and allocate memory
    double* dataArray = new double[numPoints];
    
    // read in file to dataArray
    readFileToDouble(inFileName, numPoints, *dataArray, readToArray = true);
    
    // dataArray now contains all the data, calculate mean, std dev, std err
    mean   = calculate_mean  (numPoints, *dataArray);
    stdDev = calculate_stdDev(numPoints, *dataArray, mean);
    stdErr = calculate_stdErr(numPoints, *dataArray, stdDev);
    
    // finished using dataArray, free up memory
    delete[] dataArray;
    
    // print calculated values to console
    cout << "Number of data points: " << numPoints << endl;
    cout << setprecision(5) << fixed; // fix cout to 5 decimal places
    cout << "Mean  : " << mean << endl
    << "stdDev: " << stdDev << endl
    << "stdErr: " << stdErr << endl;
    
    return 0;
}



