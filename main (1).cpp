#include <iostream>
#include <string>
#include <vector>
#include <iomanip> // setw, setprecision
#include <fstream>
#include <cctype>  // isdigit
#include <limits>  // numeric_limits
#include <sstream> // ostringstream
#include <bits/stdc++.h>
#ifdef _WIN32
#include <conio.h> // _getch for Windows
#endif

using namespace std;

// ------------------ Global takePassword function ------------------
void takePassword(string &pass)
{
    char ch;
    pass.clear();

    while (true)
    {
#ifdef _WIN32
        ch = _getch(); // Windows-only
#else
        ch = getchar(); // fallback for other platforms (echoed)
#endif

        if (ch == 13 || ch == '\n') // Enter key
        {
            cout << "\n";
            break;
        }
        else if (ch == 8) // Backspace
        {
            if (!pass.empty())
            {
                pass.pop_back();
                cout << "\b \b"; // erase * from console
            }
        }
        else if (ch == 32 || ch == 9 || ch == 27) // ignore space, tab, ESC
        {
            continue;
        }
        else
        {
            pass.push_back(ch);
            cout << "*"; // show asterisk
        }
    }
}

// ------------------ User Class ------------------
class User
{
protected:
    string name, email, phone, password;

    // ---------------- Encryption ----------------
    string encryptPassword(const string &pwd) const
    {
        string encrypted = pwd;
        for (char &c : encrypted)
            c += 3;
        return encrypted;
    }

    string decryptPassword(const string &pwd) const
    {
        string decrypted = pwd;
        for (char &c : decrypted)
            c -= 3;
        return decrypted;
    }

    // ---------------- Validation ----------------
    bool isValidEmail(const string &e) const
    {
        size_t atPos = e.find('@');
        size_t dotPos = e.find('.', atPos);
        return (atPos != string::npos && dotPos != string::npos && atPos < dotPos);
    }

    bool isValidPhone(const string &p) const
    {
        if (p.length() != 11)
            return false;
        for (char c : p)
            if (!isdigit(c))
                return false;
        return true;
    }

public:
    virtual ~User() {}

    // ---------------- Input ----------------
    virtual void saveToFile(ofstream &out) const {}
    virtual void loadFromFile(ifstream &in) {}

    virtual void inputData()
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
        cout << "Enter name: ";
        getline(cin, name);

        // Email validation
        do
        {
            cout << "Enter email: ";
            getline(cin, email);
            if (!isValidEmail(email))
                cout << "Invalid email format. Try again.\n";
        } while (!isValidEmail(email));

        // Phone validation
        do
        {
            cout << "Enter phone (11 digits): ";
            getline(cin, phone);
            if (!isValidPhone(phone))
                cout << "Invalid phone number. Must be 11 digits.\n";
        } while (!isValidPhone(phone));

        // Password validation (min 8 chars)
        do
        {
            cout << "Enter password (min 8 chars): ";
            ::takePassword(password);
            if (password.length() < 8)
                cout << "Password too short. Try again.\n";
        } while (password.length() < 8);

        password = encryptPassword(password);
    }

    // ---------------- Display ----------------
    virtual void displayData() const
    {
        cout << "Name: " << name
             << "\nEmail: " << email
             << "\nPhone: " << phone << endl;
    }

    // ---------------- Getters / Setters ----------------
    void setName(const string &n) { name = n; }
    void setEmail(const string &e) { email = e; }
    void setPhone(const string &p) { phone = p; }
    void setPassword(const string &p) { password = encryptPassword(p); }

    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getPassword() const { return decryptPassword(password); }

    // ---------------- Abstract Methods ----------------
    virtual string getID() const = 0;

    // ---------------- Stream Operators ----------------
    friend ostream &operator<<(ostream &out, const User &u)
    {
        out << u.name << "\n"
            << u.email << "\n"
            << u.phone << "\n"
            << u.password << "\n"; // saved as encrypted
        return out;
    }

    friend istream &operator>>(istream &in, User &u)
    {
        getline(in, u.name);
        getline(in, u.email);
        getline(in, u.phone);
        string encryptedPwd;
        getline(in, encryptedPwd);
        u.password = encryptedPwd; // already encrypted in file
        return in;
    }
};

// ------------------ Result Class ------------------
class Result
{
private:
    struct Course
    {
        string name;
        double credit;
        double marks;
        double totalMarks;
    };

