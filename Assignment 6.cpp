// Assignment 6
// Matrix class
// Sam Frost 9907936

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class matrix {
private:
    int rows;
    int columns;
    double* dataArray;
    
public:
    // default constructor
    matrix() {
        rows = 0; columns = 0; dataArray = nullptr;
        dataArray = 0;
        //cout << "Default constructor called" << endl;
    }
    // parametized constructor
    matrix(int m, int n) {
        rows = m; columns = n; dataArray = new double[m*n];
        // set all elements to 0 by default
        for (int i(0); i < m*n; i++) {
            dataArray[i] = 0;
        }
    }
    
    // destructor
    ~matrix() { delete[] dataArray; /*cout << "Destructor called" << endl;*/ }
    
    // deep copying
    matrix(const matrix &A); // copy constructor
    matrix & operator = (const matrix &A); // copy assignment operator
    // moving
    matrix(matrix &&A); // move constructor
    matrix & operator = (matrix &&A); // move assignment operator
    
    // declare functions
    int loc(int i, int j);
    bool matrixIsBlank();
    void setRows(int m);
    void setColumns(int n);
    void setElement(int i, int j, double x);
    double getRows();
    double getColumns();
    double getElement(int i, int j);
    matrix minor(int i, int j);
    double determinant();
    friend ostream & operator << (ostream &os, const matrix &A);
    friend istream & operator >> (istream &is, matrix &A);
    
    // overloaded operator declarations
    matrix operator + (const matrix &A);
    matrix operator - (const matrix &A);
    matrix operator * (const matrix &A);
};

// defining copy constructor for deep copy
matrix::matrix(const matrix &A) {
    // declare dataArray and copy rows and columns
    dataArray = 0;
    rows = A.rows; columns = A.columns;
    if (rows > 0 && columns > 0) {
        dataArray = new double[rows*columns];
        // copy values into new array
        for (int i(0); i < rows*columns; i++) dataArray[i] = A.dataArray[i];
    }
    cout << "Copy constructor called" << endl;
}

// defining assignment operator for deep copy
matrix & matrix::operator = (const matrix &A) {
    if (&A == this) return *this; // self assignment check
    // first delete this object's array
    delete[] dataArray; dataArray = 0; rows = 0; columns = 0;
    // now copy rows and columns and declare new array
    rows = A.rows; columns = A.columns;
    if (rows > 0 && columns > 0) {
        dataArray = new double[rows*columns];
        // copy values into new array
        for (int i(0); i < rows*columns; i++) dataArray[i] = A.dataArray[i];
    }
    cout << "Copy assignment operator called" << endl;
    return *this;
}

// defining move constructor
matrix::matrix(matrix &&A) {
    rows = A.rows;
    columns = A.columns;
    dataArray = A.dataArray;
    A.rows = 0; A.columns = 0; A.dataArray = nullptr;
    cout << "Move constructor called" << endl;
}

// defining move assignment operator
matrix & matrix::operator = (matrix &&A) {
    swap(rows, A.rows);
    swap(columns, A.columns);
    swap(dataArray, A.dataArray);
    cout << "Move assignment operator called" << endl;
    return *this;
}

// function to return raw index of a matrix element
int matrix::loc(int i, int j) { return (j - 1) + (i - 1)*rows; }

// function to check if matrix is empty
bool matrix::matrixIsBlank() {
    bool zero(true);
    for (int i(0); i < rows*columns; i++) {
        if (dataArray[i] != 0) zero = false;
    }
    return zero;
}

// function to set the number of rows of a matrix
void matrix::setRows(int m) { rows = m; }

// function to set the number of columns of a matrix
void matrix::setColumns(int n) { columns = n; }

// function to set an element of a matrix
void matrix::setElement(int i, int j, double x) { dataArray[loc(i, j)] = x; }

// function to return the number of rows of a matrix
double matrix::getRows() { return rows; }

// function to return the number of columns of a matrix
double matrix::getColumns() { return columns; }

// function to return an element of a matrix
double matrix::getElement(int i, int j) { return dataArray[loc(i, j)]; }

// function to return the i,jth minor of a matrix
matrix matrix::minor(int i, int j) {
    matrix min(rows - 1, columns - 1);
    bool seenMissedRow(false);
    // adapted from www.geeksforgeeks.org/determinant-of-a-matrix/
    int m(1), n(1);
    for (int k(1); k < rows + 1; k++) {
        for (int l(1); l < columns + 1; l++) {
            if (k != i && l != j) {
                min.dataArray[loc(m, n++)] = dataArray[loc(k, l)];
                if (n == columns) { n = 0; m++; }
            }
        }
    }
    return min;
}

// function to (recursively) find the determinant of a matrix
double matrix::determinant() {
    double det;
    if (rows != columns) cerr << "Error: determinant undefined for a non-square matrix" << endl;
    else {
        if (rows == 1 && columns == 1) det = dataArray[0];
        else if (rows == 2 && columns == 2) {
            det = dataArray[0] * dataArray[3] - dataArray[1] * dataArray[2];
        }
        else {
            int sign(1);
            det = 0;
            for (int j(1); j < columns + 1; j++) {
                det += sign * dataArray[loc(1, j)] * minor(1, j).determinant();
                sign = -sign;
            }
        }
    }
    return det;
}

// friend function to overload insertion operator
ostream & operator << (ostream &os, const matrix &A) {
    for (int i(0); i < A.rows; i++) {
        for (int j(0); j < A.columns; j++) {
            if (j != 0) os << ",";
            os << A.dataArray[j + i*A.rows];
        }
        os << "\n";
    }
    return os;
}

