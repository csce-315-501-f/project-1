//  Integration.cpp

#include <cstdlib>
#include <iostream>
#include <string>
#include "Integration.h"

vector<string> courses;

//while (s != "QUIT") {
        //cout << "Enter code to parse: " << flush;
        //getline(cin, s);
        //vector<ParseNode*> statements = p.parse(s);
        //for (int i = 0; i< statements.size(); i++) {
            //execute(statements[i], db);
        //}
    //}
	
void clear() {
	system("clear");
}

void header() {
	clear();
	cout << "Assignment Planner" << endl << endl;
}

int mainmenu() {
	int choice;
	cout << "1. Open Course" << endl;
	cout << "2. Add New Course" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter choice: " << flush;
	cin >> choice;
	return choice;
}

int selectcourse(Database& db) {
	// db code to list all courses and fill courses vector
	vector<string> courses;
	int i, c;
	clear();
	cout << "Course List" << endl << endl;
	for(i = 0; i < courses.size(); ++i)
		cout << i+1 << ". " << courses[i] << endl;
	cout << i+1 << ". Back" << endl;
	cin >> c;
	if (c == i+1) return -1;
	return c;
}

void showcourse(Database& db, int c) {
	cout << "here " << endl;
}

void opencourse(Database& db) {
	int c;
	
	while (c >= 0) {
		c = selectcourse(db);
		if (c >= 0) showcourse(db,c);
	}
}

void addcourse(Database& db) {
	clear();
	cout << "Add Course" << endl << endl;
	string name;
	string instructor;
	cout << "Enter course name: " << flush;
	cin >> name;
	cout << "Enter course instructor: " << flush; 
	cin >> instructor;
	cout << "Add course '" << name << "' with instructor '" << instructor << "'?" << endl;
	int c = 0;
	while (c <= 0 && c > 2) {
		cout << "1. Yes" << endl;
		cout << "2. No" << endl;
		cout << "Confirm: " << endl;
		cin >> c;
	}
	if (c == 1) {
		// db code to add course
	
	}
}

int main()
{
	// declare initial parts
        Database db;
	int c = 0;

	while (c != 3) {
		header();
		c = mainmenu();
		switch (c) {
		case 1:
			opencourse(db);
			break;
		case 2:
			addcourse(db);
			break;
		}
	}
    
    return 0;
}
