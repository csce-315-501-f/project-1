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
	

vector<string> keyToId;
string currentCourse;
int courseId = 0;
int assignmentId = 0;
int categoryId = 0;



vector<string> explode(string str, char delim) {
	vector<string> strings;
	stringstream buffer(str);
	string temp;
	while (std::getline(buffer, temp, delim)) {
		strings.push_back(temp);
	}
	return strings;
}

vector< vector<string> > selection(Database &db, string selectQuery) {
	vector< vector<string> > rows;
	stringstream ss;

	Parser p;
	vector<ParseNode*> parseStatements;
	parseStatements = p.parse(selectQuery);

	streambuf* oldCoutStreamBuf = cout.rdbuf(ss.rdbuf());
    execute(parseStatements[0],db);
	cout.rdbuf( oldCoutStreamBuf );

	vector<string> strings = explode(ss.str(),'\n');
	bool done = false;
	int itr = 0;
	while (!done) {
		if (strings[0].substr(0,2) == "id") {
			done = true;
		}
		strings.erase(strings.begin()+itr);

	}

	for (int i = 0; i < strings.size(); ++i) {
		rows.push_back(explode(strings[i],'\t'));
	}

	return rows;
}

void initId(Database& db) {
	// vector< vector<string> > courses = selection(db,showrel("Courses"));
	vector< vector<string> > courses = db.getAllRows("Courses");
	for (int i = 0; i < courses.size(); ++i) {
		int id;
		istringstream ( courses[i][0] ) >> id;
		if (id > courseId) courseId = id;
	}

	// vector< vector<string> > assignments = selection(db,showrel("Assignemnts"));
	vector< vector<string> > assignments = db.getAllRows("Assignments");
	for (int i = 0; i < assignments.size(); ++i) {
		int id;
		istringstream ( assignments[i][0] ) >> id;
		if (id > assignmentId) assignmentId = id;
	}


	// vector< vector<string> > courses = selection(db,showrel("AssignmentCategories"));



}








void clear() {
	system("clear");
}

void header() {
	clear();
	cout << "Assignment Planner" << endl << endl;
}

int mainmenu() {
	int choice;
	cout << "1. View Courses" << endl;
	cout << "2. Add New Course" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter choice: " << flush;
	cin >> choice;
	return choice;
}

