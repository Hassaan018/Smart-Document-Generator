#ifndef RESUME_H
#define RESUME_H

#include "helpers.h"
#include "dsa_library.h"

// =====================================================
// MODULE 2: RESUME / CV GENERATOR
// Linked List = education, experience, projects
// Array = skills with rating (sorted + searched), Stack = undo
// Undo codes: 1-3 entry added (section number), 11-13 entry deleted,
//             4 skill added, 5 skill deleted, 6 skill rating changed
// =====================================================
class ResumeGenerator
{
private:
    string name;
    string email;
    string phone;
    string summary;
    string photoPath;
    LinkedList education;
    LinkedList experience;
    LinkedList projects;
    ItemArray skills;
    ActionStack history;

    LinkedList* getList(int section)
    {
        if (section == 1) return &education;
        if (section == 2) return &experience;
        return &projects;
    }

    string sectionName(int section)
    {
        if (section == 1) return "Education";
        if (section == 2) return "Experience";
        return "Projects";
    }

    void writeSection(ofstream& file, string heading, LinkedList& list)
    {
        if (list.size() == 0)
            return;
        file << "<h2>" << heading << "</h2>\n";
        for (int i = 1; i <= list.size(); i++)
        {
            Node* node = list.getNode(i);
            file << "<p><b>" << makeSafe(node->title) << "</b>";
            if (node->text != "") file << ", " << makeSafe(node->text);
            if (node->extra != "") file << " <i>(" << makeSafe(node->extra) << ")</i>";
            file << "</p>\n";
        }
    }

    void writeResume()
    {
        string css = "h1 { margin-bottom: 0; color: #2c3e50; }\n"
                     "h2 { color: #2c3e50; border-bottom: 2px solid #2c3e50; margin-top: 25px; }\n"
                     ".bar { background: #ddd; height: 8px; width: 200px; margin-bottom: 8px; }\n"
                     ".bar div { background: #2c3e50; height: 8px; }\n";
        ofstream file;
        string fileName = makeFileName("resume_", name);
        if (!createPage(file, fileName, name + " - Resume", css))
            return;

        writeImage(file, photoPath, "photo");
        file << "<h1>" << makeSafe(name) << "</h1>\n";
        file << "<p>" << makeSafe(email) << " | " << makeSafe(phone) << "</p>\n";
        if (summary != "")
            file << "<p>" << makeSafe(summary) << "</p>\n";
        writeSection(file, "Education", education);
        writeSection(file, "Work Experience", experience);
        writeSection(file, "Projects", projects);

        if (skills.size() > 0)
        {
            skills.sortByValue();       // strongest skill first
            file << "<h2>Skills</h2>\n";
            for (int i = 0; i < skills.size(); i++)
            {
                file << "<p>" << makeSafe(skills.getName(i)) << " (" << skills.getValue(i) << "/5)</p>\n";
                file << "<div class='bar'><div style='width:" << skills.getValue(i) * 20 << "%'></div></div>\n";
            }
        }
        finishPage(file, fileName);
    }

    void addEntry()
    {
        int section = readRange("1. Education  2. Experience  3. Project\nSection: ", 1, 3);
        string title, text, extra;
        if (section == 1)
        {
            title = readText("Degree: ");
            text = readText("Institute: ");
            extra = readText("Year: ");
        }
        else if (section == 2)
        {
            title = readText("Job title: ");
            text = readText("Company: ");
            extra = readText("Duration: ");
        }
        else
        {
            title = readText("Project title: ");
            text = readText("Description: ");
        }
        int position = getList(section)->add(title, text, extra);
        history.push("Added " + title + " to " + sectionName(section), section, position);
    }

    void deleteEntry()
    {
        int section = readRange("1. Education  2. Experience  3. Project\nSection: ", 1, 3);
        getList(section)->show();
        int position = readNumber("Entry number to delete: ");
        Node* node = getList(section)->getNode(position);
        if (node == NULL)
        {
            cout << "Invalid entry number.\n";
            return;
        }
        history.push("Deleted " + node->title + " from " + sectionName(section), section + 10, position,
                     node->title, node->text, node->extra);
        getList(section)->removeAt(position);
    }