    vector<Course> courses;
    double gpa = 0.0;

    double getGradePoint(double percentage) const
    {
        if (percentage >= 80)
            return 4.00;
        else if (percentage >= 75)
            return 3.75;
        else if (percentage >= 70)
            return 3.50;
        else if (percentage >= 65)
            return 3.25;
        else if (percentage >= 60)
            return 3.00;
        else if (percentage >= 55)
            return 2.75;
        else if (percentage >= 50)
            return 2.50;
        else if (percentage >= 45)
            return 2.25;
        else if (percentage >= 40)
            return 2.00;
        else
            return 0.00;
    }

    string getLetterGrade(double gp) const
    {
        if (gp == 4.00)
            return "A+";
        else if (gp >= 3.75)
            return "A";
        else if (gp >= 3.50)
            return "A-";
        else if (gp >= 3.25)
            return "B+";
        else if (gp >= 3.00)
            return "B";
        else if (gp >= 2.75)
            return "B-";
        else if (gp >= 2.50)
            return "C+";
        else if (gp >= 2.25)
            return "C";
        else if (gp >= 2.00)
            return "D";
        else
            return "F";
    }

public:
    Result()
    {
        courses = {
            {"OOP", 3, 0, 300},
            {"DLD", 3, 0, 300},
            {"Basic EE", 3, 0, 300},
            {"Chemistry", 3, 0, 300},
            {"Math", 3, 0, 300},
            {"DLD Lab", 1.5, 0, 100},
            {"Basic EE Lab", 1.5, 0, 100},
            {"OOP Lab", 1.5, 0, 100},
            {"Chemistry Lab", 0.75, 0, 100}};
    }

    void inputMarks()
    {
        cout << "\n--- Enter Marks for Each Course ---\n";
        for (auto &c : courses)
        {
            do
            {
                cout << c.name << " (out of " << c.totalMarks << "): ";
                cin >> c.marks;
                if (c.marks < 0 || c.marks > c.totalMarks)
                    cout << "Invalid marks! Enter between 0 and " << c.totalMarks << ".\n";
            } while (c.marks < 0 || c.marks > c.totalMarks);
        }
        calculateGPA();
    }

    void calculateGPA()
    {
        double totalCredit = 0, weightedSum = 0;
        for (auto &c : courses)
        {
            double percentage = (c.marks / c.totalMarks) * 100.0;
            double gradePoint = getGradePoint(percentage);
            weightedSum += gradePoint * c.credit;
            totalCredit += c.credit;
        }
        gpa = (totalCredit > 0) ? (weightedSum / totalCredit) : 0.0;
    }

    double getGPA() const { return gpa; }

    void displayResult() const
    {
        cout << "\n--- Student Result ---\n";
        cout << left << setw(15) << "Course"
             << setw(12) << "Marks"
             << setw(12) << "Percent"
             << setw(10) << "Credit"
             << setw(8) << "Grade" << "\n";
        cout << "--------------------------------------------------------\n";

        for (const auto &c : courses)
        {
            double percentage = (c.marks / c.totalMarks) * 100.0;
            double gp = getGradePoint(percentage);

            ostringstream oss;
            oss << (int)c.marks << "/" << (int)c.totalMarks;

            cout << setw(15) << c.name
                 << setw(12) << oss.str()
                 << setw(12) << fixed << setprecision(2) << percentage
                 << setw(10) << c.credit
                 << setw(8) << getLetterGrade(gp) << "\n";
        }

        cout << "--------------------------------------------------------\n";
        cout << "Calculated GPA: " << fixed << setprecision(2) << gpa << "\n";
    }

    void saveToFile(ofstream &out) const
    {
        if (!out.is_open())
            return;
        out << gpa << "\n"
            << courses.size() << "\n";
        for (const auto &c : courses)
            out << c.name << "\n"
                << c.credit << " " << c.marks << " " << c.totalMarks << "\n";
    }

    void loadFromFile(ifstream &in)
    {
        if (!in.is_open())
            return;
        if (!(in >> gpa))
            return;
        size_t count;
        in >> count;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        courses.clear();
        for (size_t i = 0; i < count; ++i)
        {
            Course c;
            getline(in, c.name);
            in >> c.credit >> c.marks >> c.totalMarks;
            in.ignore(numeric_limits<streamsize>::max(), '\n');
            courses.push_back(c);
        }
    }