int selectcourse(Database& db) {
	// db code to list all courses and fill courses vector
    Parser p;
    // vector< vector<string> > rows = db.getAllRows("Courses");
    vector<int> mapping;
	vector<string> courses;
    /*for(int i = 0; i < rows.size(); ++i) {
	mapping.push_back(atoi(rows[i][0].c_str()));
	courses.push_back(rows[i][1]);
    }*/

	vector< vector<string> > allCourses = selection(db,showrel("Courses"));
	// vector< vector<string> > allCourses = db.getAllRows("Courses");
	keyToId.clear();
	vector<string> temp(allCourses.size());
	keyToId = temp;
	for (int i = 0; i < allCourses.size(); ++i) {
		string id = allCourses[i][0];
		string name = allCourses[i][1];
		// string instructor = allCourses[i][2];
		// string days = allCourses[i][3];
		keyToId[i] = id;
		courses.push_back(name);
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
	cout << "Enter choice: " << flush;
	cin >> c;
	if (c == i+1) return -1;
	// return mapping[c-1];
	return atoi(keyToId[c-1].c_str());
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

void addassignment(Database& db, int course) {
	clear();
    Parser p;
    vector<ParseNode*> statements;
	cout << "Add Assignment" << endl << endl;
	string name;
	string duedate;
	// char name[256];
	// char duedate[256];
	cin.ignore(1,'\n');
	cout << "Enter assignment name: " << flush;
	fflush(stdin);
	getline(cin,name);
	// cin >> name;
	// cin.getline(name,256);
	cout << "Enter due date [mm/dd/yy]: " << flush; 
	fflush(stdin);
	getline(cin,duedate);
	// cin >> duedate;
	// cin.getline(duedate,256);
	cout << "Add assignment '" << name << "' due on '" << duedate << "'?" << endl;
	int c = confirm();
		if (c == YES) {
		// db code to add assignment
		assignmentId++;
        statements = p.parse(newassignment(assignmentId,name,duedate,currentCourse));
        execute(statements[0],db);
	
		cout << "Added." << endl;
		string temp;
		cin.ignore(1,'\n');
		cout << "Press Enter key to continue..." << flush;
		fflush(stdin);
		getline(cin,temp);
	}

}

int assignments(Database& db, int course) {
	vector<string> assignments;

	string query = "a<-SELECT(Course == \"" + currentCourse + "\") Assignments; SHOW a;";
	vector< vector<string> > allAssignments = selection(db,query);
	keyToId.clear();
	vector<string> temp(allAssignments.size());
	keyToId = temp;
	for (int i = 0; i < allAssignments.size(); ++i) {
		string id = allAssignments[i][0];
		string name = allAssignments[i][1];
		// string instructor = allAssignments[i][2];
		// string days = allAssignments[i][3];
		keyToId[i] = id;
		assignments.push_back(name);
	}

	int i, c;
	clear();
	cout << "Assignment List" << endl << endl;
	for(i = 0; i < assignments.size(); ++i)
		cout << i+1 << ". " << assignments[i] << endl;
	if (assignments.size() == 0) {
		cout << "No assignments in database." << endl;
	}
	cout << i+1 << ". Back" << endl;
	cout << "Enter choice: " << flush;
	cin >> c;
	if (c == i+1) return -1;
	// return mapping[c-1];
	return atoi(keyToId[c-1].c_str());
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

int coursemenu() {
	int choice;
	cout << "1. View All Assignments" << endl;
	// cout << "2. View Assignments By Category" << endl;
	cout << "2. Add Assignment" << endl;
	// cout << "4. Remove Assignment" << endl;
	// cout << "5. Add Category" << endl;
	// cout << "6. Remove Category" << endl;
	cout << "3. \033[1;31mDelete Course\033[0m" << endl;
	cout << "4. Back" << endl;
	cout << "Enter choice: " << flush;
	cin >> choice;
	if (choice == 4)
		return -1;
	return choice;

}

void showcourse(Database& db, int courseN) {

	int opt = 0;
	while (opt != -1) {
		clear();
		// db code to show course info
		string query = "a<-SELECT(id == \"" + to_string(courseN) + "\") Courses; SHOW a;";
		vector< vector<string> > course = selection(db,query);

		currentCourse = course[0][1];

		cout << "Course: " << course[0][1] << endl;
		cout << "Instructor: " << course[0][2] << endl;
		cout << "Days: " << course[0][3] << endl;
		cout << endl;



		opt = coursemenu();
		switch (opt) {
		case 1:
			// view all assignments
			assignments(db,courseN);
			break;
		// case 2:
		// 	// view assigns by category
		// 	break;
		case 2:
			addassignment(db,courseN);
			break;
		// case 4:
		// 	// removeassign(db,course);
		// 	break;
		// case 5:
		// 	// add category
		// 	break;
		case 3:
			// delete category	
			break;
		case 4:
			// db code to delete course	
			opt = -1;
			break;
		}
	}
}

void opencourse(Database& db) {
	int c;
	// vector<string> c;

	while (c >= 0) {
		c = selectcourse(db);
		if (c >= 0) showcourse(db,c);
		// if (!c.empty()) showcourse(db,c);
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
	cin.ignore(1,'\n');
	cout << "Enter course name: " << flush;
	// cin >> name;
	fflush(stdin);
	getline(cin,name);
	cout << "Enter course instructor: " << flush; 
	// cin >> instructor;
	fflush(stdin);
	getline(cin,instructor);
	cout << "Enter course days: " << flush;
	// cin >> days;
	fflush(stdin);
	getline(cin,days);
	cout << "Add course '" << name << "' with instructor '" << instructor <<
		"' on '" << days << "'?" << endl;
	int c = confirm();
	if (c == YES) {
		// db code to add course
		courseId++;
        statements = p.parse(newcourse(courseId,name, instructor, days));
        execute(statements[0], db);
	
		cout << "Added." << endl;
		string temp;
		cin.ignore(1,'\n');
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
	if (courseFile) {
		statements = p.parse(open("Courses"));
		execute(statements[0],db);
	}
	else {
		statements = p.parse(createcourse());
		execute(statements[0],db);
	}
	if (assignFile) {
		statements = p.parse(open("Assignments"));
		execute(statements[0],db);
	}
	else {
		statements = p.parse(createassignment());
		execute(statements[0],db);
	}
	
	initId(db);



	int c = 0;

	while (c != 3) {
		header();
		c = mainmenu();
		switch (c) {
		case 1:
			opencourse(db);
			// viewcourses(db);
			break;
		case 2:
			addcourse(db);
			break;
		case 3:
			// db code to save database
			// db.writeRelation("Courses");
			// db.writeRelation("Assignments");
			break;
		case 4: 
			initId(db);
			cout << courseId << endl;
			break;
		}
	}
    
    return 0;
}
