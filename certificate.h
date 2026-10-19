#ifndef CERTIFICATE_H
#define CERTIFICATE_H

#include "helpers.h"
#include "dsa_library.h"

// =====================================================
// MODULE 1: CERTIFICATE GENERATOR
// Linked List = text lines, Queue = recipients, Array = recipient search,
// Stack = undo
// Undo codes: 1 title, 2 date, 3 line added, 4 line deleted,
//             5 name corrected, 6 recipient added
// =====================================================
class CertificateGenerator
{
private:
    string title;
    string date;
    string logoPath;
    string signaturePath;
    LinkedList lines;
    ItemArray recipients;
    ActionStack history;

    void writeCertificate(string name)
    {
        string css = ".page { border: 10px double #b8860b; text-align: center; min-height: 400px; }\n"
                     "h1 { color: #b8860b; font-size: 38px; }\n"
                     ".name { font-size: 30px; font-weight: bold; color: #17365d; margin: 15px 0; }\n"
                     ".signature { margin-top: 50px; }\n";
        ofstream file;
        string fileName = makeFileName("certificate_", name);
        if (!createPage(file, fileName, "Certificate", css))
            return;

        writeImage(file, logoPath, "logo");
        file << "<h1>" << makeSafe(title) << "</h1>\n";
        file << "<p>This certificate is proudly presented to</p>\n";
        file << "<div class='name'>" << makeSafe(name) << "</div>\n";
        for (int i = 1; i <= lines.size(); i++)
            file << "<p>" << makeSafe(lines.getNode(i)->title) << "</p>\n";
        file << "<p>Date: " << makeSafe(date) << "</p>\n";
        file << "<div class='signature'>\n";
        writeImage(file, signaturePath, "signature");
        file << "<p>________________________</p>\n<p>Authorized Signature</p>\n</div>\n";
        finishPage(file, fileName);
    }

    void addRecipient()
    {
        string name = readText("Recipient name: ");
        if (name == "")
            return;
        if (recipients.search(name) != -1)
            cout << "This recipient already exists.\n";
        else if (!recipients.add(name))
            cout << "Recipient list is full.\n";
        else
        {
            history.push("Added recipient " + name, 6, 0, name);
            cout << "Recipient added.\n";
        }
    }

    // Linear search, then the certificate can be corrected and regenerated
    void searchRecipient()
    {
        string name = readText("Name to search: ");
        int index = recipients.search(name);
        if (index == -1)
        {
            cout << "Recipient not found.\n";
            return;
        }
        cout << "Found at position " << index + 1 << ".\n";
        string newName = readText("Type the corrected name (or press Enter to keep it): ");
        if (newName != "")
        {
            history.push("Corrected name " + name + " to " + newName, 5, 0, name, newName);
            recipients.setName(index, newName);
            name = newName;
        }
        writeCertificate(name);
    }

    void addLine()
    {
        string line = readText("Certificate text line: ");
        int position = readNumber("Position to insert at (0 = at the end): ");
        if (position == 0)
            position = lines.size() + 1;
        position = lines.insertAt(position, line, "", "", "");
        history.push("Added line: " + line, 3, position);
    }

    void deleteLine()
    {
        lines.show();
        int position = readNumber("Line number to delete: ");
        Node* node = lines.getNode(position);
        if (node == NULL)
        {
            cout << "Invalid line number.\n";
            return;
        }
        history.push("Deleted line: " + node->title, 4, position, node->title);
        lines.removeAt(position);
    }

    void undo()
    {
        Action last;
        if (!history.pop(last))
        {
            cout << "Nothing to undo.\n";
            return;
        }
        if (last.field == 1) title = last.oldTitle;
        else if (last.field == 2) date = last.oldTitle;
        else if (last.field == 3) lines.removeAt(last.position);
        else if (last.field == 4) lines.insertAt(last.position, last.oldTitle, "", "", "");
        else if (last.field == 5)
        {
            int index = recipients.search(last.oldText);
            if (index != -1) recipients.setName(index, last.oldTitle);
        }
        else if (last.field == 6)
        {
            int index = recipients.search(last.oldTitle);
            if (index != -1) recipients.removeAt(index);
        }
        cout << "Undone: " << last.description << "\n";
    }

public:
    // Recipients go into a queue and certificates are made in the same order
    void generate()
    {
        if (recipients.size() == 0)
        {
            cout << "Add at least one recipient first.\n";
            return;
        }
        StringQueue queue;
        for (int i = 0; i < recipients.size(); i++)
            queue.enqueue(recipients.getName(i));
        while (!queue.isEmpty())
            writeCertificate(queue.dequeue());
    }

    CertificateGenerator()
    {
        title = "Certificate of Achievement";
        date = "";
        logoPath = "";
        signaturePath = "";
    }

    void run()
    {
        if (date == "")
            date = readText("Certificate date: ");

        int choice;
        do
        {
            cout << "\n--- CERTIFICATE GENERATOR ---\n"
                 << "1. Add recipient\n2. Show recipients\n3. Search recipient (correct / regenerate)\n"
                 << "4. Sort recipients A-Z\n5. Add text line\n6. Delete text line\n7. Show text lines\n"
                 << "8. Change title\n9. Change date\n10. Set logo and signature images\n"
                 << "11. Show recent actions\n12. Undo last change\n13. Generate certificates\n0. Back\n";
            choice = readNumber("Choice: ");

            if (choice == 1) addRecipient();
            else if (choice == 2) recipients.show(false);
            else if (choice == 3) searchRecipient();
            else if (choice == 4) { recipients.sortByName(); cout << "Sorted A to Z.\n"; }
            else if (choice == 5) addLine();
            else if (choice == 6) deleteLine();
            else if (choice == 7) lines.show();
            else if (choice == 8) { history.push("Changed title", 1, 0, title); title = readText("New title: "); }
            else if (choice == 9) { history.push("Changed date", 2, 0, date); date = readText("New date: "); }
            else if (choice == 10)
            {
                logoPath = readText("Logo image path (Enter to skip): ");
                signaturePath = readText("Signature image path (Enter to skip): ");
            }
            else if (choice == 11) history.show();
            else if (choice == 12) undo();
            else if (choice == 13) generate();
            else if (choice != 0) cout << "Invalid choice.\n";
        } while (choice != 0);
    }
};

#endif
