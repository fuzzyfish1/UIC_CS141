/* ---------------------------------------------
Program 5: Unscrambler program

Course: CS 141, Fall 2021
System: Ubuntu nano, Windows subsystem for linux(Ubuntu) nano, Windows CLion
Author: Zain Ali

A game on a board, the board size is specified by the user.
 The dificulty of the game is specified by the user,
 and determines how scrambled the board is initially.
 The user is given a set of words to unscramble, by rotating a column or row.
 Each character is a letter on the board.
 Each Word must have one space in between
----------------------------------------------
*/
#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <vector>     // For vectors
#include <string>     // String operations
#include <algorithm>  // use of vector algorithms

using namespace std;

// used to determine whether the computer is linux or windows
// if it is linux, set isLinux to true, allowing for a slightly different file read
// if it is windows, set isLinux to false, and change the filepath for the dictionary
#ifdef _WIN32
	const bool isLinux =false;
	const string dictFilePath = "../dictionary-3to5-letters.txt";
#else
	const bool isLinux = true;
	const string dictFilePath = "dictionary-3to5-letters.txt";
#endif

// the board class stores information about the board
class board {

public:

	// constructor
	// size is the size of the board
	// scrambleNum is the number of times a board is scrambled
	board(int size = 0, int scrambleNum = 0) {
		this->boardRows = size;
		this->boardColumns = size;
		this->scrambleNum = scrambleNum;
		this->boardState = vector<char>(this->boardColumns * this->boardRows, 1);
	}

	//boardDataToPrint, prints a board in a readable format
	void printBoard(vector<char> boardDataToPrint) {

		for (int i = 0; i < this->boardColumns; i++) {
			cout << "   " << i;
		}

		cout << endl;

		//cout << "   " << setfill('-') << setw(this->boardColumns * 4) << endl;
		cout << "   " << string(this->boardColumns * 4 - 1, '-') << endl; //setw and setfill

		for (int i = 0; i < boardRows; i++) {

			cout << i << " | ";

			for (int j = 0; j < boardColumns; j++) {
				cout << boardDataToPrint.at(boardRows * i + j) << " | ";
			}
			cout << endl;
			cout << "   " << string(this->boardColumns * 4 - 1, '-') << endl;
			//cout << endl;
		}

		cout << "Current board words:";

		for (char i: boardDataToPrint) {
			cout << i;
		}

		cout << endl;

	}

	// populateBoard, populates the board with random words from a dictionary
	// dictionary, is a vector of strings, which is the dictionary the random words are chosen from
	void populateBoard(const vector<string> &dictionary) {

		int maxWordSize = 5;
		int minWordSize = 3;

		int boardIndex = 0;

		int remainingSpace = boardRows * boardColumns;

		// words are at least 3 characters
		// so when remainingSpace is less than 3, the board is full
		while (remainingSpace > 2) {

			// word that might be put into the board
			string newWord = dictionary.at(rand() % dictionary.size());

			// if the word is too long, it is not put into the board
			if (newWord.size() >= minWordSize && newWord.size() <= maxWordSize) {

				// this adds the word to a list of all chosen words, for later use
				this->chosenWords.push_back(newWord);

				// add a letter at each corresponding spot,
				// bc vector is 1D we dont need to acount for wrapping to next row
				for (int index = 0; index < newWord.size(); index++, boardIndex++) {
					this->boardState.at(boardIndex) = newWord.at(index);
					remainingSpace--;
				}

				// add a space character at the end of the board if we have room
				if (remainingSpace >= 1) {
					this->boardState.at(boardIndex) = ' ';
					boardIndex++;
					remainingSpace--;
				}

			} else {
				// word is invalid and will not be entered into the board
				// find a new word
				continue;
			}

			//recalculates the max Word Size based on remaining space
			maxWordSize = (remainingSpace >= 5) ? 5 : remainingSpace;

		}

		// after no more words can be put on board
		// fill remaining board with space character
		for (int i = 0; i < boardRows * boardColumns; i++) {
			if (this->boardState.at(i) == 1) {
				this->boardState.at(i) = ' ';
			}
		}

		// store the unscrambled board for later use
		this->unscrambledBoardState = this->boardState;
	}

