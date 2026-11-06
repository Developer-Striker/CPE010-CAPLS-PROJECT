#include <iostream>
#include <iomanip>
#include <limits>
#include <cstddef>
#include <string>
#include "ComputerLabSystem.h"

namespace
{
    void printHeader()
    {
        std::cout << "\n+--------------------------------------------------------------+\n"
                  << "|           COMPUTER LAB INCIDENT MANAGEMENT SYSTEM            |\n"
                  << "+--------------------------------------------------------------+\n";
    }

    void printSectionHeader(const char *title)
    {
        std::cout << "\n+--------------------------------------------------------------+\n"
                  << "| " << std::left << std::setw(61) << title << "|\n"
                  << "+--------------------------------------------------------------+\n";
    }

    void printIncidentEntryHeader()
    {
        printSectionHeader("SYSTEM ISSUE INCIDENT ENTRY");
        std::cout << "\n  [Categories]  0: Hardware  |  1: Software  |  2: Network\n"
                  << "  [Priorities]  1: Urgent    |  2: Normal    |  3: Low\n\n"
                  << "  --------------------------------------------------------------\n";
    }

    void printStatusEntryHeader()
    {
        printSectionHeader("SELECT NEW INCIDENT STATUS");
        std::cout << "\n  [Statuses]  0: Pending  |  1: In Progress  |  2: Resolved\n\n"
                  << "  --------------------------------------------------------------\n";
    }

    void printMenu()
    {
        std::cout << "\n  INCIDENTS\n"
                  << "  [1] Add new incident\n"
                  << "  [2] Process next urgent incident\n"
                  << "  [3] Update incident status\n"
                  << "  [4] Undo last update\n"
                  << "\n  REPORTS\n"
                  << "  [5] View computer history\n"
                  << "  [6] View problematic computers\n"
                  << "  [7] View category summary\n"
                  << "  [8] Sort and view all records\n"
                  << "\n  [0] Exit\n";
    }

    // Added for the space consistency
    void printEntryPrompt(const char *label)
    {
        std::cout << "  " << std::left << std::setw(20) << label << " : ";
    }

    bool readNumber(const char *prompt, int &value)
    {
        printEntryPrompt(prompt);
        if (std::cin >> value)
            return true;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n  Please enter a number.\n";
        return false;
    }
}

int main()
{
    ComputerLabSystem sys;
    int choice = -1;

    do
    {
        printHeader();
        std::cout << "       [Track, prioritize, and review computer lab issues]\n";
        printMenu();
        std::cout << "  " << std::left << std::setw(20) << "Select an option" << " : ";
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n  Please enter a number from the menu.\n";
            continue;
        }

        if (choice == 1)
        {
            int compNum, cat, priority;
            std::string date, desc;
            printIncidentEntryHeader();
            printEntryPrompt("Computer Number");
            if (!(std::cin >> compNum))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n  Please enter a number.\n";
                continue;
            }
            printEntryPrompt("Category [0-2]");
            if (!(std::cin >> cat))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n  Please enter a number.\n";
                continue;
            }
            if (cat < 0 || cat > 2)
            {
                std::cout << "  Invalid category.\n";
                continue;
            }
            printEntryPrompt("Priority [1-3]");
            if (!(std::cin >> priority))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n  Please enter a number.\n";
                continue;
            }
            if (priority < 1 || priority > 3)
            {
                std::cout << "  Invalid priority.\n";
                continue;
            }
            printEntryPrompt("Date (YYYY-MM-DD)");
            std::cin >> date;
            printEntryPrompt("Description");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, desc);
            sys.addIncident(compNum, static_cast<Category>(cat), priority, date, desc);
        }
        else if (choice == 2)
            sys.processNextIncident();
        else if (choice == 3)
        {
            int id, status;
            if (!sys.displayIncidentStatusOptions())
                continue;
            if (!readNumber("Incident ID", id))
                continue;
            printStatusEntryHeader();
            printEntryPrompt("New Status [0-2]");
            if (!(std::cin >> status))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n  Please enter a number.\n";
                continue;
            }
            if (status < 0 || status > 2)
            {
                std::cout << "  Invalid status.\n";
                continue;
            }
            sys.updateStatus(id, static_cast<Status>(status));
        }
        else if (choice == 4)
            sys.undoLastUpdate();
        else if (choice == 5)
        {
            int compNum;
            printSectionHeader("VIEW COMPUTER HISTORY");
            printEntryPrompt("Computer Number");
            if (std::cin >> compNum)
                sys.displayComputerHistory(compNum);
            else
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n  Please enter a number.\n";
            }
        }
        else if (choice == 6)
            sys.displayMostProblematicComputers();
        else if (choice == 7)
            sys.displayCategorySummary();
        else if (choice == 8)
        {
            int sortCat;
            printSectionHeader("SORT AND VIEW INCIDENTS");
            std::cout << "\n  [Sort By]  1: Priority  |  2: Date  |  3: Status\n\n"
                      << "  --------------------------------------------------------------\n";
            printEntryPrompt("Sort By [1-3]");
            if (!(std::cin >> sortCat))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n  Please enter a number.\n";
                continue;
            }
            if (sortCat < 1 || sortCat > 3)
            {
                std::cout << "  Invalid sort option.\n";
                continue;
            }
            sys.selectionSort(sortCat);
            sys.displayAll();
        }
        else if (choice != 0)
            std::cout << "  That option is not on the menu.\n";

        if (choice != 0)
            std::cout << "\n  --------------------------------------------------------\n";
    } while (choice != 0);

    std::cout << "\n  Goodbye.\n";
    return 0;
}