    // A skill that already exists is not added again, only its rating is updated
    void addSkill()
    {
        string skill = readText("Skill name: ");
        if (skill == "")
            return;
        int rating = readRange("Rating (1 to 5): ", 1, 5);
        int index = skills.search(skill);
        if (index == -1)
        {
            if (skills.add(skill, rating))
                history.push("Added skill " + skill, 4, 0, skill);
            else
                cout << "Skill list is full.\n";
        }
        else
        {
            history.push("Changed rating of " + skill, 6, 0, skill, toText(skills.getValue(index)));
            skills.setValue(index, rating);
            cout << "Skill already existed, rating updated.\n";
        }
    }

    void deleteSkill()
    {
        skills.show(true);
        string skill = readText("Skill to delete: ");
        int index = skills.search(skill);
        if (index == -1)
        {
            cout << "Skill not found.\n";
            return;
        }
        history.push("Deleted skill " + skill, 5, 0, skill, toText(skills.getValue(index)));
        skills.removeAt(index);
    }

    void searchSkill()
    {
        int index = skills.search(readText("Skill to search: "));
        if (index == -1)
            cout << "Skill not found.\n";
        else
            cout << "Found: " << skills.getName(index) << " - rating " << skills.getValue(index) << "/5\n";
    }

    void undo()
    {
        Action last;
        if (!history.pop(last))
        {
            cout << "Nothing to undo.\n";
            return;
        }
        int type = last.field;
        if (type >= 1 && type <= 3)
            getList(type)->removeAt(last.position);
        else if (type >= 11 && type <= 13)
            getList(type - 10)->insertAt(last.position, last.oldTitle, last.oldText, last.oldExtra, "");
        else if (type == 5)
            skills.add(last.oldTitle, atoi(last.oldText.c_str()));
        else
        {
            int index = skills.search(last.oldTitle);
            if (index != -1 && type == 4) skills.removeAt(index);
            if (index != -1 && type == 6) skills.setValue(index, atoi(last.oldText.c_str()));
        }
        cout << "Undone: " << last.description << "\n";
    }

    void showAll()
    {
        cout << "Education:\n";
        education.show();
        cout << "Experience:\n";
        experience.show();
        cout << "Projects:\n";
        projects.show();
        cout << "Skills:\n";
        skills.show(true);
    }

public:
    ResumeGenerator()
    {
        name = "";
    }

    void generate()
    {
        if (name == "")
            cout << "Open the Resume module and enter your details first.\n";
        else
            writeResume();
    }

    void run()
    {
        if (name == "")
        {
            name = readText("Full name: ");
            email = readText("Email: ");
            phone = readText("Phone: ");
            summary = readText("Short summary (Enter to skip): ");
            photoPath = readText("Photo path (Enter to skip): ");
        }

        int choice;
        do
        {
            cout << "\n--- RESUME / CV GENERATOR ---\n"
                 << "1. Add education / experience / project\n2. Delete an entry\n3. Show everything\n"
                 << "4. Add or update skill\n5. Delete skill\n6. Sort skills (strongest first)\n"
                 << "7. Search skill\n8. Show recent actions\n9. Undo last action\n10. Generate resume\n0. Back\n";
            choice = readNumber("Choice: ");

            if (choice == 1) addEntry();
            else if (choice == 2) deleteEntry();
            else if (choice == 3) showAll();
            else if (choice == 4) addSkill();
            else if (choice == 5) deleteSkill();
            else if (choice == 6) { skills.sortByValue(); skills.show(true); }
            else if (choice == 7) searchSkill();
            else if (choice == 8) history.show();
            else if (choice == 9) undo();
            else if (choice == 10) generate();
            else if (choice != 0) cout << "Invalid choice.\n";
        } while (choice != 0);
    }
};

#endif