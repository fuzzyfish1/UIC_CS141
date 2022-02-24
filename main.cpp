/* ---------------------------------------------
    Program 1: Gladiolus
        Prompt for the number of sections and display
        an ASCII gladiolus flower, with the sections centered
        above each other.

    Course: CS 141, Fall 2021. Tues 5pm lab
    System: Ubuntu using terminal(gcc), and CLion
    Author: Zain Ali

    Original ASCII graphics gives:
                                         ------------------------------------------

                                             zzzzzzz      A      IIIIIII N    N
                                                  z      A A        I    NN   N
                                                 z      A   A       I    N N  N
                                                z      AAAAAAA      I    N  N N
                                               z      A       A     I    N   NN
                                             zzzzzzz A         A IIIIIII N    N

                                         ------------------------------------------
*/

#include <iostream>   // for cin and cout
#include <iomanip>    // The number in setw(...) is the total of blank spaces including the printed item.

using namespace std;  // so that we don't need to preface every cin and cout with std::

// this function displays my name, where every large character
// is written in its own character
void displayGraphic() {

	// display a line across the top
	cout << setw(41) << " ";
	for (int i = 0; i < 42; i++) {
		cout << "-";
	}

	cout << endl << endl;
	// Display ASCII graphics for "ZAIN"
	cout << setw(45) << " " << "zzzzzzz      A      IIIIIII N    N  \n"
		 << setw(45) << " " << "     z      A A        I    NN   N  \n"
		 << setw(45) << " " << "    z      A @ A       I    N N  N  \n"
		 << setw(45) << " " << "   z      AAAAAAA      I    N  N N  \n"
		 << setw(45) << " " << "  z      A       A     I    N   NN  \n"
		 << setw(45) << " " << "zzzzzzz A         A IIIIIII N    N  \n"
		 << endl;
	cout << setw(41) << " ";

	// Display a line across the bottom
	for (int i = 0; i < 42; i++) {
		cout << "-";
	}
	cout << endl;
}

void displayGladiolus(int totalSections) {

	// the initial line at the top
	cout << setw(totalSections + 3) << "---" << endl;

	// loops for every section of the Gladiolus flower
	for (int sectionCounter = 1; sectionCounter <= totalSections; sectionCounter++) {

		// this is a loop creating everything above the line with the @
		for (int i = sectionCounter; i > 1; i--) {
			cout << setw(totalSections - sectionCounter + i) << "(";
			cout << setw(2 * (sectionCounter - i + 2));
			cout << ")" << endl;
		}

		// this shows is the center line of
		// each flower with the @ symbol
		cout << setw(totalSections - sectionCounter + 1);
		cout << "(";
		cout << setw(sectionCounter + 1);
		cout << "@";
		cout << setw(sectionCounter + 1);
		cout << ")" << endl;

		// this is the loop the bottom part of the flower
		for (int i = 1; i < sectionCounter; i++) {
			cout << setw(totalSections - sectionCounter + i + 1) << "(";
			cout << setw(2 * (sectionCounter - i + 1));
			cout << ")" << endl;
		}
		// this is the flower section divider
		cout << setw(totalSections + 3) << "---" << endl;

	}
	// this prints the stem and leaves
	int leafPattern = 3;

	for (int leafLineNum = 1; leafLineNum <= 2 * totalSections; leafLineNum++) {

		// print leaf based on pattern
		if (leafPattern == 0 || leafPattern == 2) {
			cout << setw(totalSections + 2);
			cout << "|" << endl;
		} else if (leafPattern == 1) {
			cout << setw(totalSections + 2);
			cout << "\\|" << endl;
		} else {
			cout << setw(totalSections + 3);
			cout << "|/" << endl;
		}
		// changing to the next leaf pattern
		leafPattern++;
		leafPattern %= 4;
	}

}

int main() {

	// Display the menu and get the user choice
	int menuOption = 0;

	// the menu and text selector
	cout << "Program 1: Gladiolus            \n"
		 << "Choose from among the following options:  \n"
		 << "   1. Display original graphic  \n"
		 << "   2. Display Gladiolus         \n"
		 << "   3. Exit the program          \n"
		 << "Your choice -> ";
	cin >> menuOption;

	// Handle menu option of 3 to exit
	if (menuOption == 3) {

		exit(0);
	}
		// Handle menu option of 1 to display custom ASCII graphics
	else if (menuOption == 1) {
		displayGraphic();
	}//end if( menuOption == 1)  // menu option to display custom graphic

	else if (menuOption == 2) {
		// Prompt for and get the number of flower sections.
		int numberOfSections = 0;
		cout << "Number of sections -> ";
		cin >> numberOfSections;
		// my gladiolus display function
		displayGladiolus(numberOfSections);

	} //end if( menuOption == 2) // menu option to display building

	// the end of the program
	cout << "Exiting" << endl;
	return 0;
}// end main()
