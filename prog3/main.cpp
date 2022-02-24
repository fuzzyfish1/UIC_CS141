/* main.cpp
    Divvy Bike Data Analysis program.
    Author: Zain Ali
    Date: 11/3/2021
    Class: CS 141, Fall 2021, UIC
    System: Windows 10, Clion
    Menu Option 5 Extra Credit URL: https://www.google.com/maps/place/The+Letterman+Chicago/@41.8761007,-87.6542301,17z/data=!3m2!4b1!5s0x880e2cddf165f823:0xfcdcf8550638fc82!4m5!3m4!1s0x880e2cddf499cd4d:0x5b6dfc7e6bb2f0b8!8m2!3d41.876061!4d-87.6520149
    Coordinates for Home I would like to live in:  41.87625245639088 -87.65214869055426
*/

#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <vector>     // For vectors
#include <string>     // String operations
#include <algorithm>  // use of vector algorithms
#include <cmath>

// we have a line ending difference between Windows and Linux
// zybooks also uses linux line endings(found expirimentally)
// determines if linux or windows
const bool isLinux =
#ifdef _WIN32
	false;
#else
    true;
#endif

using namespace std;

// filepaths for my computer
//const string sampleFilePath = "../divvyridesampledata.csv";
//const string weekendFilePath = "../weekdayweekend.csv";
//const string septFilePath = "../all_divvy_rides_september.csv";

//file paths for zybooks
const string sampleFilePath = "divvyridesampledata.csv";
const string weekendFilePath = "weekdayweekend.csv";
const string septFilePath = "all_divvy_rides_september.csv";

// class trip
// holds all the information for a trip
// one trip is one full line in the csv file
class trip {

public:

	trip() { // default constructor with default params
		this->rideID = -1;
		this->member = true;
		this->startTime = "Default";
		this->endTime = "Default";
		this->startLat = 0;
		this->startLng = 0;
		this->endLat = 0;
		this->endLng = 0;
		this->type = "default";
		this->startStationName = "default";
		this->endStationName = "default";
	}

	trip(string data) { // constructor to construct trip from one line from file
		// assumes input data is a string of comma separated values
		// and that the data is in the correct order
		// and that the data is valid

        // split line into data points separated by a comma
		int findComma = data.find(",");
		this->rideID = data.substr(0, findComma);

		// remove the first data point and comma from the string
		//so we can find the next data point easier
		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		this->type = data.substr(0, findComma);

		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		this->startTime = data.substr(0, findComma);

		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		this->endTime = data.substr(0, findComma);

		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		this->startStationName = data.substr(0, findComma);

		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		this->endStationName = data.substr(0, findComma);

		// same thing just casted into doubles
		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		this->startLat = stod((data.substr(0, findComma)));

		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		this->startLng = stod(data.substr(0, findComma));

		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		this->endLat = stod((data.substr(0, findComma)));

		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		this->endLng = stod((data.substr(0, findComma)));

		// if the ride is a member ride we remove the \r at end of line before passing it in
		data.replace(0, findComma + 1, "");
		findComma = data.find(",");
		string checkMember = data.substr(0, findComma);

		// member is true or false
		if (checkMember == "member") {
			this->member = true;
		} else {
			this->member = false;
		}
	}

	// getters
	string getRideID() {
		return this->rideID;
	}

	string getType() {
		return this->type;
	}

	string getStartTime() {
		return this->startTime;
	}

	string getEndTime() {
		return this->endTime;
	}

	string getStartStationName() {
		return this->startStationName;
	}

	string getEndStationName() {
		return this->endStationName;
	}

	long double getStartLat() {
		return this->startLat;
	}

	long double getStartLng() {
		return this->startLng;
	}

	long double getEndLat() {
		return this->endLat;
	}

	long double getEndLng() {
		return this->endLng;
	}

	bool getMember() {
		return this->member;
	}

	// setters
	void setRideID(string rideID) {
		this->rideID = rideID;
	}

	void setType(bool member) {
		this->member = member;
	}

	void setStartTime(string startTime) {
		this->startTime = startTime;
	}

	void setEndTime(string endTime) {
		this->endTime = endTime;
	}

	void setStartStationName (long double startLat) {
		this->startLat = startLat;
	}

	void setEndStationName (long double startLng) {
		this->startLng = startLng;
	}

	void setStartLat (long double endLat) {
		this->endLat = endLat;
	}

	void setStartLng (long double endLng) {
		this->endLng = endLng;
	}

	void setEndLat(string type) {
		this->type = type;
	}

	void setEndLng(string startStationName) {
		this->startStationName = startStationName;
	}

	void setMember(string endStationName) {
		this->endStationName = endStationName;
	}

private:

	// variables, 1 for each column/datapoint in a csv file
	string rideID;
	string startTime;
	string endTime;
	string type;
	string startStationName;
	string endStationName;
	long double startLat;
	long double startLng;
	long double endLat;
	long double endLng;
	bool member;

};