	// rotates a column on the board
	// col is the index of the column to be rotated (assumed that col inputs are valid, bc validity is checked elsewhere)
	// rotations is how much to rotate by (can be negative)
	void rotateCol(int col, int rotations) {

		// create a board to rotate
		vector<char> newStates = this->boardState;

		// index on the board that is being rotated
		int newStatesIndex;
		// index on the board that is still unmodified
		int oldStatesIndex;

		// rotate things
		for (int i = 0; i < this->boardRows; i++) {

			// this is really similar to the rotate row function, except the indexes are multiplied by columns to move down a row instead
			// + col, shifts everything to correct column
			// % boardRows, wraps an index to the top of the board
			oldStatesIndex = this->boardColumns * ((i + this->boardRows) % this->boardRows) + col;
			newStatesIndex = this->boardColumns * ((i + rotations + this->boardRows) % this->boardRows) + col;

			newStates.at(newStatesIndex) = this->boardState.at(oldStatesIndex);

		}

		// apply rotation changes to the current board
		this->boardState = newStates;

	}

	// rotates a row on the board
	// row shows the index of the row to be rotated (assumed that row inputs are valid, bc validity is checked elsewhere)
	// rotations is the amount to rotate by (can be negative)
	void rotateRow(int row, int rotations) {

		// create a board to rotate
		vector<char> newStates = this->boardState;

		// smallest index of the row to be rotated
		int rowIndex = row * boardColumns;

		// algorithm is based on an observation from the CS141 website that
		// the originalString at the index rotations has the new value of the same string at index 0

		int newStatesIndex;
		int oldStatesIndex;

		// rotate things
		for (int i = 0; i < this->boardColumns; i++) {

			// % this->boardColumns allows wraping of the index to other side of board
			// + this->boardColumns changes possible rotations from negative to corresponding positive value
			// + rowIndex makes sure that the index is on the correct row
			oldStatesIndex = ((i + this->boardColumns) % this->boardColumns) + rowIndex;
			newStatesIndex = ((this->boardColumns + rotations + i) % this->boardColumns) + rowIndex;

			newStates.at(newStatesIndex) = this->boardState.at(oldStatesIndex);

		}

		// applies changes to the board
		this->boardState = newStates;

	}

	// scrambles the board, up to the number scrambleNum specified by the user elsewhere
	void scramble() {

		// rotates a random row then random column on board by 1
		for (int i = 0; i < this->scrambleNum; i++) {
			this->rotateRow(rand() % this->boardRows, 1);
			this->rotateCol(rand() % this->boardColumns, 1);
		}

		// store the scrambled board for later use
		this->afterScrambleBoardState = this->boardState;

	}

	// prints out the goal unscrambling of the user
	void printChosenWords() {

		cout << " These are the words that you should try to"
				" spell out using the board, in order: " << endl;

		cout << "   " << this->chosenWords.at(0);

		for (int i = 1; i < this->chosenWords.size(); i++) {
			cout << ", " << this->chosenWords.at(i);
		}
		cout << "." << endl;
	}

	// getters and setters for private variables in board class
	vector<char> getCurrentBoardStates() {
		return this->boardState;
	}

	vector<char> getUnscrambledBoardStates() {
		return this->unscrambledBoardState;
	}

	vector<string> getChosenWords() {
		return this->chosenWords;
	}

	int getBoardRows() const {
		return this->boardRows;
	}

	int getBoardCols() const {
		return this->boardColumns;
	}

	int getScrambleNum() const {
		return this->scrambleNum;
	}

	void setUnscrambledBoardStates(const vector<char> &unscrambledBoardStates) {
		this->unscrambledBoardState = unscrambledBoardStates;
	}

	void setBoardState(const vector<char> &boardState) {
		this->boardState = boardState;
	}

	vector<char> getAfterScrambleBoardStates() {
		return this->afterScrambleBoardState;
	}

	void setScrambleNum(int scrambleNum) {

		this->scrambleNum = scrambleNum;
	}

	void setBoardRows(int rows) {
		this->boardRows = rows;
	}

