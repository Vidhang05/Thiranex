#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <cctype>

using namespace std;

// STEP 2: Design Structure - Create a class named "Student"
class Student {
private:
    int rollNo;
    char name[50];
    char course[50];
    int age;


public:
    // Default constructor
    Student() {
        rollNo = 0;
        age = 0;
        strcpy(name, "");
        strcpy(course, "");
    }

    // Setter for roll number
    void setRollNo(int r) {
        rollNo = r;
    }

    // Setter for name
    void setName(const char* n) {
        strncpy(name, n, 49);
        name[49] = '\0';
    }

    // Setter for age
    void setAge(int a) {
        age = a;
    }

    // Setter for course
    void setCourse(const char* c) {
        strncpy(course, c, 49);
        course[49] = '\0';
    }

    // Input only name with validation
    void inputName() {
        bool validName;
        do {
            cout << "Enter Name: ";
            cin.getline(name, 50);
            validName = true;
            if (strlen(name) == 0) {
                validName = false;
            } else {
                for (size_t i = 0; i < strlen(name); ++i) {
                    if (!isalpha(name[i]) && name[i] != ' ') {
                        validName = false;
                        break;
                    }
                }
            }
            if (!validName) {
                cout << "Invalid input. Name should only contain alphabets and spaces.\n";
            }
        } while (!validName);
    }