// function declarations

long double toRadians(const long int degree);

long double distance(long int lat1, long int long1, long int lat2, long int long2);

/*
 * originalSize is a refrence param
 * it puts in the original # trips before file cleaning
 * then you can get #of trips after cleaning with .size()
 * */
vector<trip> readCleanTripsFromFile(string filePath, int &originalSize);

bool isWeekend(string date);

void displayMenu();

vector<trip> optionOne();

void optionTwo(vector<trip> trips);

void optionThree (vector<trip> trips);

void optionFour (vector<trip> trips);

void optionFive(vector<trip> trips);

int main() {

	bool keepGoing = true;
	int optionInput;

	vector<trip> trips;
	cout << "Analyze Divvy bike ride information using September trip data." << endl;
	cout << "First select option 1 to read and clean the data." << endl << endl;

	while (true) { // forces user to select option 1 before continuing

		displayMenu();
		cin >> optionInput;

		if (optionInput == 1) { // option one sets trips for rest of the program
			trips = optionOne();
			break;
		} else {
			cout << "First read in data by selecting menu option 1";
		}
	}

	while (keepGoing) { // main menu and option select without forced option 1

        displayMenu();
        cin >> optionInput;

        switch (optionInput) {

        	case 1: // option one sets trips for the rest of the program
				trips = optionOne();
        	    break;

        	case 2:
				optionTwo(trips);
        	    break;

			case 3:
				optionThree(trips);
                break;

			case 4:
				optionFour(trips);
				break;

			case 5:
				optionFive(trips);
                break;

			case 6: // exit
				keepGoing = false;
				break;

        	default:
        	    cout << "Invalid option. Please try again." << endl;
        	    break;
        }
    }

	return 0;

}

// calculates if a date is on the weekend or weekday
bool isWeekend(string date) {

	// date is in format MM/DD/YYYY
	int dateBreakIndex = date.find("/");
	// all same month so it doesnt matter for this particular program
    //int month = stoi(date.substr(0, dateBreakIndex));

	// all same year so it doesnt matter for this particular program

	date = date.erase(0,dateBreakIndex + 1);

	dateBreakIndex = date.find("/");
	int day = stoi(date.substr(0, dateBreakIndex));

	// counts the weeks back to the first week in the month
	// then can calculate corresponding day of the week
	day %= 7;

	// on 4 and 5 are the first weekends in the month
	if (day == 4 || day == 5) {
        return true;
	} else {
        return false;
    }

}

//function for displaying the menu
void displayMenu() {
	// main menu for user to select option

	cout << "Select a menu option:" << endl;
	cout << "   1. Select datafile, display rides totals and clean data" << endl;
	cout << "   2. Display overall trip information" << endl;
	cout << "   3. Display percentage of members vs. casual riders" << endl;
	cout << "   4. Display weekday vs weekend usage" << endl;
	cout << "   5. Extra Credit: find closest station" << endl;
	cout << "   6. Exit" << endl;
	cout << "Your choice --> ";
}

//function for menu option 1
vector<trip> optionOne() {

	int originalSize = 0;

	vector<trip> cleanTrips;

	cout << "Select datafile:" << endl;
	cout << "1. Small subset of data with 14 rides to help create your program" << endl;
	cout << "2. Week day vs weekend rides" << endl;
	cout << "3. All September 2021 data (not tested in Zybooks)" << endl;
	cout << "Your selection--> " << endl;

	int choice;
	cin >> choice;

	//the pick which file to read
	switch (choice) {

		case 1: // reads sample path
			cleanTrips = readCleanTripsFromFile(sampleFilePath, originalSize);
			break;

		case 2:// reads weekend path
			cleanTrips = readCleanTripsFromFile(weekendFilePath, originalSize);
			break;

		case 3: // reads september file path
			cleanTrips = readCleanTripsFromFile(septFilePath, originalSize);
			break;

		default: // invalid option
			cout << "Not a valid menu option" << endl;
			break;
	};

	cout << "Total # of trips found in datafile: " << originalSize << endl <<
		 "   Total # of trips in clean data: " << cleanTrips.size() << endl;

	return cleanTrips;
}

