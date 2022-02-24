/* ---------------------------------------------
    Program 2: Great 13
        Peg puzzle, jumping pieces to try and
        leave only a single piece.

    Course: CS 141, Fall 2021. Tues 5pm lab
    System(s): CLion on lenovo ideapad 330-15ikb Windows 10
    	and
    	nano(Terminal) on my Asus laptop in a box Ubuntu
    Author: Zain Ali

    Running the program looks like:

        Author:  Zain Ali
        Class:   CS 141, Fall '21
        Program: #2 Great 13

        Make a series of jumps until there is a single piece left in the
        middle. On each move you must jump an adjacent piece into an empty
        square, jumping horizontally, vertically, or diagonally.

             Board    Position
               #          A
             # # #      B C D
           # # . # #  E F G H I
             # # #      J K L
               #          M

        1. Enter position to move from, piece to jump, and destination (e.g. EFG): efg
             Board    Position
               #          A
             # # #      B C D
           . . # # #  E F G H I
             # # #      J K L
               #          M

        2. Enter position to move from, piece to jump, and destination (e.g. EFG):

        . . .
   ---------------------------------------------
*/

#include <iostream>
#include <cmath>
#include <cctype>

using namespace std;

/* global variables that store the character value of a peg or a hole
 '#' there is a peg
 '.' there is no peg (hole)
 */

char pA,
		pB,
		pC,
		pD,
		pE,
		pF,
		pG,
		pH,
		pI,
		pJ,
		pK,
		pL,
		pM;

/* variables that store the value of the row and column of a pos
   calculated by getPos(char pos) temporarily
 * */
int rowVal, colVal;

/* this function initializes the game field
   with the values the game board initially has
 * */
void resetField() {

	pA = '#';
	pB = '#';
	pC = '#';
	pD = '#';
	pE = '#';
	pF = '#';
	pG = '.';
	pH = '#';
	pI = '#';
	pJ = '#';
	pK = '#';
	pL = '#';
	pM = '#';

	// at the start only 'G' is empty
}

// ----------------------------------------------------------------------
// Display the game instructions, especially at the begining of the game
void displayInstructions() {

	cout << "Class: CS 141             " << endl
		 << "Program: #2 Great 13      " << endl
		 << "  " << endl

		 << "Make a series of jumps until there is a single piece left in the   " << endl
		 << "middle. On each move you must jump an adjacent piece into an empty " << endl
		 << "square, jumping horizontally, vertically, or diagonally.           " << endl
		 << "Input of 'R' resets the board back to the beginning, and input of  " << endl
		 <<"\'X\' exits the game.\'   "
		 << endl << endl;
}

// ----------------------------------------------------------------------
// Display the board, using the current field, and displays the corresponding letter positions next to it
void displayBoard() {

	cout << "\n "
		 << "    Board   " <<                                                   " Position \n"
		 << "       " <<                       pA <<                       "          A    \n"
		 << "     " <<            pB << " " << pC << " " << pD <<            "      B C D  \n"
		 << "   " << pE << " " << pF << " " << pG << " " << pH << " " << pI << "  E F G H I\n"
		 << "     " <<            pJ << " " << pK << " " << pL <<            "      J K L  \n"
		 << "       " <<                       pM <<                       "          M    \n"
		 << endl;

}

// calculates the row and column of a char pos
// 'G' (center) is at row = 3, col = 3
int getPos(char pos) {

	// total columns in that row
	int totCols = 1;
	// 'A' + letterNum = char we are checking
	char letterToCheck = 'A';

	// loops through rows and cols, finding
	for (int row = 1; row <= 5; row++) {

		//cout << string((5 - totCols), ' ');
		// loops through every value in the column
		for (int col = 1; col <= totCols; col++) {

			// for testing purposes
			// cout << (char) ('A' + letterNum) << " ";

			// if letter found, store in rowVal and ColVal for later
			if (letterToCheck == pos) {

				rowVal = row;
				// math to account for spaces in board grid
				colVal = col + (5 - totCols) / 2;

				return 1; // forces method to end
			}

			// increments the letter
			// 'A' goes to 'B' etc.
			letterToCheck++;
		}

		// changes the total columns for next row
		// it is + 1 or -1 on each side depending on before or after center ('G')
		(letterToCheck > 'G') ? totCols -= 2 : totCols += 2;
	}

}