    friend ostream &operator<<(ostream &os, const Result &r)
    {
        os << "GPA: " << fixed << setprecision(2) << r.gpa;
        return os;
    }

    virtual ~Result() {}
};

class Student : public User
{
private:
    string roll;
    Result result;
    bool hasResult = false;

public:
    // Input student details
    void inputData() override
    {
        User::inputData();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        do
        {
            cout << "Enter roll number: ";
            getline(cin, roll);
            if (roll.empty())
                cout << "Roll number cannot be empty.\n";
        } while (roll.empty());
    }

    // Display student info + result
    void displayData() const override
    {
        cout << "\n--- Student Information ---\n";
        User::displayData();
        cout << "Roll: " << roll << endl;
        if (hasResult)
            result.displayResult();
        else
            cout << "[No result assigned yet]\n";
    }

    // Getters and setters
    void setRoll(const string &r) { roll = r; }
    string getRoll() const { return roll; }
    string getID() const override { return roll; }

    Result &getResult() { return result; }
    const Result &getResult() const { return result; }
    bool resultAssigned() const { return hasResult; }

    // Save student info + result
    void saveToFile(ofstream &out) const override
    {
        if (!out.is_open())
            return;
        out << roll << "\n"
            << name << "\n"
            << email << "\n"
            << phone << "\n"
            << password << "\n";
        result.saveToFile(out);
        out << "END_STUDENT\n";
    }

    // Load student info + result
    void loadFromFile(ifstream &in) override
    {
        if (!in.is_open())
            return;
        getline(in, roll);
        getline(in, name);
        getline(in, email);
        getline(in, phone);
        getline(in, password);
        result.loadFromFile(in);
        hasResult = (result.getGPA() > 0.0);
        string dummy;
        getline(in, dummy); // END_STUDENT
    }

    // Assign marks to courses
    void assignResult()
    {
        cout << "\n--- Assigning Marks for " << name << " (" << roll << ") ---\n";
        result.inputMarks();
        hasResult = true;
        cout << "\n Result updated successfully!\n";
    }

    // Student Dashboard
    void dashboard()
    {
        int choice;
        do
        {
            cout << "\n========== Student Dashboard ==========\n";
            cout << "1. View your information\n";
            cout << "2. Update/Edit information\n";
            cout << "3. Change password\n";
            cout << "4. View Result\n";
            cout << "5. Logout\n";
            cout << "=======================================\n";
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
            case 1:
                displayData();
                break;

            case 2:
            {
                string n, e, ph, r;
                cout << "Enter new name: ";
                getline(cin, n);
                do
                {
                    cout << "Enter new email: ";
                    getline(cin, e);
                    if (!isValidEmail(e))
                        cout << "Invalid email format.\n";
                } while (!isValidEmail(e));
                do
                {
                    cout << "Enter new phone: ";
                    getline(cin, ph);
                    if (!isValidPhone(ph))
                        cout << "Invalid phone number.\n";
                } while (!isValidPhone(ph));
                do
                {
                    cout << "Enter new roll number: ";
                    getline(cin, r);
                    if (r.empty())
                        cout << "Roll number cannot be empty.\n";
                } while (r.empty());

                setName(n);
                setEmail(e);
                setPhone(ph);
                setRoll(r);
                cout << "Information updated successfully!\n";
                break;
            }

            case 3:
            {
                string newPass;
                do
                {
                    cout << "Enter new password (min 8 chars): ";
                    ::takePassword(newPass);
                    if (newPass.length() < 8)
                        cout << "Password too short.\n";
                } while (newPass.length() < 8);
                setPassword(newPass);
                cout << " Password changed successfully!\n";
                break;
            }

            case 4:
                if (hasResult)
                    result.displayResult();
                else
                    cout << "[No result assigned yet]\n";
                break;

            case 5:
                cout << "Logging out...\n";
                break;

            default:
                cout << "Invalid choice.\n";
            }

        } while (choice != 5);
    }

    // Operator Overloads
    friend ostream &operator<<(ostream &os, const Student &s)
    {
        os << "Roll: " << s.roll << ", Name: " << s.name
           << ", GPA: " << fixed << setprecision(2) << s.result.getGPA();
        return os;
    }

    // Equality operator (==) overload
    bool operator==(const Student &other) const
    {
        return (roll == other.roll && name == other.name && email == other.email);
    }

    // Less than operator (<) overload for sorting by roll number
    bool operator<(const Student &other) const
    {
        return roll < other.roll;
    }
};