// reads a file and outputs a cector of clean trips made from that file path
vector<trip> readCleanTripsFromFile(string filePath, int &originalSize) {

	originalSize = 0;

	ifstream inStream;                      // Declare an input stream for reading
	inStream.open(filePath);       // Open dictionary file
	assert(inStream.fail() == false);     // Ensure file open worked

	vector<trip> tripList;

	string newWord;

	// blocks the first read from while to skip title
	bool titleBlock = true;

	// read a line into newWord until end of file
	while (getline(inStream, newWord)) {

		// count original size regardless of it is cleaned or not
		originalSize += 1;

		// if ,, exists in file then it has an empty data point so dont read it
		// also if it is the first line of the file(title) then skip it
		if ((newWord.find(",,") != string::npos) || titleBlock) {
			titleBlock = false;
			continue;
		}

		// removes /r at end of line on linux computers
		if (isLinux) {
			newWord.erase(newWord.end() - 1);
		}

		// adds a new trip to the vector
		tripList.push_back(trip(newWord));
	}

	// removes one from original size to remove title
	originalSize -= 1;

	// Close the dictionary file
	inStream.close();

	return tripList;

}

// Utility functions getting distant between two map points 
// Attribution: https://www.geeksforgeeks.org/program-distance-two-points-earth/  
// converting degrees to radians
long double toRadians(const long double degree) {
	// cmath library in C++
	// defines the constant
	// M_PI as the value of
	// pi accurate to 1e-30
	long double one_deg = (3.14159265358979323846) / 180;
	return (one_deg * degree);
}

long double distance(long double lat1, long double long1,
					 long double lat2, long double long2) {
	// Convert the latitudes
	// and longitudes
	// from degree to radians.
	lat1 = toRadians(lat1);
	long1 = toRadians(long1);
	lat2 = toRadians(lat2);
	long2 = toRadians(long2);

	// Haversine Formula
	long double dlong = long2 - long1;
	long double dlat = lat2 - lat1;

	long double ans = pow(sin(dlat / 2), 2) +
					  cos(lat1) * cos(lat2) *
					  pow(sin(dlong / 2), 2);

	ans = 2 * asin(sqrt(ans));

	// Radius of Earth in
	// Kilometers, R = 6371
	// Use R = 3956 for miles
	long double R = 3956;

	// Calculate the result
	ans = ans * R;

	return ans;
}

long double roundTenth(long double x) {
	// rounds to the tenths place
	return floor(x * 10 + 0.5) / 10;
}

void optionTwo(vector<trip> trips) {

	// find avg distance of all trips

	long double sumDist = 0;
	long double dist = 0;

	// trip with highest distance
	trip tripMax = trips.at(0);
	long double maxDist = 0;

	// finds maximum distance traveled and total distance traveled
	for (int index = 0; index < trips.size(); index++) {

		dist = distance(trips.at(index).getStartLat(), trips.at(index).getStartLng(),
						trips.at(index).getEndLat(), trips.at(index).getEndLng());

		// finds max distance traveled
		if (maxDist < dist) {
			maxDist = roundTenth(dist);
			tripMax = trips.at(index);
		}

		// adds distance to total distance traveled
		sumDist += dist;
    }

	// rounds numbers
	sumDist = round(sumDist);

	// round to tenths place
	long double avgDist = roundTenth(sumDist / trips.size());

	cout << "   Total # of miles traveled: "<< sumDist <<"\n" <<
			"   Average length of trips in miles:  "<< avgDist << "\n" <<
			"   \n" <<
			"   Longest trip information below: \n" <<
			"   -------------------------------\n" <<
			"   Trip ID: "<< tripMax.getRideID() <<"\n" <<
			"   Trip start location: " << tripMax.getStartStationName() << "\n" <<
			"   Trip end location: " << tripMax.getEndStationName() << "\n" <<
			"   Trip distance in miles: "<< maxDist << endl;
}

void optionThree(vector<trip> trips) {

	double totalMembers = 0;

	// finds total number of members
	for (int index = 0; index < trips.size(); index++) {

		if (trips.at(index).getMember()) {
			totalMembers += 1;
		}
    }

	// total number of casual members (all remaining trips)
	double totalCasual = trips.size() - totalMembers;

	// converts to percent
	double percentMembers = roundTenth(totalMembers / (double)trips.size() * 100.0);
	double percentCasual = roundTenth(totalCasual / (double)trips.size() * 100.0);

	string sPercentMembers = to_string(percentMembers);
	string sPercentCasual = to_string(percentCasual);

	// removes extraneous digits bc after rounding double looks like 4.100000 ... when printed
	sPercentCasual = sPercentCasual.erase(4, sPercentCasual.size() -4);
	sPercentMembers = sPercentMembers.erase(4, sPercentMembers.size() -4);

	cout << "Casual Rider Percentage: " << sPercentCasual << "% " << endl;
	cout << "Member Rider Percentage: " << sPercentMembers << "% " << endl;

}