// checks if a move is valid, 3 letters adjacent and in a straight line
bool isAdjacent(char startPos, char jumpPos, char endPos) {

	// the positions have to be letters in between 'A' and 'M'
	if ((startPos > 'M') && (startPos < 'A')) {
		return false;
	} else if ((jumpPos > 'M') && (jumpPos < 'A')){
		return false;
	} else if ((endPos > 'M') && (endPos < 'A')){
		return false;
	}

	// initialize col and row position variables for start jump and end
	int rowStart = 0, columnStart = 0, rowJump = 0, columnJump = 0, rowEnd = 0, columnEnd = 0;

	// gets col and row pos for each position
	getPos(startPos);
	rowStart = rowVal;
	columnStart = colVal;

	getPos(jumpPos);
	rowJump = rowVal;
	columnJump = colVal;

	getPos(endPos);
	rowEnd = rowVal;
	columnEnd = colVal;

	//the changes in position from 2 positions in the form of (Final - initial)
	int colDeltaStrtJmp = columnStart - columnJump;
	int colDeltaJmpEnd = columnJump - columnEnd;
	int rowDeltaStrtJmp = rowStart - rowJump;
	int rowDeltaJmpEnd = rowJump - rowEnd;

	// no position can be the same letter spot
	if (startPos == jumpPos || jumpPos == endPos || endPos == startPos){
		return false;

	// checks if change in distance is no more than one and in a straight line
	// the change in rows and columns from any 2 letters has to be less than or equal to 1 (adjacentcy check)and
	// the change in rows and cols from start to jump must be the same as from jump to end (linearity check)
	} else if (
			(fabs(colDeltaStrtJmp) <= 1)
			&& (fabs(rowDeltaStrtJmp) <= 1)
			&& (colDeltaJmpEnd == colDeltaStrtJmp)
			&& (rowDeltaStrtJmp == rowDeltaJmpEnd)
			){

		return true;

	// everything else is an illegal move bc nonadjacency or nonlinearity
	} else {
		return false;
	}

}

/* checks if there is a peg '#' at a particular location
   pos is any specified char from the letter field
*/
bool hasPiece (char pos){

	// val stores char value at pos
	char val;

	switch (pos){

		case 'A':
			val = pA;
			break;

		case 'B':
			val = pB;
			break;

		case 'C':
			val = pC;
			break;

		case 'D':
			val = pD;
			break;

		case 'E':
			val = pE;
			break;

		case 'F':
			val = pF;
			break;

		case 'G':
			val = pG;
			break;

		case 'H':
			val = pH;
			break;

		case 'I':
			val = pI;
			break;

		case 'J':
			val = pJ;
			break;

		case 'K':
			val = pK;
			break;

		case 'L':
			val = pL;
			break;

		case 'M':
			val = pM;
			break;

		default:
			val = 'E';
			break;

	}
	return val == '#';

}

// used for changing the board
// dumps a char at a position
void updateField(char pos, char val){

	switch (pos){

		case 'A':
			pA = val;
			break;

		case 'B':
			pB = val;
			break;

		case 'C':
			pC = val;
			break;

		case 'D':
			pD = val;
			break;

		case 'E':
			pE = val;
			break;

		case 'F':
			pF = val;
			break;

		case 'G':
			pG = val;
			break;

		case 'H':
			pH = val;
			break;

		case 'I':
			pI = val;
			break;

		case 'J':
			pJ = val;
			break;

		case 'K':
			pK = val;
			break;

		case 'L':
			pL = val;
			break;

		case 'M':
			pM = val;
			break;

	}

}

