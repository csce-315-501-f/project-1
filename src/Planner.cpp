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
	cout << "1. View Course" << endl;
	cout << "2. Add New Course" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter choice: " << flush;
	cin >> choice;
	return choice;
}

int selectcourse(Database& db) {
	// db code to list all courses and fill courses vector
    Parser p;
    vector< vector<string> > rows = db.getAllRows("Courses");
    vector<int> mapping;
	vector<string> courses;
    for(int i = 0; i < rows.size(); ++i) {
	mapping.push_back(atoi(rows[i][0].c_str()));
	courses.push_back(rows[i][1]);
    }
    //vector<ParseNode*> statements;
    //statements = p.parse(showrel("Courses"));
    //execute(statements[0],db);
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
	return mapping[c-1];
}

int coursemenu() {
	int choice;
	cout << "1. View All Assignments" << endl;
	cout << "2. View Assignments By Category" << endl;
	cout << "3. Add Assignment" << endl;
	cout << "4. Remove Assignment" << endl;
	cout << "5. Add Category" << endl;
	cout << "6. Remove Category" << endl;
	cout << "7. \033[1;31mDelete Course\033[0m" << endl;
	cout << "8. Back" << endl;
	cout << "Enter choice: " << flush;
	cin >> choice;
	if (choice == 8)
		return -1;
	return choice;

}

#define YES 1
#define NO 2
int confirm() {
	int c = 0;
	while (c != 1 && c != 2) {
		cout << "1. Yes" << endl;
		cout << "2. No" << endl;
		cout << "Confirm: " << flush;
		cin >> c;
	}
	return c;
}


int l = 0;
void addassignment(Database& db, int course) {
	clear();
    Parser p;
    vector<ParseNode*> statements;
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
	int c = confirm();
		if (c == YES) {
		// db code to add assignment
        statements = p.parse(newassignment(l++,name,duedate));
        execute(statements[0],db);
	
		cout << "Added." << endl;
		string temp;
		cout << "Press Enter key to continue..." << flush;
		fflush(stdin);
		getline(cin,temp);
	}

}

int assignments(Database& db, int course) {
    Parser p;
    vector<ParseNode*> statements;
	// db code to list all assignments in a course and fill assigns vector
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

	int opt = 0;
    Parser p;
    vector<ParseNode*> statements;
	while (opt != -1) {
		clear();
		// db code to show course info
	//vector<vector<string> > vs = db.getRowsWhere("Courses","Title","chris");
	//for(int i = 0; i < vs.size(); ++i) {
		//for (int j = 0; j < vs[i].size(); ++j)
			//cout << vs[i][j] << endl;
		//cout << endl;
	//}
        statements = p.parse(selectcourse(course));
        execute(statements[0], db);
        //execute(statements[1], db);
	//vector<vector<string> > val = db.getRowsWhere("Courses","id",to_string(course));
	//cout << val[0][1] << endl << endl;
	//cout << "Instructor: " << val[0][2] << endl;
	//cout << "Days: " << val[0][3] << endl << endl;
        //statements = p.parse("a<-SELECT(id == \"" + to_string(course) + "\") Courses; SHOW a;");
        //execute(statements[0], db);
		opt = coursemenu();
		switch (opt) {
		case 1:
			// view all assignments
			break;
		case 2:
			// view assigns by category
			break;
		case 3:
			addassignment(db,course);
			break;
		case 4:
			removeassign(db,course);
			break;
		case 5:
			// add category
			break;
		case 6:
			// delete category	
			break;
		case 7:
			// db code to delete course	
			opt = -1;
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

int k = 0;
void addcourse(Database& db) {
	clear();
    Parser p;
    vector<ParseNode*> statements;// =p.parse(s);
	cout << "Add Course" << endl << endl;
	string name;
	string instructor;
	string days;
	cout << "Enter course name: " << flush;
	fflush(stdin);
	getline(cin,name);
	cout << "Enter course instructor: " << flush; 
	fflush(stdin);
	getline(cin,instructor);
	cout << "Enter course days: " << flush;
	fflush(stdin);
	getline(cin,days);
	cout << "Add course '" << name << "' with instructor '" << instructor <<
		"' on '" << days << "'?" << endl;
	int c = confirm();
	if (c == YES) {
		// db code to add course
        statements = p.parse(newcourse(k++,name, instructor, days));
        execute(statements[0], db);
	
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
	ifstream assignFile("Assignments.db");
	ifstream courseFile("Courses.db");

	// db code to initilize database with dbfiles
	Parser p;
	vector<ParseNode*> statements;
	if (assignFile) {
		statements = p.parse(open("Assignments"));
		execute(statements[0],db);
	}
	else {
		statements = p.parse(createassignment());
		execute(statements[0],db);
	}
	if (courseFile) {
		statements = p.parse(open("Courses"));
		execute(statements[0],db);
	}
	else {
		statements = p.parse(createcourse());
		execute(statements[0],db);
	}
	
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
		case 3:
			// db code to save database
			break;
		}
	}
    
    return 0;
}