void optionFour (vector<trip> trips) {

	double totalRidesWeekends = 0;
	double totalRidesWeekdays = 0;

	cout << "Select type of display: \n" <<
			"      1. Counts of rides per hour in the day\n" <<
			"      2. Proportional 50 column graph with @ for weekday and + for weekend\n" <<
			"   Your selection-->\n";

	int option = 0;
	cin >> option;

	// size is 24 bc there are 24 hours in a day
	vector<double> ridesPerHourWeekend(24,0);
	vector<double> ridesPerHourWeekday(24,0);

	for (int index = 0; index < trips.size(); index++) {

		// finds if trip is on weekend or weekdau
		int indexSpace = trips.at(index).getStartTime().find(" ");

		string dateNoTime = trips.at(index).getStartTime().substr(0, indexSpace);
		bool weekend = isWeekend(dateNoTime);

		// finds the hour the trip was made
		string timeNoDate = trips.at(index).getStartTime().erase(0, indexSpace + 1);

		int indexColon = timeNoDate.find(":");

		int hour = stoi(timeNoDate.substr(0, indexColon));

		if (weekend) {
			ridesPerHourWeekend.at(hour) += 1;
			totalRidesWeekends += 1;
		}
		else {
			ridesPerHourWeekday.at(hour) += 1;
			totalRidesWeekdays += 1;
		}
	}

	int largestNumberOfRides = 0;

	// finds largest number of rides for weekend
	for (int index = 0; index < ridesPerHourWeekend.size(); index++) {
		if (largestNumberOfRides < ridesPerHourWeekend.at(index)) {
			largestNumberOfRides = ridesPerHourWeekend.at(index);
		}
	}

	// finds largest number of rides for weekdays
	for (int index = 0; index < ridesPerHourWeekday.size(); index ++) {
		if (largestNumberOfRides < ridesPerHourWeekday.at(index)) {
			largestNumberOfRides = ridesPerHourWeekday.at(index);
		}
	}

	// different ways to print this data
	if (option == 1) { // subopt 1

		cout << "    LargestNumberOfRides is: " << largestNumberOfRides << endl;

		cout << "   Rides per hour for weekday and weekend:\n";
		for (int index = 0; index < ridesPerHourWeekday.size(); index++) {

			cout << "   " << index << ": " << ridesPerHourWeekday.at(index) << "    " << ridesPerHourWeekend.at(index) << endl;

		}
	} else if (option == 2) { // subopt 2

		for (int index = 0; index < ridesPerHourWeekday.size(); index++) {
			cout << "   " << index << ": ";
			cout << string(ridesPerHourWeekday.at(index) * 50.0 / largestNumberOfRides, '@') << endl;
			cout << string(ridesPerHourWeekend.at(index) * 50.0 / largestNumberOfRides, '+') << endl;

		}

	}

}

void optionFive (vector<trip> trips){
	// 41.87625245639088, -87.65214869055426
	// The Letterman Chicago - West Loop Apartments
	// I want to get an apartment here so i don't have to commute 2+hrs one way anymore
	// I am looking for a roommate to help make it cheaper

	cout << "Input latitude and longitude of the home: ";

	// get input long and lat
	long double latHouse;
	long double lngHouse;

	cin >> latHouse;
	cin >> lngHouse;

	cout << "   You entered: " << roundTenth(latHouse)<< " for latitude and " << roundTenth(lngHouse) << " for longitude\n";

	long double distToDivvyStart = 0;
	long double distToDivvyEnd = 0;
	// there are always 2 stations in every trip a start and end check them both
	// startDivvy checks if it was the starting station in the trip or ending station
	bool startDivvy = false;
	trip nearestDivvy;

	long double leastDistance = distance(trips.at(0).getEndLat(), trips.at(0).getEndLng(), latHouse, lngHouse);

	for (int index = 0; index < trips.size(); index++) {

		//there are 2 divvy stations in evry trip (start and end)
		distToDivvyEnd = distance(trips.at(index).getEndLat(), trips.at(index).getEndLng(), latHouse, lngHouse);
        distToDivvyStart = distance(trips.at(index).getStartLat(), trips.at(index).getStartLng(), latHouse, lngHouse);

		if (distToDivvyStart < leastDistance) {
            leastDistance = distToDivvyStart;
			nearestDivvy = trips.at(index);
			startDivvy = true;
        }

		if (distToDivvyEnd < leastDistance) {
            leastDistance = distToDivvyEnd;
			nearestDivvy = trips.at(index);
			startDivvy = false;
        }

	}
	// google maps orders divvy bike stations by time(including traffic and  squiggly roads)
	// this finds exact distance so the answers may vary slightly

	// print final answer based on if it was the start station or end station
	if (startDivvy) {
		cout << "   Closest Divvy station is: " << nearestDivvy.getStartStationName() << " at " << nearestDivvy.getStartLat()
		<< ", " << nearestDivvy.getStartLng() << ", " << roundTenth(leastDistance) << " miles away." << endl;
	} else {
		cout << "   Closest Divvy station is: " << nearestDivvy.getEndStationName() << " at " << nearestDivvy.getEndLat()
		<< ", " << nearestDivvy.getEndLng() << ", " << roundTenth(leastDistance) << " miles away." << endl;
	}

}