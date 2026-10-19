#ifndef REPORTCARD_H
#define REPORTCARD_H

#include "helpers.h"
#include "dsa_library.h"

// =====================================================
// MODULE 3: REPORT CARD GENERATOR
// Array = subjects and marks (sorted + searched), Queue = students,
// Stack = log of mark corrections
// =====================================================
struct Student
{
    string rollNo;
    string name;
    string className;
    string photoPath;
    ItemArray subjects;     // subject names + marks
};

class ReportCardGenerator
{
private:
    Student students[20];
    int studentCount;
    ActionStack history;

    int findStudent(string rollNo)
    {
        for (int i = 0; i < studentCount; i++)
        {
            if (students[i].rollNo == rollNo)
                return i;
        }
        return -1;
    }

    void writeReportCard(Student student)
    {
        string css = ".page { border-top: 6px solid #004085; font-family: Arial, sans-serif; }\n"
                     "h2 { color: #004085; text-align: center; }\n"
                     "img.photo { display: block; margin: auto; }\n"
                     "table { width: 100%; border-collapse: collapse; margin-top: 15px; }\n"
                     "th, td { border: 1px solid #ccc; padding: 10px; text-align: left; }\n"
                     "th { background: #004085; color: white; }\n"
                     ".best { background: #d4edda; }\n.weak { background: #f8d7da; }\n"
                     ".summary { margin-top: 20px; font-weight: bold; text-align: right; }\n";
        ofstream file;
        string fileName = makeFileName("reportcard_", student.rollNo);
        if (!createPage(file, fileName, "Report Card - " + student.name, css))
            return;

        student.subjects.sortByValue();         // highest marks first
        int subjectCount = student.subjects.size();
        int total = 0;
        for (int i = 0; i < subjectCount; i++)
            total += student.subjects.getValue(i);

        double percentage = (double) total / subjectCount;      // every subject is out of 100
        string grade;
        if (percentage >= 80) grade = "A+";
        else if (percentage >= 70) grade = "A";
        else if (percentage >= 60) grade = "B";
        else if (percentage >= 50) grade = "C";
        else grade = "F";

        writeImage(file, student.photoPath, "photo");
        file << "<h2>ACADEMIC REPORT CARD</h2>\n";
        file << "<p><b>Name:</b> " << makeSafe(student.name) << "<br>\n";
        file << "<b>Roll / Reg No:</b> " << makeSafe(student.rollNo) << "<br>\n";
        file << "<b>Class / Program:</b> " << makeSafe(student.className) << "</p>\n";

        file << "<table>\n<tr><th>Subject</th><th>Marks</th><th>Out of</th></tr>\n";
        for (int i = 0; i < subjectCount; i++)
        {
            string rowClass = "";
            if (subjectCount > 1 && i == 0) rowClass = " class='best'";
            if (subjectCount > 1 && i == subjectCount - 1) rowClass = " class='weak'";
            file << "<tr" << rowClass << "><td>" << makeSafe(student.subjects.getName(i)) << "</td><td>"
                 << student.subjects.getValue(i) << "</td><td>100</td></tr>\n";
        }
        file << "</table>\n";

        file << "<div class='summary'>\n";
        file << "<p>Best subject: " << makeSafe(student.subjects.getName(0)) << "<br>\n";
        file << "Weakest subject: " << makeSafe(student.subjects.getName(subjectCount - 1)) << "</p>\n";
        file << "<p>Total: " << total << " / " << subjectCount * 100 << "<br>\n";
        file << "Percentage: " << percentage << "%<br>\n";
        file << "Grade: " << grade << "<br>\n";
        file << "Result: " << (percentage >= 50 ? "PASS" : "FAIL") << "</p>\n</div>\n";
        finishPage(file, fileName);
    }

    void addStudent()
    {
        if (studentCount == 20)
        {
            cout << "Student list is full.\n";
            return;
        }
        Student s;
        s.rollNo = readText("Roll / Registration No: ");
        if (findStudent(s.rollNo) != -1)
        {
            cout << "This roll number already exists.\n";
            return;
        }
        s.name = readText("Student name: ");
        s.className = readText("Class / Program: ");
        s.photoPath = readText("Photo path (Enter to skip): ");

        int subjectCount = readRange("Number of subjects (1 to 10): ", 1, 10);
        for (int i = 1; i <= subjectCount; i++)
        {
            string subject = readText("  Subject " + toText(i) + " name: ");
            int marks = readRange("  Marks (0 to 100): ", 0, 100);
            s.subjects.add(subject, marks);
        }

        students[studentCount] = s;
        studentCount++;
        history.push("Added student " + s.name + " (" + s.rollNo + ")");
        cout << "Student added to the queue.\n";
    }

    void searchSubject()
    {
        int index = findStudent(readText("Roll no. of the student: "));
        if (index == -1)
        {
            cout << "Student not found.\n";
            return;
        }
        string subject = readText("Subject to search: ");
        int position = students[index].subjects.search(subject);
        if (position == -1)
            cout << "Subject not found.\n";
        else
            cout << subject << ": " << students[index].subjects.getValue(position) << "/100\n";
    }

    void correctMarks()
    {
        int index = findStudent(readText("Roll no. of the student: "));
        if (index == -1)
        {
            cout << "Student not found.\n";
            return;
        }
        string subject = readText("Subject to correct: ");
        int position = students[index].subjects.search(subject);
        if (position == -1)
        {
            cout << "Subject not found.\n";
            return;
        }
        int oldMarks = students[index].subjects.getValue(position);
        int newMarks = readRange("New marks (0 to 100): ", 0, 100);
        students[index].subjects.setValue(position, newMarks);
        history.push("Changed " + subject + " marks of " + students[index].rollNo + " from "
                     + toText(oldMarks) + " to " + toText(newMarks));
    }

public:
    ReportCardGenerator()
    {
        studentCount = 0;
    }

    // Students are queued by roll number and processed in the order they were entered
    void generate()
    {
        if (studentCount == 0)
        {
            cout << "Add at least one student first.\n";
            return;
        }
        StringQueue queue;
        for (int i = 0; i < studentCount; i++)
            queue.enqueue(students[i].rollNo);
        while (!queue.isEmpty())
            writeReportCard(students[findStudent(queue.dequeue())]);
    }

    void run()
    {
        int choice;
        do
        {
            cout << "\n--- REPORT CARD GENERATOR ---\n"
                 << "1. Add student\n2. Search a subject's marks\n3. Correct marks\n"
                 << "4. Show correction history\n5. Generate report cards\n0. Back\n";
            choice = readNumber("Choice: ");

            if (choice == 1) addStudent();
            else if (choice == 2) searchSubject();
            else if (choice == 3) correctMarks();
            else if (choice == 4) history.show();
            else if (choice == 5) generate();
            else if (choice != 0) cout << "Invalid choice.\n";
        } while (choice != 0);
    }
};

#endif
