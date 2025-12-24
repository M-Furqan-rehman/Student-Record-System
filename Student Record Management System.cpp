#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

// ====================== Student Class ======================
class Student {
private:
    int id;
    string name;
    int age;
    string email;
    string course;

public:
    Student(int studentId, const string& studentName, int studentAge,
            const string& studentEmail, const string& studentCourse)
        : id(studentId), name(studentName), age(studentAge),
          email(studentEmail), course(studentCourse) {}

    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getEmail() const { return email; }
    string getCourse() const { return course; }

    void setName(const string& newName) { name = newName; }
    void setAge(int newAge) { age = newAge; }
    void setEmail(const string& newEmail) { email = newEmail; }
    void setCourse(const string& newCourse) { course = newCourse; }

    void display() const {
        cout << left << setw(5) << id
             << setw(20) << name
             << setw(5) << age
             << setw(25) << email
             << setw(15) << course << endl;
    }
};

// ================= Student Management System =================
class StudentManagementSystem {
private:
    vector<Student> students;
    int nextId;

    static void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    StudentManagementSystem() : nextId(1) {}

    // ================= FILE HANDLING =================
    void saveToFile() const {
        ofstream file("students.txt");
        for (const auto& s : students) {
            file << s.getId() << ","
                 << s.getName() << ","
                 << s.getAge() << ","
                 << s.getEmail() << ","
                 << s.getCourse() << "\n";
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file("students.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, email, course;
            int id, age;
            char comma;

            ss >> id >> comma;
            getline(ss, name, ',');
            ss >> age >> comma;
            getline(ss, email, ',');
            getline(ss, course);

            students.emplace_back(id, name, age, email, course);
            nextId = max(nextId, id + 1);
        }
        file.close();
    }

    // ================= ADD =================
    void addStudent() {
        string name, email, course;
        int age;

        cout << "Enter student name: ";
        clearInputBuffer();
        getline(cin, name);

        cout << "Enter age: ";
        cin >> age;
        clearInputBuffer();

        cout << "Enter email: ";
        getline(cin, email);

        cout << "Enter course: ";
        getline(cin, course);

        students.emplace_back(nextId++, name, age, email, course);
        cout << "Student added successfully!\n";
    }

    // ================= DISPLAY =================
    void displayAllStudents() const {
        if (students.empty()) {
            cout << "No records found.\n";
            return;
        }

        cout << "\nID   Name                 Age  Email                     Course\n";
        cout << "------------------------------------------------------------------\n";
        for (const auto& s : students)
            s.display();
    }

    // ================= SORT =================
    void sortById() {
        sort(students.begin(), students.end(),
             [](const Student& a, const Student& b) {
                 return a.getId() < b.getId();
             });
        cout << "Students sorted by ID successfully.\n";
    }

    // ================= BINARY SEARCH =================
    void binarySearchStudent() {
        if (students.empty()) {
            cout << "No data available.\n";
            return;
        }

        sortById(); // Required before binary search

        int id;
        cout << "Enter student ID to search: ";
        cin >> id;

        int low = 0, high = students.size() - 1;
        while (low <= high) {
            int mid = (low + high) / 2;

            if (students[mid].getId() == id) {
                cout << "\nStudent Found:\n";
                students[mid].display();
                return;
            }
            else if (students[mid].getId() < id)
                low = mid + 1;
            else
                high = mid - 1;
        }

        cout << "Student not found.\n";
    }

    // ================= UPDATE =================
    void updateStudent() {
        int id;
        cout << "Enter student ID: ";
        cin >> id;

        for (auto& s : students) {
            if (s.getId() == id) {
                string name, email, course;
                int age;

                clearInputBuffer();
                cout << "New name (Enter to skip): ";
                getline(cin, name);
                if (!name.empty()) s.setName(name);

                cout << "New age (0 to skip): ";
                cin >> age;
                if (age > 0) s.setAge(age);
                clearInputBuffer();

                cout << "New email (Enter to skip): ";
                getline(cin, email);
                if (!email.empty()) s.setEmail(email);

                cout << "New course (Enter to skip): ";
                getline(cin, course);
                if (!course.empty()) s.setCourse(course);

                cout << "Record updated successfully.\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    // ================= DELETE =================
    void deleteStudent() {
        int id;
        cout << "Enter student ID: ";
        cin >> id;

        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getId() == id) {
                students.erase(it);
                cout << "Record deleted successfully.\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    // ================= MENU =================
    void displayMenu() const {
        cout << "\n===== STUDENT RECORD MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Sort Students by ID\n";
        cout << "4. Binary Search Student\n";
        cout << "5. Update Student\n";
        cout << "6. Delete Student\n";
        cout << "7. Exit\n";
        cout << "Choose: ";
    }
};

// =========================== MAIN ===========================
int main() {
    StudentManagementSystem system;
    system.loadFromFile();

    int choice;
    do {
        system.displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: system.addStudent(); break;
            case 2: system.displayAllStudents(); break;
            case 3: system.sortById(); break;
            case 4: system.binarySearchStudent(); break;
            case 5: system.updateStudent(); break;
            case 6: system.deleteStudent(); break;
            case 7:
                system.saveToFile();
                cout << "Data saved. Goodbye!\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
    } while (choice != 7);

    return 0;
}
