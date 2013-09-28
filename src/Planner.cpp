//  Integration.cpp

#include <cstdlib>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <string>
#include "Integration.h"

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
	if (courses.size() == 0) {
		cout << "No courses in database." << endl;
	}
	cout << i+1 << ". Back" << endl;
	cin >> c;
	if (c == i+1) return -1;
	return c;
}

int coursemenu() {
	int choice;
	cout << "1. Add Assignment" << endl;
	cout << "2. Remove Assignment" << endl;
	cout << "3. Back" << endl;
	cout << "Enter choice: " << flush;
	cin >> choice;
	return choice;

}

void addassignment(Database& db, int course) {
	clear();
	cout << "Add Assignment" << endl << endl;
	string name;
	string duedate;
	cout << "Enter assignment name: " << flush;
	fflush(stdin);
	getline(cin,name);
	cout << "Enter due date [mm/dd/yy]: " << flush; 
	fflush(stdin);
	getline(cin,duedate);
	cout << "Add assignment '" << name << "' due on '" << duedate << "'?" << endl;
	int c = 0;
	while (c != 1 && c != 2) {
		cout << "1. Yes" << endl;
		cout << "2. No" << endl;
		cout << "Confirm: " << flush;
		cin >> c;
	}
	if (c == 1) {
		// db code to add assignment
	
		cout << "Added." << endl;
		string temp;
		cout << "Press Enter key to continue..." << flush;
		fflush(stdin);
		getline(cin,temp);
	}

}

int assignments(Database& db, int course) {
	// db code to list all assignments in a course and fill courses vector
	vector<string> assigns;
	int i, c;
	clear();
	cout << "Assignment List" << endl << endl;
	for(i = 0; i < assigns.size(); ++i)
		cout << i+1 << ". " << assigns[i] << endl;
	if (assigns.size() == 0) {
		cout << "No assignments in database." << endl;
	}
	cout << i+1 << ". Back" << endl;
	cin >> c;
	if (c == i+1) return -1;
	return c;
}

void removeassign(Database& db, int course) {
	int assignment;

	while (assignment >= 0) {
		assignment = assignments(db, course);
		if (assignment >= 0) {
			// db code to remove assignment
			
		}
	}
	
}

void showcourse(Database& db, int course) {
	// db code to show course info

	int opt = 0;
	while (opt != -1) {
		opt = coursemenu();
		switch (opt) {
		case 1:
			addassignment(db,course);
			break;
		case 2:
			removeassign(db,course);
			break;
		}
	}
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
	fflush(stdin);
	getline(cin,name);
	cout << "Enter course instructor: " << flush; 
	fflush(stdin);
	getline(cin,instructor);
	cout << "Add course '" << name << "' with instructor '" << instructor << "'?" << endl;
	int c = 0;
	while (c != 1 && c != 2) {
		cout << "1. Yes" << endl;
		cout << "2. No" << endl;
		cout << "Confirm: " << flush;
		cin >> c;
	}
	if (c == 1) {
		// db code to add course
	
		cout << "Added." << endl;
		string temp;
		cout << "Press Enter key to continue..." << flush;
		fflush(stdin);
		getline(cin,temp);
	}
}

int main()
{
        Database db;
	vector<string> dbfiles;
	char pwd[128];
	getcwd(pwd,128);
	DIR* dir = opendir(pwd);
	if (dir) {
		dirent* fil;
		while ( fil = readdir(dir)) {
			if (strstr(fil->d_name,".db"))
				dbfiles.push_back(fil->d_name);
		}
		closedir(dir);
	}

	// db code to initilize database with dbfiles

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
