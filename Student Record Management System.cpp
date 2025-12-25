#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include <cctype>

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
    
    void displayCompact() const {
        cout << "ID: " << id << " | Name: " << name 
             << " | Age: " << age << " | Course: " << course << endl;
    }
    
    // For file saving
    string toFileString() const {
        return to_string(id) + "," + name + "," + to_string(age) + "," + email + "," + course;
    }
};

// ================= Student Management System =================
class StudentManagementSystem {
private:
    vector<Student> students;
    int nextId;

    // Make this private helper function static
    static void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    static string toLower(const string& str) {
        string lowerStr = str;
        transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }
    
    // Private helper function for sorting by ID
    void sortById() {
        sort(students.begin(), students.end(),
             [](const Student& a, const Student& b) {
                 return a.getId() < b.getId();
             });
    }

public:
    StudentManagementSystem() : nextId(1) {}

    // ================= FILE HANDLING =================
    void saveToFile() const {
        ofstream file("students.txt");
        if (!file) {
            cerr << "Error: Cannot save to file!\n";
            return;
        }
        
        for (const auto& s : students) {
            file << s.toFileString() << "\n";
        }
        file.close();
        cout << "\n✅ Data saved successfully to 'students.txt'!\n";
    }

    void loadFromFile() {
        ifstream file("students.txt");
        if (!file) {
            cout << "No existing data found. Starting fresh.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() == 5) {
                try {
                    int id = stoi(tokens[0]);
                    string name = tokens[1];
                    int age = stoi(tokens[2]);
                    string email = tokens[3];
                    string course = tokens[4];
                    
                    students.emplace_back(id, name, age, email, course);
                    nextId = max(nextId, id + 1);
                } catch (...) {
                    cerr << "Warning: Corrupted data line skipped: " << line << endl;
                }
            }
        }
        file.close();
        cout << "✅ Loaded " << students.size() << " student records.\n";
    }

    // ================= ADD STUDENT =================
    void addStudent() {
        string name, email, course;
        int age;

        cout << "\n" << string(50, '=') << "\n";
        cout << "         ADD NEW STUDENT\n";
        cout << string(50, '=') << "\n";
        
        clearInputBuffer(); 
        cout << "Enter student name: ";
        getline(cin, name);
        
        while (name.empty()) {
            cout << "❌ Name cannot be empty. Enter student name: ";
            getline(cin, name);
        }

        cout << "Enter age (15-80): ";
        while (!(cin >> age) || age < 15 || age > 80) {
            cout << "❌ Invalid age! Enter age (15-80): ";
            cin.clear();
            clearInputBuffer();
        }
        
        clearInputBuffer();
        cout << "Enter email: ";
        getline(cin, email);
        
        // Simple email validation
        if (email.find('@gamil.com') == string::npos) {
            cout << "⚠️  Warning: Email format might be invalid.\n";
        }

        cout << "Enter course: ";
        getline(cin, course);
        
        if (course.empty()) {
            course = "Not Specified";
        }

        students.emplace_back(nextId++, name, age, email, course);
        cout << "\n✅ Student added successfully! (ID: " << (nextId-1) << ")\n";
    }

    // ================= DISPLAY STUDENTS =================
    void displayAllStudents() const {
        if (students.empty()) {
            cout << "\n📭 No student records found.\n";
            return;
        }

        cout << "\n" << string(70, '=') << "\n";
        cout << "               STUDENT LIST (" << students.size() << " records)\n";
        cout << string(70, '=') << "\n";
        cout << left << setw(5) << "ID"
             << setw(20) << "Name"
             << setw(5) << "Age"
             << setw(25) << "Email"
             << setw(15) << "Course" << endl;
        cout << string(70, '-') << endl;
        
        for (const auto& s : students)
            s.display();
            
        cout << string(70, '=') << "\n";
    }

    // ================= LINEAR SEARCH =================
    void linearSearchStudent() {
        if (students.empty()) {
            cout << "\n📭 No data available for search.\n";
            return;
        }

        cout << "\n" << string(40, '=') << "\n";
        cout << "         LINEAR SEARCH\n";
        cout << string(40, '=') << "\n";
        cout << "1. Search by ID\n";
        cout << "2. Search by Name\n";
        cout << "3. Search by Course\n";
        cout << "4. Back to Search Menu\n";
        cout << string(40, '-') << "\n";
        cout << "Choose: ";
        
        int choice;
        cin >> choice;
        clearInputBuffer();

        switch(choice) {
            case 1: {
                int id;
                cout << "Enter student ID: ";
                cin >> id;
                
                bool found = false;
                for (const auto& s : students) {
                    if (s.getId() == id) {
                        cout << "\n✅ Student Found:\n";
                        cout << string(60, '-') << "\n";
                        s.display();
                        cout << string(60, '-') << "\n";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "❌ Student with ID " << id << " not found.\n";
                }
                break;
            }
            case 2: {
                string name;
                cout << "Enter student name (or part of name): ";
                getline(cin, name);
                
                string searchName = toLower(name);
                vector<Student> results;
                
                for (const auto& s : students) {
                    if (toLower(s.getName()).find(searchName) != string::npos) {
                        results.push_back(s);
                    }
                }
                
                if (!results.empty()) {
                    cout << "\n🔍 Found " << results.size() << " matching student(s):\n";
                    cout << string(60, '-') << "\n";
                    for (const auto& s : results) {
                        s.displayCompact();
                    }
                    cout << string(60, '-') << "\n";
                } else {
                    cout << "❌ No students found with name containing: " << name << "\n";
                }
                break;
            }
            case 3: {
                string course;
                cout << "Enter course name: ";
                getline(cin, course);
                
                string searchCourse = toLower(course);
                vector<Student> results;
                
                for (const auto& s : students) {
                    if (toLower(s.getCourse()).find(searchCourse) != string::npos) {
                        results.push_back(s);
                    }
                }
                
                if (!results.empty()) {
                    cout << "\n👨‍🎓 Found " << results.size() << " student(s) in course '" << course << "':\n";
                    cout << string(60, '-') << "\n";
                    for (const auto& s : results) {
                        s.displayCompact();
                    }
                    cout << string(60, '-') << "\n";
                } else {
                    cout << "❌ No students found in course: " << course << "\n";
                }
                break;
            }
            case 4:
                return;
            default:
                cout << "❌ Invalid choice!\n";
        }
    }

    // ================= BINARY SEARCH =================
    void binarySearchStudent() {
        if (students.empty()) {
            cout << "\n📭 No data available for search.\n";
            return;
        }

        // Check if already sorted by ID
        bool isSorted = true;
        for (size_t i = 1; i < students.size(); i++) {
            if (students[i].getId() < students[i-1].getId()) {
                isSorted = false;
                break;
            }
        }
        
        if (!isSorted) {
            cout << "\n⚠️  List is not sorted by ID!\n";
            cout << "Binary search requires sorted data.\n";
            cout << "1. Sort by ID first, then search\n";
            cout << "2. Use linear search instead\n";
            cout << "3. Cancel search\n";
            cout << "Choose: ";
            
            int choice;
            cin >> choice;
            
            if (choice == 1) {
                sortById(); // Now this function exists
                cout << "✅ List sorted by ID.\n";
            } else if (choice == 2) {
                linearSearchStudent();
                return;
            } else {
                return;
            }
        }

        int id;
        cout << "\nEnter student ID to search: ";
        cin >> id;

        int low = 0, high = students.size() - 1;
        int comparisons = 0;
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            comparisons++;
            
            if (students[mid].getId() == id) {
                cout << "\n✅ Student Found (Binary Search - " << comparisons << " comparisons):\n";
                cout << string(60, '=') << "\n";
                students[mid].display();
                cout << string(60, '=') << "\n";
                return;
            }
            else if (students[mid].getId() < id) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        cout << "❌ Student with ID " << id << " not found.\n";
    }

    // ================= SORTING MENU =================
    void sortMenu() {
        if (students.empty()) {
            cout << "\n📭 No students to sort.\n";
            return;
        }

        cout << "\n" << string(40, '=') << "\n";
        cout << "         SORT OPTIONS\n";
        cout << string(40, '=') << "\n";
        cout << "1. Sort by ID (Ascending)\n";
        cout << "2. Sort by ID (Descending)\n";
        cout << "3. Sort by Name (A-Z)\n";
        cout << "4. Sort by Name (Z-A)\n";
        cout << "5. Sort by Age (Youngest first)\n";
        cout << "6. Sort by Age (Oldest first)\n";
        cout << "7. Back to Main Menu\n";
        cout << string(40, '-') << "\n";
        cout << "Choose: ";
        
        int choice;
        cin >> choice;
        
        cout << "\n";
        switch(choice) {
            case 1:
                sort(students.begin(), students.end(),
                     [](const Student& a, const Student& b) {
                         return a.getId() < b.getId();
                     });
                cout << "✅ Students sorted by ID (Ascending)!\n";
                break;
            case 2:
                sort(students.begin(), students.end(),
                     [](const Student& a, const Student& b) {
                         return a.getId() > b.getId();
                     });
                cout << "✅ Students sorted by ID (Descending)!\n";
                break;
            case 3:
                sort(students.begin(), students.end(),
                     [](const Student& a, const Student& b) {
                         return toLower(a.getName()) < toLower(b.getName());
                     });
                cout << "✅ Students sorted by Name (A-Z)!\n";
                break;
            case 4:
                sort(students.begin(), students.end(),
                     [](const Student& a, const Student& b) {
                         return toLower(a.getName()) > toLower(b.getName());
                     });
                cout << "✅ Students sorted by Name (Z-A)!\n";
                break;
            case 5:
                sort(students.begin(), students.end(),
                     [](const Student& a, const Student& b) {
                         return a.getAge() < b.getAge();
                     });
                cout << "✅ Students sorted by Age (Youngest first)!\n";
                break;
            case 6:
                sort(students.begin(), students.end(),
                     [](const Student& a, const Student& b) {
                         return a.getAge() > b.getAge();
                     });
                cout << "✅ Students sorted by Age (Oldest first)!\n";
                break;
            case 7:
                return;
            default:
                cout << "❌ Invalid choice!\n";
                return;
        }
        
        displayAllStudents();
    }

    // ================= SEARCH MENU =================
    void searchMenu() {
        cout << "\n" << string(40, '=') << "\n";
        cout << "         SEARCH OPTIONS\n";
        cout << string(40, '=') << "\n";
        cout << "1. Fast Search (Binary - requires sorted ID)\n";
        cout << "2. Flexible Search (Linear - search by ID/Name/Course)\n";
        cout << "3. Back to Main Menu\n";
        cout << string(40, '-') << "\n";
        cout << "Choose: ";
        
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 1: binarySearchStudent(); break;
            case 2: linearSearchStudent(); break;
            case 3: return;
            default: cout << "❌ Invalid choice!\n";
        }
    }

    // ================= UPDATE STUDENT =================
    void updateStudent() {
        if (students.empty()) {
            cout << "\n📭 No students to update.\n";
            return;
        }

        int id;
        cout << "\nEnter student ID to update: ";
        cin >> id;

        for (auto& s : students) {
            if (s.getId() == id) {
                cout << "\nCurrent Information:\n";
                cout << string(40, '-') << "\n";
                s.displayCompact();
                cout << string(40, '-') << "\n\n";
                
                string name, email, course;
                int age;

                clearInputBuffer();
                cout << "New name (press Enter to keep '" << s.getName() << "'): ";
                getline(cin, name);
                if (!name.empty()) s.setName(name);

                cout << "New age (0 to keep " << s.getAge() << "): ";
                if (cin >> age && age > 0) {
                    s.setAge(age);
                }
                clearInputBuffer();

                cout << "New email (press Enter to keep '" << s.getEmail() << "'): ";
                getline(cin, email);
                if (!email.empty()) s.setEmail(email);

                cout << "New course (press Enter to keep '" << s.getCourse() << "'): ";
                getline(cin, course);
                if (!course.empty()) s.setCourse(course);

                cout << "\n✅ Student record updated successfully!\n";
                
                cout << "\nUpdated Information:\n";
                cout << string(40, '-') << "\n";
                s.displayCompact();
                cout << string(40, '-') << "\n";
                return;
            }
        }
        cout << "❌ Student with ID " << id << " not found.\n";
    }

    // ================= DELETE STUDENT =================
    void deleteStudent() {
        if (students.empty()) {
            cout << "\n📭 No students to delete.\n";
            return;
        }

        int id;
        cout << "\nEnter student ID to delete: ";
        cin >> id;

        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getId() == id) {
                cout << "\n⚠️  Are you sure you want to delete this student?\n";
                cout << string(40, '-') << "\n";
                it->displayCompact();
                cout << string(40, '-') << "\n";
                cout << "Enter 'yes' to confirm: ";
                
                string confirm;
                clearInputBuffer();
                getline(cin, confirm);
                
                if (toLower(confirm) == "yes" || toLower(confirm) == "y") {
                    students.erase(it);
                    cout << "\n✅ Student record deleted successfully!\n";
                } else {
                    cout << "\n❌ Deletion cancelled.\n";
                }
                return;
            }
        }
        cout << "❌ Student with ID " << id << " not found.\n";
    }

    // ================= STATISTICS =================
    void showStatistics() const {
        if (students.empty()) {
            cout << "\n📭 No statistics available (empty database).\n";
            return;
        }

        cout << "\n" << string(50, '=') << "\n";
        cout << "          DATABASE STATISTICS\n";
        cout << string(50, '=') << "\n";
        cout << "Total Students: " << students.size() << "\n";
        
        // Age statistics
        if (!students.empty()) {
            int totalAge = 0;
            int minAge = students[0].getAge();
            int maxAge = students[0].getAge();
            
            for (const auto& s : students) {
                totalAge += s.getAge();
                if (s.getAge() < minAge) minAge = s.getAge();
                if (s.getAge() > maxAge) maxAge = s.getAge();
            }
            
            double avgAge = static_cast<double>(totalAge) / students.size();
            cout << "Average Age: " << fixed << setprecision(1) << avgAge << " years\n";
            cout << "Age Range: " << minAge << " - " << maxAge << " years\n";
        }
        
        // Course distribution
        cout << "\nCourse Distribution:\n";
        cout << string(30, '-') << "\n";
        
        vector<string> courses;
        for (const auto& s : students) {
            courses.push_back(s.getCourse());
        }
        
        sort(courses.begin(), courses.end());
        courses.erase(unique(courses.begin(), courses.end()), courses.end());
        
        for (const auto& course : courses) {
            int count = 0;
            for (const auto& s : students) {
                if (s.getCourse() == course) count++;
            }
            cout << left << setw(20) << course << ": " << count << " student(s)\n";
        }
        
        cout << string(50, '=') << "\n";
    }

    // ================= MAIN MENU =================
    void displayMenu() const {
        cout << "\n" << string(50, '=') << "\n";
        cout << "    STUDENT RECORD MANAGEMENT SYSTEM \n";
        cout << string(50, '=') << "\n";
        cout << "Total Students: " << students.size() << "\n";
        cout << string(50, '-') << "\n";
        cout << "1. Add New Student\n";
        cout << "2. View All Students\n";
        cout << "3. Search Students\n";
        cout << "4. Sort Students\n";
        cout << "5. Update Student Record\n";
        cout << "6. Delete Student\n";
        cout << "7. Show Statistics\n";
        cout << "8. Save & Exit\n";
        cout << string(50, '-') << "\n";
        cout << "Choose option (1-8): ";
    }
};
bool authenticate() {
    const string ADMIN_ID = "admin";
    const string ADMIN_PASS = "admin1234";

    string id, pass;
    int attempts = 3;

    cout << "\n" << string(50, '=') << "\n";
    cout << "           ADMIN LOGIN\n";
    cout << string(50, '=') << "\n";

    while (attempts--) {
        cout << "Enter Admin ID: ";
        cin >> id;
        cout << "Enter Password: ";
        cin >> pass;

        if (id == ADMIN_ID && pass == ADMIN_PASS) {
            cout << "\n✅ Login successful! Full access granted.\n";
            return true;
        } else {
            cout << "❌ Invalid credentials. Attempts left: "
                 << attempts << "\n\n";
        }
    }

    cout << "🚫 Access denied. Program exiting.\n";
    return false;
}
// =========================== MAIN ===========================
int main() {

    if (!authenticate()) {
        return 0;   // Exit program if login fails
    }
    StudentManagementSystem system;    
    cout << "\n" << string(60, '*') << "\n";
    cout << "  WELCOME TO STUDENT RECORD MANAGEMENT SYSTEM\n";
    cout << string(60, '*') << "\n";
    
    system.loadFromFile();
    
    int choice;
    do {
        system.displayMenu();
        
        if (!(cin >> choice)) {
            cin.clear();
            // Use a simple clear buffer here since we can't access the private method
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n❌ Invalid input! Please enter a number 1-8.\n";
            continue;
        }
        
        switch (choice) {
            case 1: system.addStudent(); break;
            case 2: system.displayAllStudents(); break;
            case 3: system.searchMenu(); break;
            case 4: system.sortMenu(); break;
            case 5: system.updateStudent(); break;
            case 6: system.deleteStudent(); break;
            case 7: system.showStatistics(); break;
            case 8:
                system.saveToFile();
                cout << "\n" << string(50, '=') << "\n";
                cout << "         THANK YOU FOR USING\n";
                cout << "    STUDENT MANAGEMENT SYSTEM\n";
                cout << string(50, '=') << "\n";
                break;
            default:
                cout << "\n❌ Invalid option! Please choose 1-8.\n";
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        
    } while (choice != 8);

    return 0;
}
