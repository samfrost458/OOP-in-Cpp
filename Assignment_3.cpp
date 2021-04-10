// Assignment 3
// Physics course database
// Sam Frost 9907936

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> // includes sort(), find()

using namespace std;

// function to strip string of white space and remove uppercase
string noSpaceOrLowercase(string str) {
	// from http://www.cplusplus.com/forum/beginner/9557/
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	// from https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

// function to test if a string is an integer
bool isInt(string str) {
	// attempt to convert string to int with stoi()
	// if stod() fails, it throws an invalid_argument
	int integer;
	try {
		integer = stoi(str);
		return true;
	}
	catch (invalid_argument) {
		// error in converting string to int
		return false;
	}
}

// function to ask user for an integer out of a certain list
int askIntegerExceptions(vector <int> exceptions) {
	int input;
	bool allowed{ false };
	while (!allowed) {
		// check input for validity
		while (!(cin >> input) || cin.get() != '\n' || cin.fail()) {
			cerr << "Error: please enter an integer ";
			cin.clear(); cin.ignore(10000, '\n');
		}
		
		// check input to see if it is allowed from the list
		for (int i{ 0 }; i < exceptions.size(); i++) {
			if (input == exceptions[i]) allowed = true;
		}
		if (allowed) {
			return input;
		}
		else {
			// not allowed, tell user
			cerr << "Error: input must be from list (" << exceptions[0];
			for (int i{ 1 }; i < exceptions.size(); i++) {
				cout << ", " << exceptions[i];
			}
			cout << ")" << endl;
		}
	}
}

// function to split strings by a delimiting character (eg. space (by default))
vector <string> splitUp(string str, char delim = ' ') {
	stringstream ss(str);
	stringstream ssDelim(delim);
	vector <string> splitUpArray(istream_iterator < string > {ss}, istream_iterator < string >(ssDelim));
	return splitUpArray;
}

// function to apply capital letter formating to a string
string capitalFormat(string strIn) {
	string strOut;
	// strip strIn of all uppercase
	transform(strIn.begin(), strIn.end(), strIn.begin(), ::tolower);

	// split str into words by space
	vector <string> wordArray{ splitUp(strIn, ' ') };

	// array of words to not capitalise
	vector <string> exceptions;
	exceptions.push_back("and"); exceptions.push_back("of"); exceptions.push_back("in");
	exceptions.push_back("for"); exceptions.push_back("to"); exceptions.push_back("the");

	// loop over all words and capitalise first letter
	for (int i{ 0 }; i < wordArray.size(); i++) {
		bool exception{ false };
		
		// check for exceptions
		for (int j{ 0 }; j < exceptions.size(); j++) {
			if (wordArray[i] == exceptions[j]) exception = true;
		}
			
		if (!exception) {
			// if no exception, captilise first letter of word
			wordArray[i][0] = toupper(wordArray[i][0]);
			if (wordArray[i] == "(m)") wordArray[i] = "(M)";
		}

		// put every element of wordArray into output string
		strOut += wordArray[i];
		strOut += " ";
	}
	return strOut;
}

//function to ask user for a string input and return an array with the string separated by some delimiting character (eg. space (by default))
vector <string> askInputArray(char delim = ' ') {
	string input;
	getline(cin, input, '\n');

	// split up input by space
	vector <string> inputArray{ splitUp(input, delim) };

	return inputArray;
}

// function to ask user for course input and process string into a vector
vector <string> askForCourseInput() {
	//declare variables
	vector <string> inputArray, courseList;
	string input, courseTitle;
	bool notFinished{ true };
	while (notFinished) {
		inputArray = askInputArray(' ');
		while (inputArray.size() <= 1 || !isInt(inputArray[0])) {
			// important to put inputArray.size() check first, since if a blank line was entered inputArray[0] would not exist
			// user has entered in the wrong format, ask again
			if (inputArray.size() != 0 && noSpaceOrLowercase(inputArray[0]) != "x") {
				cerr << "Error: please enter a course in the form: YXXXX Course Name" << endl;
			}
			
			if (inputArray.size() != 0 && noSpaceOrLowercase(inputArray[0]) == "x") break;
			// if error message hasn't printed by now, a blank line was entered, do nothing
			inputArray.clear();
			inputArray = askInputArray(' ');
		}

		// if user inputs 'x' or 'X' then exit loop
		if (noSpaceOrLowercase(inputArray[0]) == "x") {
			notFinished = false;
			break;
		}

		// assign inputArray elements to courseNum and courseTitle
		int courseNum{ stoi(inputArray[0]) };
		string courseTitle;
		for (int i{ 1 }; i < inputArray.size(); i++) {
			courseTitle += inputArray[i] + " ";
		}

		// apply capital letter formating
		courseTitle = capitalFormat(courseTitle);

		// combine courseNum and courseTitle into single stringstream
		stringstream combinedCourse;
		combinedCourse << "PHYS " << courseNum << "\t" << courseTitle;

		// put combinedCourse stringstream into the vector 'courseList'
		courseList.push_back(combinedCourse.str());
		combinedCourse.str(""); // clear combinedInput buffer
	}
	return courseList;
}

// function to compare user string input to arguments
string askAorB(string A, string B) {
	string input;
	while (!(cin >> input) || cin.fail() || !(noSpaceOrLowercase(input) == A || noSpaceOrLowercase(input) == B)) {
		// check if modified (without spaces or uppercase) input is equal to the strings A or B
		cerr << "Error: please enter '" << A << "' or ' " << B << "' ";
		cin.clear(); cin.ignore(10000, '\n');
	}

	if (noSpaceOrLowercase(input) == A) {
		return noSpaceOrLowercase(A);
	}
	if (noSpaceOrLowercase(input) == B) {
		return noSpaceOrLowercase(B);
	}
}

// function to check if user-entered year is usable
void yearIsUsable(vector <string> courseList, int& printYear) {
	bool useableYear{ false };
	while (!useableYear) {
		for (int i{ 0 }; i < courseList.size(); i++) {
			// convert iterator to string
			string course{ courseList[i] };
			char courseYearChar{ course[5] }; // element 5 gives the year, after 'PHYS '
			string courseYearStr; courseYearStr.push_back(courseYearChar);
			int courseYear = stoi(courseYearStr);

			if (courseYear == printYear) useableYear = true;
		}

		if (!useableYear) {
			// not found useable year after loop, ask user again
			cerr << "You did not seem to enter a course in year " << printYear << " earlier" << endl;
			cout << "Which year would you like to print from (1, 2, 3, 4)?" << endl;
			vector <int> allowedYears; allowedYears.push_back(1); allowedYears.push_back(2); allowedYears.push_back(3); allowedYears.push_back(4);
			printYear = askIntegerExceptions(allowedYears);
		}
	}
}

// function to print out a vector, sorting by some parameter (e.g. alphabetical, number)
void printCourseList(vector <string> courseList, string printStyle, string allOrYear, int printYear = 0) {
	// determine sorting from printstyle and allOrYear
	if (printStyle == "al") {
		// sort alphabetically
		vector <string> courseAlphabetical;
		for (int i{ 0 }; i < courseList.size(); i++) {
			// copy courseList to new temporary array courseAlphabetical
			vector <string> tempArr;
			string tempStr;

			tempStr = courseList[i];
			tempArr = splitUp(tempStr); tempStr = ""; // clear tempStr, since we will use it again
			for (int j{ 2 }; j < tempArr.size(); j++) {
				tempStr += tempArr[j] + " ";
			}
			tempStr += tempArr[0] + " "; // PHYS header
			tempStr += tempArr[1] + " "; // put course number last so that sort algorithm sees course title first

			courseAlphabetical.push_back(tempStr);
		}
		
		// sort courseAlphabetical
		vector <string> ::iterator courseAlphBegin{ courseAlphabetical.begin() }, courseAlphEnd{ courseAlphabetical.end() };
		sort(courseAlphBegin, courseAlphEnd);

		// copy courseAlphabetical back to courseList
		courseList.clear();
		for (int i{ 0 }; i < courseAlphabetical.size(); i++) {
			vector <string> tempArr;
			string tempStr;

			tempStr = courseAlphabetical[i];
			tempArr = splitUp(tempStr); tempStr = ""; // clear tempStr, since we will use it again
			tempStr += tempArr[tempArr.size()-2] + " "; // PHYS header
			tempStr += tempArr[tempArr.size()-1] + "\t"; // course number
			for (int j{ 0 }; j < tempArr.size()-2; j++) {
				tempStr += tempArr[j] + " "; // course title
			}

			// tempStr is now back in original format, push onto courseList (which has been cleared)
			courseList.push_back(tempStr);
		}
	}

	// declare iterators
	vector <string> ::iterator courseListBegin{ courseList.begin() }, courseListEnd{ courseList.end() }, printIterator;

	if (printStyle == "num") {
		// sort numerically
		sort(courseListBegin, courseListEnd);
	}
	
	// print
	bool wantToPrint;
	for (printIterator = courseListBegin; printIterator < courseListEnd; printIterator++) {
		if (allOrYear == "all") wantToPrint = true;

		if (allOrYear == "year" || printYear != 0) {
			// convert iterator to string
			string course{ *printIterator };
			char courseYearChar{ course[5] }; // element 5 gives the year, after 'PHYS '
			string courseYearStr; courseYearStr.push_back(courseYearChar);
			int courseYear = stoi(courseYearStr);
			
			// check if courseYear and printYear are equal
			if (courseYear == printYear) wantToPrint = true;
			else wantToPrint = false;
		}

		if (wantToPrint) cout << *printIterator << endl;
	}
}

int main() {
	// declare variables
	vector <string> courseList;
	int printYear{ 0 };
	string allOrYear;
	string printStyle;
	bool inputIsInvalid{ true };

	// ask user to input course details
	cout << "Enter the course number YXXXX and course title on the same line, with a space in between. "
		 << "Enter as many as you like, and when you are finished, enter 'x'" << endl;
	while (inputIsInvalid) {
		courseList = askForCourseInput();
		// check if user did not input anything
		if (courseList.size() == 0) cout << "You did not seem to enter any courses, please enter some" << endl;
		else inputIsInvalid = false;
	}

	// print out the whole list
	cout << "\nList of courses (by course number):" << endl;
	printCourseList(courseList, "num", allOrYear = "all");

	// ask the user which year they want to print
	cout << "\nWhich year would you like to print from (1, 2, 3, 4)?" << endl;
	vector <int> allowedYears; allowedYears.push_back(1); allowedYears.push_back(2); allowedYears.push_back(3); allowedYears.push_back(4);
	printYear = askIntegerExceptions(allowedYears);

	// check if user input on printYear is valid
	yearIsUsable(courseList, printYear);

	// ask the user whether they want to print alphabetically or by number
	cout << "\nIndicate whether you want this course list to be printed alphabetically ('al'), or by course number ('num')" << endl;
	printStyle = askAorB("al", "num");

	// print the course list as specified
	cout << endl;
	printCourseList(courseList, printStyle, allOrYear = "year", printYear);

	return 0;
}
