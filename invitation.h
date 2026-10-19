#ifndef INVITATION_H
#define INVITATION_H

#include "helpers.h"
#include "dsa_library.h"

// =====================================================
// MODULE 4: INVITATION CARD GENERATOR
// Linked List = event details, Queue = guests, Array = guest search,
// Stack = undo for venue / date / time
// Undo codes: 1 venue, 2 date, 3 time
// =====================================================
class InvitationGenerator
{
private:
    string host;
    string eventName;
    string venue;
    string date;
    string eventTime;
    string imagePath;
    LinkedList details;
    ItemArray guests;
    ActionStack history;

    void writeInvitation(string guest)
    {
        string css = "body { background: #f2eee5; color: #294638; }\n"
                     ".page { border: 7px double #b18a4b; text-align: center; background: #fffdf7; }\n"
                     "h1 { font-size: 38px; color: #87652f; }\n"
                     "ul { text-align: left; }\n";
        ofstream file;
        string fileName = makeFileName("invitation_", guest);
        if (!createPage(file, fileName, eventName, css))
            return;

        writeImage(file, imagePath, "logo");
        file << "<p>YOU ARE INVITED</p>\n<h1>" << makeSafe(eventName) << "</h1>\n";
        file << "<p>Hosted by " << makeSafe(host) << "</p>\n";
        file << "<h2>Dear " << makeSafe(guest) << ",</h2>\n";
        file << "<p>We would be delighted by your presence.</p>\n";
        file << "<p><b>Date:</b> " << makeSafe(date) << "<br>\n";
        file << "<b>Time:</b> " << makeSafe(eventTime) << "<br>\n";
        file << "<b>Venue:</b> " << makeSafe(venue) << "</p>\n";
        if (details.size() > 0)
        {
            file << "<h3>Event Details</h3>\n<ul>\n";
            for (int i = 1; i <= details.size(); i++)
            {
                Node* node = details.getNode(i);
                file << "<li><b>" << makeSafe(node->title) << ":</b> " << makeSafe(node->text) << "</li>\n";
            }
            file << "</ul>\n";
        }
        file << "<p>We look forward to seeing you!</p>\n";
        finishPage(file, fileName);
    }

    void addGuest()
    {
        string guest = readText("Guest name: ");
        if (guest == "")
            return;
        if (guests.search(guest) != -1)
            cout << "This guest already exists.\n";
        else if (!guests.add(guest))
            cout << "Guest list is full.\n";
        else
            cout << "Guest added.\n";
    }

    // Linear search in the array, then the invitation can be regenerated
    void searchGuest()
    {
        string guest = readText("Guest name to search: ");
        int index = guests.search(guest);
        if (index == -1)
        {
            cout << "Guest not found.\n";
            return;
        }
        cout << "Found at position " << index + 1 << ".\n";
        if (readText("Regenerate this invitation? (y/n): ") == "y")
            writeInvitation(guest);
    }

    void addDetail()
    {
        string title = readText("Detail type (Schedule, Dress code, Note, Agenda...): ");
        string text = readText("Detail: ");
        if (title != "" && text != "")
            details.add(title, text);
    }

    void removeDetail()
    {
        details.show();
        if (!details.removeAt(readNumber("Detail number to remove: ")))
            cout << "Invalid number.\n";
    }

    void changeDetail(int field)
    {
        string newValue = readText("Enter new value: ");
        if (field == 1)
        {
            history.push("Changed venue", 1, 0, venue);
            venue = newValue;
        }
        else if (field == 2)
        {
            history.push("Changed date", 2, 0, date);
            date = newValue;
        }
        else
        {
            history.push("Changed time", 3, 0, eventTime);
            eventTime = newValue;
        }
    }

    void undo()
    {
        Action last;
        if (!history.pop(last))
        {
            cout << "Nothing to undo.\n";
            return;
        }
        if (last.field == 1) venue = last.oldTitle;
        else if (last.field == 2) date = last.oldTitle;
        else eventTime = last.oldTitle;
        cout << "Undone: " << last.description << "\n";
    }

public:
    InvitationGenerator()
    {
        host = "";
    }

    // Guests go into a queue and invitations are made in the same order
    void generate()
    {
        if (host == "" || guests.size() == 0)
        {
            cout << "Enter event details and add at least one guest first.\n";
            return;
        }
        StringQueue queue;
        for (int i = 0; i < guests.size(); i++)
            queue.enqueue(guests.getName(i));
        while (!queue.isEmpty())
            writeInvitation(queue.dequeue());
    }

    void run()
    {
        if (host == "")
        {
            host = readText("Host name: ");
            eventName = readText("Event name: ");
            venue = readText("Venue: ");
            date = readText("Date: ");
            eventTime = readText("Time: ");
        }

        int choice;
        do
        {
            cout << "\n--- INVITATION GENERATOR ---\n"
                 << "1. Add event detail\n2. Remove event detail\n3. Move detail up\n4. Show details\n"
                 << "5. Add guest\n6. Show guests\n7. Search guest\n8. Change venue\n9. Change date\n"
                 << "10. Change time\n11. Show recent changes\n12. Undo last change\n"
                 << "13. Set image / logo\n14. Generate invitations\n0. Back\n";
            choice = readNumber("Choice: ");

            if (choice == 1) addDetail();
            else if (choice == 2) removeDetail();
            else if (choice == 3) { details.show(); details.moveUp(readNumber("Detail number to move up: ")); }
            else if (choice == 4) details.show();
            else if (choice == 5) addGuest();
            else if (choice == 6) guests.show(false);
            else if (choice == 7) searchGuest();
            else if (choice >= 8 && choice <= 10) changeDetail(choice - 7);
            else if (choice == 11) history.show();
            else if (choice == 12) undo();
            else if (choice == 13) imagePath = readText("Image path (Enter to skip): ");
            else if (choice == 14) generate();
            else if (choice != 0) cout << "Invalid choice.\n";
        } while (choice != 0);
    }
};

#endif
