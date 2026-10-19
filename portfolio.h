#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "helpers.h"
#include "dsa_library.h"

// =====================================================
// MODULE 5: PORTFOLIO GENERATOR
// Linked List = projects, Array = skills and achievements (sorted),
// Stack = recent activity / undo
// Undo codes: 1 project added, 11 project deleted, 2 skill added,
//             12 skill deleted, 3 achievement added
// =====================================================
class PortfolioGenerator
{
private:
    string name;
    string intro;
    string photoPath;
    LinkedList projects;
    ItemArray skills;           // skill name + level (1 to 5)
    ItemArray achievements;
    ActionStack history;

    void writePortfolio()
    {
        string css = "h1 { color: #34495e; }\n"
                     "h2 { color: #34495e; border-bottom: 2px solid #34495e; margin-top: 25px; }\n"
                     ".card { border: 1px solid #ccc; padding: 12px; margin-bottom: 12px; }\n";
        ofstream file;
        string fileName = makeFileName("portfolio_", name);
        if (!createPage(file, fileName, name + " - Portfolio", css))
            return;

        writeImage(file, photoPath, "photo");
        file << "<h1>" << makeSafe(name) << "</h1>\n<p>" << makeSafe(intro) << "</p>\n";

        if (projects.size() > 0)
        {
            file << "<h2>Projects</h2>\n";
            for (int i = 1; i <= projects.size(); i++)
            {
                Node* node = projects.getNode(i);
                file << "<div class='card'>\n<h3>" << makeSafe(node->title) << "</h3>\n";
                writeImage(file, node->image, "project");
                file << "<p>" << makeSafe(node->text) << "</p>\n";
                file << "<p><i>Technology: " << makeSafe(node->extra) << "</i></p>\n</div>\n";
            }
        }
        if (skills.size() > 0)
        {
            file << "<h2>Skills</h2>\n<ul>\n";
            for (int i = 0; i < skills.size(); i++)
                file << "<li>" << makeSafe(skills.getName(i)) << " - level " << skills.getValue(i) << "/5</li>\n";
            file << "</ul>\n";
        }
        if (achievements.size() > 0)
        {
            file << "<h2>Achievements</h2>\n<ul>\n";
            for (int i = 0; i < achievements.size(); i++)
                file << "<li>" << makeSafe(achievements.getName(i)) << "</li>\n";
            file << "</ul>\n";
        }
        finishPage(file, fileName);
    }

    void addProject()
    {
        string title = readText("Project title: ");
        string description = readText("Description: ");
        string technology = readText("Technology used: ");
        string image = readText("Image path (Enter to skip): ");
        int position = projects.add(title, description, technology, image);
        history.push("Added project " + title, 1, position);
    }

    // Linear search by title or technology, then view or delete the project
    void searchProject()
    {
        int position = projects.search(readText("Title or technology to search: "));
        if (position == 0)
        {
            cout << "No matching project.\n";
            return;
        }
        Node* node = projects.getNode(position);
        cout << "Found: " << node->title << " - " << node->text << " (" << node->extra << ")\n";
        if (readText("Delete this project? (y/n): ") == "y")
        {
            history.push("Deleted project " + node->title, 11, position, node->title, node->text,
                         node->extra, node->image);
            projects.removeAt(position);
        }
    }

    void addSkill()
    {
        string skill = readText("Skill name: ");
        int level = readRange("Level (1 to 5): ", 1, 5);
        if (skills.search(skill) != -1)
            cout << "This skill already exists.\n";
        else if (skills.add(skill, level))
            history.push("Added skill " + skill, 2, 0, skill);
        else
            cout << "Skill list is full.\n";
    }

    void removeSkill()
    {
        skills.show(true);
        string skill = readText("Skill to remove: ");
        int index = skills.search(skill);
        if (index == -1)
        {
            cout << "Skill not found.\n";
            return;
        }
        history.push("Removed skill " + skill, 12, 0, skill, toText(skills.getValue(index)));
        skills.removeAt(index);
    }

    void addAchievement()
    {
        string text = readText("Achievement: ");
        if (text != "" && achievements.add(text))
            history.push("Added achievement " + text, 3, 0, text);
    }

    void undo()
    {
        Action last;
        if (!history.pop(last))
        {
            cout << "Nothing to undo.\n";
            return;
        }
        if (last.field == 1) projects.removeAt(last.position);
        else if (last.field == 11)
            projects.insertAt(last.position, last.oldTitle, last.oldText, last.oldExtra, last.oldImage);
        else if (last.field == 12) skills.add(last.oldTitle, atoi(last.oldText.c_str()));
        else
        {
            ItemArray* list = &skills;
            if (last.field == 3) list = &achievements;
            int index = list->search(last.oldTitle);
            if (index != -1) list->removeAt(index);
        }
        cout << "Undone: " << last.description << "\n";
    }

    void showAll()
    {
        cout << "Projects:\n";
        projects.show();
        cout << "Skills:\n";
        skills.show(true);
        cout << "Achievements:\n";
        achievements.show(false);
    }

public:
    PortfolioGenerator()
    {
        name = "";
    }

    void generate()
    {
        if (name == "")
            cout << "Open the Portfolio module and enter your details first.\n";
        else
            writePortfolio();
    }

    void run()
    {
        if (name == "")
        {
            name = readText("Your name: ");
            intro = readText("Short introduction: ");
            photoPath = readText("Photo path (Enter to skip): ");
        }

        int choice;
        do
        {
            cout << "\n--- PORTFOLIO GENERATOR ---\n"
                 << "1. Add project\n2. Search project (view / delete)\n3. Add skill\n4. Remove skill\n"
                 << "5. Sort skills\n6. Add achievement\n7. Show everything\n8. Show recent activity\n"
                 << "9. Undo last action\n10. Generate portfolio\n0. Back\n";
            choice = readNumber("Choice: ");

            if (choice == 1) addProject();
            else if (choice == 2) searchProject();
            else if (choice == 3) addSkill();
            else if (choice == 4) removeSkill();
            else if (choice == 5)
            {
                if (readRange("1. Sort A-Z   2. Sort by level\nChoice: ", 1, 2) == 1) skills.sortByName();
                else skills.sortByValue();
                skills.show(true);
            }
            else if (choice == 6) addAchievement();
            else if (choice == 7) showAll();
            else if (choice == 8) history.show();
            else if (choice == 9) undo();
            else if (choice == 10) generate();
            else if (choice != 0) cout << "Invalid choice.\n";
        } while (choice != 0);
    }
};

#endif