// ---------------- Teacher Class ----------------
class Teacher : public User
{
private:
    string employeeID;

public:
    void inputData() override
    {
        User::inputData();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        do
        {
            cout << "Enter employee ID: ";
            getline(cin, employeeID);
            if (employeeID.empty())
                cout << "Employee ID cannot be empty.\n";
        } while (employeeID.empty());
    }

    void displayData() const override
    {
        cout << "\n--- Teacher Information ---\n";
        User::displayData();
        cout << "Employee ID: " << employeeID << endl;
    }

    void setEmployeeID(const string &id) { employeeID = id; }
    string getEmployeeID() const { return employeeID; }
    string getID() const override { return employeeID; }

    void saveToFile(ofstream &out) const override
    {
        if (!out.is_open())
            return;
        out << employeeID << "\n"
            << name << "\n"
            << email << "\n"
            << phone << "\n"
            << password << "\n"
            << "END_TEACHER\n";
    }

    void loadFromFile(ifstream &in) override
    {
        if (!in.is_open())
            return;
        getline(in, employeeID);
        getline(in, name);
        getline(in, email);
        getline(in, phone);
        getline(in, password);
        string dummy;
        getline(in, dummy); // END_TEACHER
    }

   void dashboard(vector<Student> &students)
{
    int choice;
    do
    {
        cout << "\n========== Teacher Dashboard ==========\n";
        cout << "1. View your information\n";
        cout << "2. Edit your information\n";
        cout << "3. Change password\n";
        cout << "4. View all students\n";
        cout << "5. Search student by roll\n";
        cout << "6. Assign/Update marks for a student\n";
        cout << "7. Logout\n";
        cout << "=======================================\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            displayData();
            break;

        case 2:
        {
            string n, e, ph, id;
            cout << "Enter new name: ";
            getline(cin, n);
            do
            {
                cout << "Enter new email: ";
                getline(cin, e);
                if (!isValidEmail(e))
                    cout << "Invalid email format.\n";
            } while (!isValidEmail(e));
            do
            {
                cout << "Enter new phone: ";
                getline(cin, ph);
                if (!isValidPhone(ph))
                    cout << "Invalid phone number.\n";
            } while (!isValidPhone(ph));
            do
            {
                cout << "Enter new employee ID: ";
                getline(cin, id);
                if (id.empty())
                    cout << "Cannot be empty.\n";
            } while (id.empty());

            setName(n);
            setEmail(e);
            setPhone(ph);
            setEmployeeID(id);
            cout << " Information updated successfully!\n";
            break;
        }

        case 3:
        {
            string newPass;
            do
            {
                cout << "Enter new password (min 8 chars): ";
                ::takePassword(newPass);
                if (newPass.length() < 8)
                    cout << "Password too short.\n";
            } while (newPass.length() < 8);
            setPassword(newPass);
            cout << " Password changed successfully!\n";
            break;
        }

        case 4:
            cout << "\n--- All Students ---\n";
            for (const auto &s : students)
                s.displayData();
            break;

        case 5:
        {
            string roll;
            cout << "Enter roll to search: ";
            getline(cin, roll);
            bool found = false;
            for (const auto &s : students)
            {
                if (s.getRoll() == roll)
                {
                    s.displayData();
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << " Student not found.\n";
            break;
        }

        case 6:
        {
            string roll;
            cout << "Enter roll to assign marks: ";
            getline(cin, roll);
            bool found = false;
            for (auto &s : students)
            {
                if (s.getRoll() == roll)
                {
                    s.assignResult();
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << " Student not found.\n";
            break;
        }

        case 7:
            cout << " Logging out...\n";
            break;

        default:
            cout << " Invalid choice.\n";
        }

    } while (choice != 7);
}

    // Operator Overloads
    friend ostream &operator<<(ostream &os, const Teacher &t)
    {
        os << "Employee ID: " << t.employeeID
           << ", Name: " << t.name
           << ", Email: " << t.email;
        return os;
    }

    // Equality operator (==) overload
    bool operator==(const Teacher &other) const
    {
        return (employeeID == other.employeeID && name == other.name && email == other.email);
    }

    // Greater than operator (>) overload for sorting by employee ID
    bool operator>(const Teacher &other) const
    {
        return employeeID > other.employeeID;
    }
};

// ---------------- Admin Class ----------------
class Admin : public User
{
private:
    string adminID;

public:
    // Default Constructor
    Admin() = default;

    // Parameterized Constructor
    Admin(const string &id, const string &n, const string &e, const string &ph, const string &pass)
    {
        adminID = id;
        name = n;
        email = e;
        phone = ph;
        password = pass;
    }

    // Input Admin Data
    void inputData() override
    {
        User::inputData();
        cout << "Enter admin ID: ";
        getline(cin, adminID);
    }

    // Display Admin Data
    void displayData() const override
    {
        cout << "\n--- Admin Information ---\n";
        User::displayData();
        cout << "Admin ID   : " << adminID << "\n";
    }

    // Getters / Setters
    void setAdminID(const string &id) { adminID = id; }
    string getAdminID() const { return adminID; }
    string getID() const override { return adminID; }

    // Save to File (Text Format)
    void saveToFile(ofstream &out) const override
    {
        out << adminID << '\n'
            << name << '\n'
            << email << '\n'
            << phone << '\n'
            << password << '\n';
    }

    // Load from File (Text Format)
    void loadFromFile(ifstream &in) override
    {
        getline(in, adminID);
        getline(in, name);
        getline(in, email);
        getline(in, phone);
        getline(in, password);
    }

    // Binary Save (optional)
    void saveBinary(ofstream &out) const
    {
        size_t len;
        auto writeString = [&](const string &s)
        {
            len = s.size();
            out.write(reinterpret_cast<char *>(&len), sizeof(len));
            out.write(s.c_str(), len);
        };

        writeString(adminID);
        writeString(name);
        writeString(email);
        writeString(phone);
        writeString(password);
    }

    // Binary Load (optional)
    void loadBinary(ifstream &in)
    {
        size_t len;
        auto readString = [&](string &s)
        {
            in.read(reinterpret_cast<char *>(&len), sizeof(len));
            s.resize(len);
            in.read(&s[0], len);
        };

        readString(adminID);
        readString(name);
        readString(email);
        readString(phone);
        readString(password);
    }

    // Dashboard Menu
    void dashboard()
    {
        int choice;
        do
        {
            cout << "\n--- Admin Dashboard ---\n";
            cout << "1. View your information\n";
            cout << "2. Update/Edit your information\n";
            cout << "3. Change password\n";
            cout << "4. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case 1:
                displayData();
                break;

            case 2:
            {
                string n, e, ph, id;
                cout << "Enter new name: ";
                getline(cin, n);
                cout << "Enter new email: ";
                getline(cin, e);
                cout << "Enter new phone: ";
                getline(cin, ph);
                cout << "Enter new admin ID: ";
                getline(cin, id);

                setName(n);
                setEmail(e);
                setPhone(ph);
                setAdminID(id);

                cout << " Information updated successfully!\n";
                break;
            }

            case 3:
            {
                string newPass;
                cout << "Enter new password: ";
                ::takePassword(newPass); // Global secure password input
                setPassword(newPass);
                cout << " Password changed successfully!\n";
                break;
            }

            case 4:
                cout << " Logging out...\n";
                break;

            default:
                cout << " Invalid choice! Try again.\n";
            }

        } while (choice != 4);
    }

    // Operator Overloads
    friend ostream &operator<<(ostream &os, const Admin &a)
    {
        os << "Admin ID: " << a.adminID << ", Name: " << a.name;
        return os;
    }

    // Assignment operator (=) overload
    Admin &operator=(const Admin &other)
    {
        if (this != &other)
        {
            adminID = other.adminID;
            name = other.name;
            email = other.email;
            phone = other.phone;
            password = other.password;
        }
        return *this;
    }

    // Equality operator (==) overload
    bool operator==(const Admin &other) const
    {
        return (adminID == other.adminID && name == other.name);
    }
};

// ------------------ Main Menu ------------------
class MainMenu
{
private:
    vector<Student> students;
    vector<Teacher> teachers;
    vector<Admin> admins;

    const string studentFile = "students.txt";
    const string teacherFile = "teachers.txt";
    const string adminFile = "admins.txt";

    Admin defaultAdmin;

    // ------------------ Load All Users ------------------
    void loadUsers()
    {
        ifstream fin;

        // Load Students
        fin.open(studentFile);
        if (fin.is_open())
        {
            while (true)
            {
                Student s;
                s.loadFromFile(fin);
                if (!fin.good() || s.getRoll().empty())
                    break;
                students.push_back(s);
            }
            fin.close();
        }

        // Load Teachers
        fin.open(teacherFile);
        if (fin.is_open())
        {
            while (true)
            {
                Teacher t;
                t.loadFromFile(fin);
                if (!fin.good() || t.getEmployeeID().empty())
                    break;
                teachers.push_back(t);
            }
            fin.close();
        }

        // Load Admins
        fin.open(adminFile);
        if (fin.is_open())
        {
            while (true)
            {
                Admin a;
                a.loadFromFile(fin);
                if (!fin.good() || a.getAdminID().empty())
                    break;
                admins.push_back(a);
            }
            fin.close();
        }

        // Create default admin if none exist
        if (admins.empty())
        {
            defaultAdmin.setAdminID("admin");
            defaultAdmin.setName("Super Admin");
            defaultAdmin.setEmail("admin@example.com");
            defaultAdmin.setPhone("01234567890");
            defaultAdmin.setPassword("admin123");
            admins.push_back(defaultAdmin);
            saveUsers();
        }
    }

    // ------------------ Save All Users ------------------
    void saveUsers()
    {
        ofstream fout;

        // Save Students
        fout.open(studentFile);
        for (auto &s : students)
            s.saveToFile(fout);
        fout.close();

        // Save Teachers
        fout.open(teacherFile);
        for (auto &t : teachers)
            t.saveToFile(fout);
        fout.close();

        // Save Admins
        fout.open(adminFile);
        for (auto &a : admins)
            a.saveToFile(fout);
        fout.close();
    }

    // ------------------ Registration ------------------
    void registerUser()
    {
        int choice;
        cout << "\n--- Registration ---\n";
        cout << "1. Student\n2. Teacher\n";
        cout << "Choose user type: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            Student s;
            s.inputData();

            // Check duplicate roll
            bool exists = any_of(students.begin(), students.end(),
                                 [&](const Student &st)
                                 { return st.getRoll() == s.getRoll(); });
            if (exists)
            {
                cout << " Roll number already exists!\n";
                break;
            }

            students.push_back(s);
            cout << " Student registered successfully!\n";
            break;
        }
        case 2:
        {
            Teacher t;
            t.inputData();

            bool exists = any_of(teachers.begin(), teachers.end(),
                                 [&](const Teacher &th)
                                 { return th.getEmployeeID() == t.getEmployeeID(); });
            if (exists)
            {
                cout << " Employee ID already exists!\n";
                break;
            }

            teachers.push_back(t);
            cout << " Teacher registered successfully!\n";
            break;
        }
        default:
            cout << " Invalid choice!\n";
        }

        saveUsers();
    }

