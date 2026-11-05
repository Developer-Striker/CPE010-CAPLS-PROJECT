#include <iostream>
#include <limits>
#include <string>
#include "ComputerLabSystem.h"

namespace {

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void displayMenu() {
    std::cout << "\n=== Computer Lab Problem Log System ===\n"
              << "1. Add New Incident\n"
              << "2. Process Next Urgent Incident\n"
              << "3. Update Incident Status\n"
              << "4. Undo Last Update\n"
              << "5. View Computer History\n"
              << "6. View Problematic Computers\n"
              << "7. View Category Summary\n"
              << "8. Sort & View All Records\n"
              << "0. Exit\n"
              << "Select option: ";
}

void addIncident(ComputerLabSystem& system) {
    int compNum, category, priority;
    std::string date, description;

    std::cout << "Computer Number: ";
    std::cin >> compNum;
    std::cout << "Category (0=HW, 1=SW, 2=NW): ";
    std::cin >> category;
    std::cout << "Priority (1=Urgent, 2=Normal, 3=Low): ";
    std::cin >> priority;
    std::cout << "Date (YYYY-MM-DD): ";
    std::cin >> date;
    std::cout << "Description: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, description);

    if (category < HARDWARE || category > NETWORK) {
        std::cout << "Invalid category. Incident was not added.\n";
        return;
    }
    system.addIncident(compNum, static_cast<Category>(category), priority, date, description);
}

void updateIncidentStatus(ComputerLabSystem& system) {
    int id, status;
    std::cout << "Incident ID to update: ";
    std::cin >> id;
    std::cout << "New Status (0=Pending, 1=In Progress, 2=Resolved): ";
    std::cin >> status;
    if (status < PENDING || status > RESOLVED) {
        std::cout << "Invalid status. Incident was not updated.\n";
        return;
    }
    system.updateStatus(id, static_cast<Status>(status));
}

void viewComputerHistory(ComputerLabSystem& system) {
    int compNum;
    std::cout << "Enter Computer Number: ";
    std::cin >> compNum;
    system.displayComputerHistory(compNum);
}

void sortAndViewRecords(ComputerLabSystem& system) {
    int sortCriteria;
    std::cout << "Sort by (1=Priority, 2=Date, 3=Status): ";
    std::cin >> sortCriteria;
    system.selectionSort(sortCriteria);
    system.displayAll();
}

void performAction(ComputerLabSystem& system, int choice) {
    switch (choice) {
        case 1: addIncident(system); break;
        case 2: system.processNextIncident(); break;
        case 3: updateIncidentStatus(system); break;
        case 4: system.undoLastUpdate(); break;
        case 5: viewComputerHistory(system); break;
        case 6: system.displayMostProblematicComputers(); break;
        case 7: system.displayCategorySummary(); break;
        case 8: sortAndViewRecords(system); break;
        default: std::cout << "Invalid option. Please choose from the menu.\n"; break;
    }
}

} // namespace

int main() {
    ComputerLabSystem system;

    int choice = -1;
    while (choice != 0) {
        displayMenu();
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a menu number.\n";
            waitForEnter();
            continue;
        }

        if (choice == 0) break;
        performAction(system, choice);
        waitForEnter();
    }

    return 0;
}