    // Input only age with validation
    void inputAge() {
        cout << "Enter Age: ";
        while (!(cin >> age) || age <= 0) {
            cout << "Invalid input. Please enter a valid positive integer for Age: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Input only course
    void inputCourse() {
        cout << "Enter Course: ";
        cin.getline(course, 50);
    }

    // Input function to get roll number with validation
    int inputRollNo() {
        cout << "Enter Roll Number: ";
        // Input validation for roll number
        while (!(cin >> rollNo)) {
            cout << "Invalid input. Please enter a valid integer for Roll Number: ";
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the newline character from buffer
        return rollNo;
    }

    // Input function to get remaining details (name, age, course)
    void inputDetails() {
        bool validName;
        do {
            cout << "Enter Name: ";
            cin.getline(name, 50);
            validName = true;
            
            // Check if name is empty
            if (strlen(name) == 0) {
                validName = false;
            } else {
                // Check each character for alphabet or space
                for (size_t i = 0; i < strlen(name); ++i) {
                    if (!isalpha(name[i]) && name[i] != ' ') {
                        validName = false;
                        break;
                    }
                }
            }
            
            if (!validName) {
                cout << "Invalid input. Name should only contain alphabets and spaces.\n";
            }
        } while (!validName);

        cout << "Enter Age: ";
        // Input validation for age
        while (!(cin >> age) || age <= 0) {
            cout << "Invalid input. Please enter a valid positive integer for Age: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the newline character from buffer

        cout << "Enter Course: ";
        cin.getline(course, 50);
    }

    // Full input function (used by update)
    void input() {
        inputRollNo();
        inputDetails();
    }

    // Display function to show student details
    void display() const {
        cout << "Roll No : " << rollNo << "\n"
             << "Name    : " << name << "\n"
             << "Age     : " << age << "\n"
             << "Course  : " << course << "\n"
             << "-------------------------\n";
    }

    // Getter for roll number to use in search/update/delete
    int getRollNo() const {
        return rollNo;
    }

    // Write student data to a text file stream (format: rollNo|name|age|course)
    void writeToFile(ofstream& out) const {
        out << rollNo << "|" << name << "|" << age << "|" << course << "\n";
    }

    // Read student data from a text file stream
    bool readFromFile(ifstream& in) {
        string line;
        if (!getline(in, line)) return false;
        if (line.empty()) return false;

        // Parse the line using '|' as delimiter
        size_t pos1 = line.find('|');
        if (pos1 == string::npos) return false;

        size_t pos2 = line.find('|', pos1 + 1);
        if (pos2 == string::npos) return false;

        size_t pos3 = line.find('|', pos2 + 1);
        if (pos3 == string::npos) return false;

        rollNo = stoi(line.substr(0, pos1));
        strncpy(name, line.substr(pos1 + 1, pos2 - pos1 - 1).c_str(), 49);
        name[49] = '\0';
        age = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        strncpy(course, line.substr(pos3 + 1).c_str(), 49);
        course[49] = '\0';
        return true;
    }
};

// Global constant for the text file name
const string FILE_NAME = "students.txt";

// Function prototypes
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
bool isRollNoDuplicate(int roll);

int main() {
    int choice;

    // STEP 5: Menu-Driven Program
    do {
        cout << "\n=== Student Management System ===\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        
        // Input validation for choice
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 6: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: cout << "Exiting program. Goodbye!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}

// STEP 7: Extra Improvements - Check if a roll number already exists
bool isRollNoDuplicate(int roll) {
    ifstream inFile(FILE_NAME);
    if (!inFile) return false; // File doesn't exist yet, so no duplicates

    Student s;
    // Read through the file to check for matching roll numbers
    while (s.readFromFile(inFile)) {
        if (s.getRollNo() == roll) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

// STEP 4.1: Add Student
void addStudent() {
    Student s;

    // Ask for roll number FIRST and check for duplicates immediately
    int roll;
    while (true) {
        roll = s.inputRollNo();
        if (isRollNoDuplicate(roll)) {
            cout << "Error: A student with Roll Number " << roll << " already exists! Please enter a different Roll Number.\n";
        } else {
            break; // Roll number is unique, proceed
        }
    }

    // Now ask for the remaining details (name, age, course)
    s.inputDetails();

    // STEP 3: File Handling - Append to text file for readable and permanent storage
    ofstream outFile(FILE_NAME, ios::app);
    if (!outFile) {
        cout << "Error: Could not open file for writing.\n";
        return;
    }

    // Write the Student data as a text line
    s.writeToFile(outFile);
    outFile.close();
    cout << "Student record added successfully!\n";
}

// STEP 4.2: Display All Students
void displayAllStudents() {
    ifstream inFile(FILE_NAME);
    if (!inFile) {
        cout << "No records found or unable to open file.\n";
        return;
    }

    Student s;
    bool found = false;
    cout << "\n--- All Student Records ---\n";
    
    // Read and display until the end of the file
    while (s.readFromFile(inFile)) {
        s.display();
        found = true;
    }
    inFile.close();

    if (!found) {
        cout << "No student records available.\n";
    }
}

// STEP 4.3: Search Student
void searchStudent() {
    int rollToSearch;
    cout << "Enter Roll Number to search: ";
    cin >> rollToSearch;

    ifstream inFile(FILE_NAME);
    if (!inFile) {
        cout << "No records found or unable to open file.\n";
        return;
    }

    Student s;
    bool found = false;
    // Read records one by one and compare roll numbers
    while (s.readFromFile(inFile)) {
        if (s.getRollNo() == rollToSearch) {
            cout << "\n--- Student Found ---\n";
            s.display();
            found = true;
            break; // Stop searching once found
        }
    }
    inFile.close();

    if (!found) {
        cout << "Student with Roll Number " << rollToSearch << " not found.\n";
    }
}

// STEP 4.4: Update Student
void updateStudent() {
    int rollToUpdate;
    cout << "Enter Roll Number to update: ";
    cin >> rollToUpdate;

    ifstream inFile(FILE_NAME);
    if (!inFile) {
        cout << "No records found or unable to open file.\n";
        return;
    }

    // Read all students into memory
    Student students[100];
    int count = 0;
    while (students[count].readFromFile(inFile) && count < 100) {
        count++;
    }
    inFile.close();

    bool found = false;
    for (int i = 0; i < count; i++) {
        if (students[i].getRollNo() == rollToUpdate) {
            cout << "Current Record Details:\n";
            students[i].display();

            // Show sub-menu to choose what to update
            int updateChoice;
            cout << "\nWhat do you want to update?\n";
            cout << "1. Roll Number\n";
            cout << "2. Name\n";
            cout << "3. Age\n";
            cout << "4. Course\n";
            cout << "5. All Details\n";
            cout << "6. Cancel\n";
            cout << "Enter your choice: ";
            while (!(cin >> updateChoice) || updateChoice < 1 || updateChoice > 6) {
                cout << "Invalid choice. Please enter a number between 1 and 6: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (updateChoice == 6) {
                cout << "Update cancelled.\n";
                return;
            }

            switch (updateChoice) {
                case 1: {
                    int newRoll;
                    while (true) {
                        newRoll = students[i].inputRollNo();
                        if (newRoll != rollToUpdate && isRollNoDuplicate(newRoll)) {
                            cout << "Error: The new Roll Number already belongs to another student. Try again.\n";
                        } else {
                            break;
                        }
                    }
                    break;
                }
                case 2:
                    students[i].inputName();
                    break;
                case 3:
                    students[i].inputAge();
                    break;
                case 4:
                    students[i].inputCourse();
                    break;
                case 5: {
                    int newRoll;
                    while (true) {
                        newRoll = students[i].inputRollNo();
                        if (newRoll != rollToUpdate && isRollNoDuplicate(newRoll)) {
                            cout << "Error: The new Roll Number already belongs to another student. Try again.\n";
                        } else {
                            break;
                        }
                    }
                    students[i].inputDetails();
                    break;
                }
            }

            // Rewrite the entire file with updated data
            ofstream outFile(FILE_NAME);
            for (int j = 0; j < count; j++) {
                students[j].writeToFile(outFile);
            }
            outFile.close();

            found = true;
            cout << "Student record updated successfully!\n";
            break;
        }
    }

    if (!found) {
        cout << "Student with Roll Number " << rollToUpdate << " not found.\n";
    }
}

// STEP 4.5: Delete Student
void deleteStudent() {
    int rollToDelete;
    cout << "Enter Roll Number to delete: ";
    cin >> rollToDelete;

    ifstream inFile(FILE_NAME);
    if (!inFile) {
        cout << "No records found or unable to open file.\n";
        return;
    }

    // Create a temporary file to store non-deleted records
    ofstream tempFile("temp.txt");
    if (!tempFile) {
        cout << "Error: Could not create temporary file.\n";
        inFile.close();
        return;
    }

    Student s;
    bool found = false;
    while (s.readFromFile(inFile)) {
        if (s.getRollNo() != rollToDelete) {
            // Write to temporary file only if it's NOT the record to delete
            s.writeToFile(tempFile);
        } else {
            found = true; // We found the record and intentionally skipped writing it
        }
    }

    inFile.close();
    tempFile.close();

    // Remove old file and rename temporary file to the original file name
    if (found) {
        remove(FILE_NAME.c_str());
        rename("temp.txt", FILE_NAME.c_str());
        cout << "Student record deleted successfully!\n";
    } else {
        remove("temp.txt"); // Cleanup the unused temp file
        cout << "Student with Roll Number " << rollToDelete << " not found.\n";
    }
}