    // ------------------ Login ------------------
    void loginUser()
    {
        int choice;
        cout << "\n--- Login ---\n";
        cout << "1. Student\n2. Teacher\n3. Admin\n";
        cout << "Choose user type: ";
        cin >> choice;
        cin.ignore();

        string id, password;
        cout << "Enter ID (Roll / Employee / Admin): ";
        getline(cin, id);
        cout << "Enter Password: ";
        ::takePassword(password);

        bool success = false;

        switch (choice)
        {
        case 1:
            for (auto &s : students)
            {
                if (s.getRoll() == id && s.getPassword() == password)
                {
                    cout << "\n Student login successful!\n";
                    s.dashboard();
                    success = true;
                    break;
                }
            }
            break;

        case 2:
            for (auto &t : teachers)
            {
               if (t.getEmployeeID() == id && t.getPassword() == password)
{
    cout << "\n Teacher login successful!\n";
    t.dashboard(students); // pass reference
    saveUsers(); // save any changes to student data
    success = true;
    break;
}

            }
            break;

        case 3:
            for (auto &a : admins)
            {
                if (a.getAdminID() == id && a.getPassword() == password)
                {
                    cout << "\n Admin login successful!\n";
                    adminDashboard(a);
                    success = true;
                    break;
                }
            }
            break;

        default:
            cout << " Invalid choice!\n";
        }

        if (!success)
            cout << " Login failed. Invalid ID or password.\n";
    }