	void setBoardCols(int cols) {
		this->boardColumns = cols;
	}

private:
	// number of rows
	int boardRows;
	// number of columns
	int boardColumns;
	// number of times to scramble the board
	int scrambleNum;

	// current values of all board characters
	vector<char> boardState;
	// values of all board characters before scrambling
	vector<char> unscrambledBoardState;
	// values of all board characters after scrambling
	vector<char> afterScrambleBoardState;

	// vector of words chosen at random by populateBoard()
	vector<string> chosenWords;

};

// for reading a file
// filepath parameter is the path to the file to be read
vector<string> readFile(const string &filePath) {

	// open file
	ifstream inStream;
	inStream.open(filePath);

	// if the file doesnt exist or open properly throw an error
	assert(!inStream.fail());

	// place to store the words into
	vector<string> dictionary;

	string newWord;

	// read a line into newWord until end of file
	while (getline(inStream, newWord)) {

		// we do this so the input is consistent across zybooks(linux) and my windows computer
		// this deals with the extra /r at the end of all lines in linux machines, this does not exist on windows machines
		if (isLinux) {
			newWord.erase(newWord.end() - 1);
		}

		dictionary.push_back(newWord);
	}

	// close the file
	inStream.close();

	return dictionary;

}

// this allows the user to set the number of rows and columns and difficulty of the board
// this is only run the first time optionB is run
// gameBoard is which board to set the values for
void getBoardValues(board &gameBoard) {

	int boardSize;
	int scrambleNum;

	cout << "Choose your board size (must be a number greater than or equal to 4):" << endl;

	//int boardSize;
	cin >> boardSize;

	while (boardSize < 4) {
		cout << " The board size must be at least 4. Retry.";
		cout << "Choose your board size (must be a number greater than or equal to 4):" << endl;
		cin >> boardSize;
	}

	cout << " Choose the number of times you would like the board to be scrambled " <<
		 "(must be a single number >= 1): \n";

	//int scrambleNum;
	cin >> scrambleNum;

	while (scrambleNum < 1) {
		cout << "The number of times you select must be at least 1. Retry.";
		cout
				<< "Choose the number of times you would like the board to be scrambled (must be a single number >= 1): "
				<< endl;
		cin >> scrambleNum;
	}

	gameBoard.setScrambleNum(scrambleNum);
	gameBoard.setBoardRows(boardSize);
	gameBoard.setBoardCols(boardSize);

}

// creates and initializes a new board
void generateNewBoard(board &gameBoard, const vector<string> &dictionary) {

	int boardSize = gameBoard.getBoardRows();
	int scrambleNum = gameBoard.getScrambleNum();

	gameBoard = board(boardSize, scrambleNum);

	gameBoard.populateBoard(dictionary);
	gameBoard.scramble();
}

// this is called for optionB
// gameBoard is the board to modify
// dictionary is the dictionary to fill with random words
// isFirstB is a bool that is true if this is the first time optionB is run
void optionB(board &gameBoard, const vector<string> &dictionary, bool isFirstB = false) {

	// creates as board and initializes it for the first time
	if (!isFirstB) {

		getBoardValues(gameBoard);
		generateNewBoard(gameBoard, dictionary);

	} else {

		gameBoard.setBoardState(gameBoard.getAfterScrambleBoardStates());
	}

}

// this is the menu for rotations
void RotationMenu(int size) {
	cout << "Enter the row/column you would like to rotate, and the number of positions to rotate by.\n" <<
		 "   This should be in the format of <R or C> <row/column number> <number of positions to rotate>, \n" <<
		 "   where valid row and column numbers are between 0 and " << size - 1 << ", \n" <<
		 "   E.g. R 0 1 would rotate the top row (row 0) of the board to the right once, \n" <<
		 "        c 1 -2 would rotate the second column (col 1) of the board upwards twice. \n" <<
		 "   Your choice:" << endl;
}