// checks if the user has won or not
bool isWin (){

	int filledSpace = 0;

	// checks every spot to count total filled spaces
	(pA == '#') ? filledSpace += 1 : filledSpace += 0;
	(pB == '#') ? filledSpace += 1 : filledSpace += 0;
	(pC == '#') ? filledSpace += 1 : filledSpace += 0;
	(pD == '#') ? filledSpace += 1 : filledSpace += 0;
	(pE == '#') ? filledSpace += 1 : filledSpace += 0;
	(pF == '#') ? filledSpace += 1 : filledSpace += 0;
	(pG == '#') ? filledSpace += 1 : filledSpace += 0;
	(pH == '#') ? filledSpace += 1 : filledSpace += 0;
	(pI == '#') ? filledSpace += 1 : filledSpace += 0;
	(pJ == '#') ? filledSpace += 1 : filledSpace += 0;
	(pK == '#') ? filledSpace += 1 : filledSpace += 0;
	(pL == '#') ? filledSpace += 1 : filledSpace += 0;
	(pM == '#') ? filledSpace += 1 : filledSpace += 0;

	// you win when there is only 1 filled space left
	if(filledSpace > 1){
		//cout << "filled Space " << filledSpace;
		return false;
	} else {
		return true;
	}

	// if the loop finished it means you have 1 peg, and you win
	cout << endl << "Congratulations, you did it!" << endl;

}

// ----------------------------------------------------------------------
// Main section of the program, containing the game play loop
int main() {

	displayInstructions();

	// Set board values to the default starting position
	resetField();

	// Game play loop
	displayBoard();

	int moveNumber = 1;
	char moveFrom, jumpOver, endAt;

	while (true) {

		// obtain user inputs, and converts to uppercase for easier comparisons
		cout << moveNumber << ". "
			 << "Enter positions from, jump, and to (e.g. EFG): ";
		cin >> moveFrom;
		moveFrom = toupper(moveFrom);

		// If 'X' to exit was chosen, break out of game play loop
		if (moveFrom == 'X') {
			cout << "Exiting" << endl << endl;
			cout << "Better luck next time." << endl;
			break;
		}

			// If 'R' to reset the board was chosen, reinitialize everything
			// display the board, and continue back up to re-run the while(...) loop
		else if (moveFrom == 'R') {
			cout << endl;
			cout << "   *** Restarting" << endl;
			moveNumber = 1;
			resetField();
			displayBoard();
			continue;
		}

		// Additionally now also read the piece-to-jump and destination inputs.
		cin >> jumpOver;
		cin >> endAt;

		jumpOver = toupper(jumpOver);
		endAt = toupper(endAt);

		// Ensure location we're moving FROM has a PlayerPiece in it. If it doesn't, then
		// continue back up to re-run the while(...) loop.
		if (!hasPiece(moveFrom)){
			cout << endl;
			cout << "   *** Source must have a piece. Please retry" << endl;
			cout << endl;
			continue;
		}

		// Ensure piece being jumped isn't blank. If it is, then
		// continue back up to re-run the while(...) loop.
		if (!hasPiece(jumpOver)){
			cout << endl;
			cout << "   *** Must jump a piece. Please retry" << endl;
			cout << endl;
			continue;
		}

		// Ensure location we're moving TO is blank. If it is not, then
		// continue back up to re-run the while(...) loop.
		if (hasPiece(endAt)) {
			cout << endl;
			cout << "   *** Destination must be blank. Retry" << endl;
			cout << endl;
			continue;
		}

		// Ensure set of pieces (from, jump, to) are adjacent.  If they aren't, then
		// continue back up to re-run the while(...) loop.
		if (!isAdjacent(moveFrom, jumpOver, endAt)) {
			cout << endl;
			cout << "   *** Pieces must be adjacent. Please retry";
			cout << endl << endl;
			continue;
		}

		// Make the move, update the field and display the board
		moveNumber += 1;

		updateField(moveFrom, '.');
		updateField(jumpOver, '.');
		updateField(endAt,'#');

		displayBoard();

		// ends game when you win
		if (isWin()) {
			cout << endl;
			cout << "Congratulations, you did it!" << endl;
			break;
		}
	}

	cout << "Done." << endl;
	return 0;

}