    // ------------------ Remove Student Account ------------------
    void removeStudentAccount()
    {
        string roll;
        cout << "Enter student roll number to remove: ";
        getline(cin, roll);

        auto it = find_if(students.begin(), students.end(),
                          [&](const Student &s)
                          { return s.getRoll() == roll; });

        if (it != students.end())
        {
            cout << "Are you sure you want to remove student: " << it->getName()
                 << " (Roll: " << it->getRoll() << ")? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();

            if (confirm == 'y' || confirm == 'Y')
            {
                students.erase(it);
                saveUsers();
                cout << " Student account removed successfully!\n";
            }
            else
            {
                cout << " Removal cancelled.\n";
            }
        }
        else
        {
            cout << " Student not found with roll number: " << roll << "\n";
        }
    }

    // ------------------ Remove Teacher Account ------------------
    void removeTeacherAccount()
    {
        string employeeID;
        cout << "Enter teacher employee ID to remove: ";
        getline(cin, employeeID);

        auto it = find_if(teachers.begin(), teachers.end(),
                          [&](const Teacher &t)
                          { return t.getEmployeeID() == employeeID; });

        if (it != teachers.end())
        {
            cout << "Are you sure you want to remove teacher: " << it->getName()
                 << " (Employee ID: " << it->getEmployeeID() << ")? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();

            if (confirm == 'y' || confirm == 'Y')
            {
                teachers.erase(it);
                saveUsers();
                cout << " Teacher account removed successfully!\n";
            }
            else
            {
                cout << " Removal cancelled.\n";
            }
        }
        else
        {
            cout << " Teacher not found with employee ID: " << employeeID << "\n";
        }
    }