// option R
// gameBoard is the board to modify
void optionR(board &gameBoard) {

	RotationMenu(gameBoard.getBoardCols());

	char option;
	int indexNum;
	int rotations;

	// option are converted to uppercase to account all options
	// for example, 'r' and 'R' are the same
	cin >> option >> indexNum >> rotations;

	option = toupper(option);

	// checks validity of inputs
	bool optionInvalid = option != 'R' && option != 'C';
	bool indexInvalid = (indexNum < 0) || (indexNum >= gameBoard.getBoardCols());

	// reinput until valid inputs are entered
	while (optionInvalid || indexInvalid) { // if something is invalid get new inputs

		// cout statements, prioritizing options first
		if (optionInvalid) {
			cout << "First input must be 'R' or 'C'. Try again." << endl;
		} else if (indexInvalid) {
			cout << "Number must be between 0 and " << gameBoard.getBoardCols() - 1
				 << ". Try again." << endl;
		}

		RotationMenu(gameBoard.getBoardCols());

		// re input both even if only one is wrong
		cin >> option >> indexNum >> rotations;
		option = toupper(option);

		optionInvalid = option != 'R' && option != 'C';
		indexInvalid = (indexNum < 0) || (indexNum >= gameBoard.getBoardCols());

	}

	if (option == 'R') { // rotate row
		gameBoard.rotateRow(indexNum, rotations);

	} else if (option == 'C') {    // rotate column
		gameBoard.rotateCol(indexNum, rotations);

	} else { // should never get here
		cout << "Invalid option" << endl;
	}

}

// menu that shows menu options everytime
void optionMenu() {
	cout << "Enter one of the following: \n"
			"   R to rotate a row or column, \n"
			"   C to view what the completed board should look like, \n"
			"   B to reset the board back to the beginning, \n"
			"   G to generate a new board with new random words, \n"
			"   S to have the computer automatically solve a board that you enter (5 points extra credit), \n"
			"   Q to quit. " << endl <<
		 "   Your choice: " << endl;
}

// start menu
void startMenu(int numWords) {
	cout << "Program 5: Unscrambler	" << endl;
	cout << "CS 141, Fall 2021, UIC	" << endl;
	cout << " " << endl;
	cout << "You will be presented with a board that is filled  " << endl;
	cout << "with scrambled words. Rotate the rows and columns  " << endl;
	cout << "to get the words displayed in the right order. " << endl;

	cout << numWords << " words have been read in from the dictionary.\n"
						"\n"
						"Enter Q to quit or B to begin: ";
}

int main() {

	bool isRunning = true;
	bool isFirstB = true;
	char option;

	srand(1);

	board gameBoard;

	vector<string> wordsDict = readFile(dictFilePath);
	int myDictSize = wordsDict.size();

	startMenu(myDictSize);

	while (isRunning) {

		cin >> option;

		option = toupper(option);

		switch (option) {

			case 'R': // rotate a row or column
				optionR(gameBoard);
				break;

			case 'C': // print correct board
				cout << "The completed board should look like: " << endl;
				gameBoard.printBoard(gameBoard.getUnscrambledBoardStates());
				break;

			case 'B': // initialize new board or reset it back to the beginning
				optionB(gameBoard, wordsDict, !isFirstB);
				isFirstB = false;
				break;

			case 'G': // generate new board
				//optionG(gameBoard, wordsDict);
				generateNewBoard(gameBoard, wordsDict);
				break;

			case 'S': // Extra Cred
			// not impossible to implement, but not worth the extra points
			// theoretically very similar to maze program
				//gameBoard.unScrambleOne();
				break;

			case 'Q': // quit
				cout << "Thank you for playing! \n";
				isRunning = false;
				break;

			default:
				cout << "You chose something else" << endl;
				break;
		}

		// if the player wins stop the loop
		if ((gameBoard.getUnscrambledBoardStates() == gameBoard.getCurrentBoardStates()) && isRunning) {

			gameBoard.printBoard(gameBoard.getCurrentBoardStates());
			cout << "Congratulations, you won! Thank you for playing!" << endl;
			break;

		} else if (isRunning) {
			// if the game is still running, show the menu and the board

			gameBoard.printChosenWords();

			cout << "Current board: " << endl;

			gameBoard.printBoard(gameBoard.getCurrentBoardStates());

			optionMenu();
		}

	}

	cout << " Exiting program... ";

	return 0;
}