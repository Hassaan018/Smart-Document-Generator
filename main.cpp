#include "certificate.h"
#include "resume.h"
#include "reportcard.h"
#include "invitation.h"
#include "portfolio.h"

// ANSI color codes (used only for the console text in this file)
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";

// Prints the program header
void showHeader()
{
    cout << "\n" << CYAN << BOLD;
    cout << "  ============================================\n";
    cout << "  ||                                        ||\n";
    cout << "  ||         SMART DOCUMENT GENERATOR       ||\n";
    cout << "  ||                                        ||\n";
    cout << "  ============================================\n" << RESET;
    cout << YELLOW << "   Certificates | Resumes | Report Cards\n";
    cout << "        Invitations | Portfolios\n" << RESET;
    cout << CYAN << "  --------------------------------------------\n" << RESET;
}

// =====================================================
// MAIN MENU
// A queue holds the documents requested in this session and
// generates them in the order they were requested.
// =====================================================
int main()
{
    system("");     // lets Windows consoles understand the color codes
    string documentNames[5] = {"Certificate", "Resume", "Report Card", "Invitation", "Portfolio"};

    CertificateGenerator certificate;
    ResumeGenerator resume;
    ReportCardGenerator reportCard;
    InvitationGenerator invitation;
    PortfolioGenerator portfolio;
    StringQueue requests;
    int choice;

    do
    {
        showHeader();
        for (int i = 0; i < 5; i++) {
            cout << GREEN << "  " << i + 1 << ". " << RESET << documentNames[i] << " Generator\n";
        }
        cout << GREEN << "  6. " << RESET << "Add a document to the generation queue\n"
             << GREEN << "  7. " << RESET << "Generate queued documents (first come, first served)\n"
             << RED << "  0. " << RESET << "Exit\n";

        choice = readNumber(CYAN + "  Choice: " + RESET);

        if (choice == 1) certificate.run();
        else if (choice == 2) resume.run();
        else if (choice == 3) reportCard.run();
        else if (choice == 4) invitation.run();
        else if (choice == 5) portfolio.run();
        else if (choice == 6)
        {
            int type = readRange("Which document (1 to 5)? ", 1, 5);
            if (requests.enqueue(documentNames[type - 1]))
                cout << GREEN << documentNames[type - 1] << " added to the queue." << RESET << "\n";
            else
                cout << RED << "Queue is full." << RESET << "\n";
        }
        else if (choice == 7)
        {
            if (requests.isEmpty())
                cout << YELLOW << "The queue is empty." << RESET << "\n";
            while (!requests.isEmpty())
            {
                string type = requests.dequeue();
                cout << YELLOW << "Processing " << type << "..." << RESET << "\n";
                if (type == documentNames[0]) certificate.generate();
                else if (type == documentNames[1]) resume.generate();
                else if (type == documentNames[2]) reportCard.generate();
                else if (type == documentNames[3]) invitation.generate();
                else portfolio.generate();
            }
        }
        else if (choice != 0)
            cout << RED << "Invalid choice." << RESET << "\n";
    } while (choice != 0);

    cout << GREEN << BOLD << "Goodbye!" << RESET << "\n";
    return 0;
}