    // ------------------ View All Students ------------------
    void viewAllStudents()
    {
        cout << "\n--- All Students ---\n";
        if (students.empty())
        {
            cout << "No students registered.\n";
            return;
        }

        for (const auto &s : students)
        {
            s.displayData();
            cout << "------------------------\n";
        }
    }

    // ------------------ View All Teachers ------------------
    void viewAllTeachers()
    {
        cout << "\n--- All Teachers ---\n";
        if (teachers.empty())
        {
            cout << "No teachers registered.\n";
            return;
        }

        for (const auto &t : teachers)
        {
            t.displayData();
            cout << "------------------------\n";
        }
    }

    // ------------------ Admin Dashboard ------------------
    void adminDashboard(Admin &a)
    {
        int choice;
        do
        {
            cout << "\n========== Admin Dashboard ==========\n";
            cout << "1. View your information\n";
            cout << "2. Change password\n";
            cout << "3. Enter marks & calculate GPA for a student\n";
            cout << "4. View all students\n";
            cout << "5. View all teachers\n";
            cout << "6. Remove student account\n";
            cout << "7. Remove teacher account\n";
            cout << "8. Logout\n";
            cout << "======================================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case 1:
                a.displayData();
                break;

            case 2:
            {
                string newPass;
                cout << "Enter new password: ";
                ::takePassword(newPass);
                a.setPassword(newPass);
                cout << " Password changed successfully!\n";
                saveUsers();
                break;
            }

            case 3:
            {
                string roll;
                cout << "Enter student roll: ";
                getline(cin, roll);
                bool found = false;

                for (auto &s : students)
                {
                    if (s.getRoll() == roll)
                    {
                        s.assignResult();
                        saveUsers();
                        found = true;
                        break;
                    }
                }

                if (!found)
                    cout << " Student not found!\n";
                break;
            }

            case 4:
                viewAllStudents();
                break;

            case 5:
                viewAllTeachers();
                break;

            case 6:
                removeStudentAccount();
                break;

            case 7:
                removeTeacherAccount();
                break;

            case 8:
                cout << " Logging out...\n";
                break;

            default:
                cout << " Invalid choice!\n";
            }

        } while (choice != 8);
    }

public:
    MainMenu()
    {
        loadUsers();
    }

    void run()
    {
        int choice;
        do
        {
            cout << "\n========== Student Management System ==========\n";
            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "3. Exit\n";
            cout << "===============================================\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
            }

        } while (choice != 3);

        saveUsers();
    }
};

// ------------------ Main Function ------------------
int main()
{
    MainMenu menu;
    menu.run();
    return 0;
}