// friend function to overload extraction operator
istream & operator >> (istream &is, matrix &A) {
    string inStr;
    int tempRows(0), tempColumns(1);
    is >> inStr;
    
    // find rows and columns
    bool seenNewRow(false);
    for (int i(0); i < inStr.size(); i++) {
        if (inStr[i] == ',' && !seenNewRow) { tempColumns++; }
        if (inStr[i] == ')') { tempRows++; seenNewRow = true; }
    }
    
    // temporary vector, will be deep copied to A later
    vector <double> tempVec;
    
    // go through and isolate each number and convert to double
    try {
        string numStr;
        for (int i(0); i < inStr.size(); i++) {
            if (inStr[i] != ',' && inStr[i] != '(' && inStr[i] != ')') numStr += inStr[i];
            else {
                if ((inStr[i] == ',' && inStr[i - 1] != ')') || inStr[i] == ')') {
                    tempVec.push_back(stod(numStr));
                    numStr.clear();
                }
            }
        }
        if (tempVec.size() == 0) { cerr << "Error with matrix input" << endl; }
        
        // temporary matrix
        matrix tempMat(tempRows, tempColumns);
        for (int i(0); i < tempRows*tempColumns; i++) {
            tempMat.dataArray[i] = tempVec[i];
        }
        
        // finally copy temp to A
        A = move(tempMat);
    }
    catch (invalid_argument) { cerr << "Error in converting input to double" << endl; }
    return is;
}

// function to overload + operator
matrix matrix::operator + (const matrix &A) {
    matrix sum(rows, columns);
    if (rows == A.rows && columns == A.columns) {
        for (int i(0); i < rows*columns; i++) {
            sum.dataArray[i] = dataArray[i] + A.dataArray[i];
        }
    }
    else cerr << "Error: cannot add two matrices of different dimensions" << endl;
    return sum;
}

// function to overload - operator
matrix matrix::operator - (const matrix &A) {
    matrix sub(rows, columns);
    if (rows == A.rows && columns == A.columns) {
        for (int i(0); i < rows*columns; i++) {
            sub.dataArray[i] = dataArray[i] - A.dataArray[i];
        }
    }
    else cerr << "Error: cannot subtract two matrices of different dimensions" << endl;
    return sub;
}

// function to overload * operator
matrix matrix::operator * (const matrix &A) {
    matrix product(A.rows, columns);
    if (columns == A.rows) {
        // for each i,jth element of the product matrix
        for (int i(1); i < rows + 1; i++) {
            for (int j(1); j < columns + 1; j++) {
                // loop over the two matrices being multiplied
                for (int m(1); m < columns + 1; m++) {
                    product.dataArray[loc(i, j)] += dataArray[loc(i, m)] * A.dataArray[loc(m, j)];
                }
            }
        }
    }
    else cerr << "Error: cannot multiply two matrices due to incorrect dimensions" << endl;
    return product;
}

int main() {
    // demonstrating deep copy and extraction operator
    matrix A(2, 2);
    A.setElement(1, 1, 1); A.setElement(1, 2, 2); A.setElement(2, 1, 3); A.setElement(2, 2, 4);
    cout << "Matrix A:\n" << A;
    cout << "Matrix B is a deep copy of A using assignment operator:\n";
    matrix B; B = A;
    cout << B;
    cout << "Matrix C is a deep copy of A using copy constructor:\n";
    matrix C(A);
    cout << C;
    cout << "Now changing an element of A:\n";
    A.setElement(1, 1, 5); cout << A;
    cout << "But B and C are unchanged:\nB:\n" << B << "C:\n" << C << endl;
    
    // demonstrating overloaded + - * operators
    cout << "Calculations with overloaded operators +, -, *" << endl;
    cout << "A + B:\n" << A + B;
    cout << "A - B:\n" << A - B;
    cout << "A * B:\n" << A * B << endl;
    
    // attempting to use + - * operators on matrices of incorrect sizes
    cout << "Attempting to use +, -, * operators with incorrect sizes\n";
    matrix D(3, 3);
    D.setElement(1, 1, 1); D.setElement(1, 2, 2); D.setElement(1, 3, 3);
    D.setElement(2, 1, 4); D.setElement(2, 2, 5); D.setElement(2, 3, 6);
    D.setElement(3, 1, 7); D.setElement(3, 2, 8); D.setElement(3, 3, 10);
    cout << "Matrix D:\n" << D;
    matrix sum( A + D );
    matrix sub( A - D );
    matrix product( A * D );
    
    // demonstrating minor and recursive determinant
    int rowElim(3), colElim(3);
    cout << "\nFinding the determinant of A:\n" << A.determinant() << endl;
    cout << "Finding the (" << rowElim << "," << colElim << ") minor of D, by eliminating row " << rowElim << " and column " << colElim << ":\n" << D.minor(rowElim, colElim);
    cout << "Finding the determinant of D recursively using minors:\n" << D.determinant() << endl << endl;
    
    // demonstrating move operator and assignment
    cout << "Values from matrix A moved to matrix E by move constructor:\n";
    matrix E(move(A));
    cout << E;
    cout << "So now A is just:\n" << A << endl;
    cout << "Values from matrix B moved to matrix F by move assignment operator:\n";
    matrix F; F = move(B);
    cout << F;
    cout << "So now B is just:\n" << B << endl;
    
    
    // demonstrating insertion operator
    cout << "Enter a matrix of the form (a,b,...),(c,d,...),..." << endl;
    matrix input1;
    while (!(cin >> input1) || cin.fail() || input1.matrixIsBlank()) {
        cout << "Please try again" << endl;
        cin.clear();
    }
    cout << "Your matrix was\n" << input1 << endl;
    
    cout << "The determinant is " << input1.determinant() << endl;
    return 0;
}
