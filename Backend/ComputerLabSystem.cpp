#include <iostream>
#include <iomanip>
#include "ComputerLabSystem.h"

namespace {
void printSectionHeader(const char* title) {
    std::cout << "\n+--------------------------------------------------------------+\n"
              << "| " << std::left << std::setw(61) << title << "|\n"
              << "+--------------------------------------------------------------+\n";
}

void printTableHeader() {
    std::cout << "  " << std::left
              << std::setw(6) << "ID"
              << std::setw(9) << "Computer"
              << std::setw(12) << "Category"
              << std::setw(10) << "Priority"
              << std::setw(15) << "Status"
              << std::setw(13) << "Date"
              << "Description\n"
              << "  " << std::string(90, '-') << "\n";
}

const char* getPriorityString(int priority) {
    if (priority == 1) return "Urgent";
    if (priority == 2) return "Normal";
    if (priority == 3) return "Low";
    return "Unknown";
}
}

ComputerLabSystem::ComputerLabSystem() : nextId(1) {}

void ComputerLabSystem::addIncident(int compNum, Category cat, int priority, std::string date, std::string desc) {
    Incident newIncident = {nextId, compNum, cat, priority, PENDING, date, desc};
    ++nextId;
    database.push_back(newIncident);
    std::cout << "\n  [OK] Incident #" << newIncident.id << " added successfully.\n";
}

int ComputerLabSystem::searchIncident(int searchId) {
    for (std::size_t i = 0; i < database.size(); i++) {
        if (database[i].id == searchId) {
            return i;
        }
    }
    return -1;
}

bool ComputerLabSystem::displayIncidentStatusOptions() {
    printSectionHeader("UPDATE INCIDENT STATUS");
    std::cout << "\n  " << std::left << std::setw(12) << "ID"
              << "CURRENT STATUS\n"
              << "  " << std::string(34, '-') << "\n";

    if (database.empty()) {
        std::cout << "  No incidents are currently available.\n";
        return false;
    }

    for (const auto& incident : database) {
        std::cout << "  " << std::left << std::setw(12) << incident.id
                  << getStatusString(incident.status) << "\n";
    }
    std::cout << "\n";
    return true;
}

void ComputerLabSystem::updateStatus(int id, Status newStatus) {
    int index = searchIncident(id);
    if (index != -1) {
        undoStack.push(database[index]); 
        database[index].status = newStatus;
        std::cout << "\n  [OK] Incident #" << id << " is now " << getStatusString(newStatus) << ".\n";
    } else {
        std::cout << "\n  [!] Incident #" << id << " was not found.\n";
    }
}

void ComputerLabSystem::undoLastUpdate() {
    printSectionHeader("UNDO LAST UPDATE");
    if (undoStack.empty()) {
        std::cout << "\n  [!] There is no update to undo.\n";
        return;
    }
    Incident previousState = undoStack.top();
    undoStack.pop();

    int index = searchIncident(previousState.id);
    if (index != -1) {
        database[index] = previousState;
        std::cout << "\n  [OK] Incident #" << previousState.id << " was restored to its previous status.\n";
    }
}

void ComputerLabSystem::processNextIncident() {
    printSectionHeader("PROCESS NEXT URGENT INCIDENT");
    if (database.empty()) {
        std::cout << "\n  No incidents are available to process.\n";
        return;
    }

    int highestPriorityIdx = -1;
    for (std::size_t i = 0; i < database.size(); i++) {
        if (database[i].status != RESOLVED) {
            if (highestPriorityIdx == -1 || database[i].priority < database[highestPriorityIdx].priority) {
                highestPriorityIdx = i;
            }
        }
    }

    if (highestPriorityIdx != -1) {
        printTableHeader();
        printIncident(database[highestPriorityIdx]);
        updateStatus(database[highestPriorityIdx].id, IN_PROGRESS);
    } else {
        std::cout << "\n  All incidents are resolved.\n";
    }
}

void ComputerLabSystem::displayComputerHistory(int compNum) {
    std::cout << "\n  Computer Number : " << compNum << "\n";
    printTableHeader();
    bool found = false;
    for (const auto& inc : database) {
        if (inc.compNum == compNum) {
            printIncident(inc);
            found = true;
        }
    }
    if (!found) std::cout << "  No incidents found for this computer.\n";
}

void ComputerLabSystem::displayMostProblematicComputers() {
    printSectionHeader("PROBLEMATIC COMPUTERS");
    const int MAX_COMPUTERS = 100;
    int frequencies[MAX_COMPUTERS] = {0};

    for (const auto &inc : database) {
        if (inc.compNum >= 0 && inc.compNum < MAX_COMPUTERS) {
            frequencies[inc.compNum]++;
        }
    }

    std::cout << "\n  " << std::left << std::setw(18) << "COMPUTER"
              << "INCIDENT COUNT\n"
              << "  " << std::string(34, '-') << "\n";
    bool found = false;
    for (int i = 0; i < MAX_COMPUTERS; i++) {
        if (frequencies[i] > 0) {
            std::cout << "  Computer #" << std::left << std::setw(8) << i
                      << frequencies[i] << "\n";
            found = true;
        }
    }
    if (!found)
        std::cout << "  No incident records found.\n";
}

void ComputerLabSystem::displayCategorySummary() {
    printSectionHeader("CATEGORY SUMMARY");
    int hw = 0, sw = 0, nw = 0;
    for (const auto &inc : database) {
        if (inc.category == HARDWARE)
            hw++;
        else if (inc.category == SOFTWARE)
            sw++;
        else if (inc.category == NETWORK)
            nw++;
    }
    std::cout << "\n  HARDWARE  " << std::setw(5) << hw
              << "  |  SOFTWARE  " << std::setw(5) << sw
              << "  |  NETWORK  " << nw << "\n";
}

void ComputerLabSystem::selectionSort(int criteria) {
    if (database.size() < 2) {
        std::cout << "\n  [OK] Records sorted.\n";
        return;
    }
    for (std::size_t i = 0; i < database.size() - 1; i++) {
        std::size_t min_idx = i;
        for (std::size_t j = i + 1; j < database.size(); j++) {
            bool shouldSwap = false;
            if (criteria == 1)
                shouldSwap = database[j].priority < database[min_idx].priority;
            else if (criteria == 2)
                shouldSwap = database[j].date < database[min_idx].date;
            else if (criteria == 3)
                shouldSwap = database[j].status < database[min_idx].status;

            if (shouldSwap)
                min_idx = j;
        }
        Incident temp = database[i];
        database[i] = database[min_idx];
        database[min_idx] = temp;
    }
    std::cout << "\n  [OK] Records sorted.\n";
}

void ComputerLabSystem::displayAll() {
    if (database.empty()) {
        std::cout << "\n  No incident records found.\n";
        return;
    }
    printSectionHeader("ALL INCIDENT RECORDS");
    printTableHeader();
    for (const auto& inc : database) {
        printIncident(inc);
    }
}

void ComputerLabSystem::printIncident(const Incident& inc) {
    const std::string description = inc.description.size() > 28
        ? inc.description.substr(0, 25) + "..."
        : inc.description;
    std::cout << "  " << std::left
              << std::setw(6) << inc.id
              << std::setw(9) << inc.compNum
              << std::setw(12) << getCatString(inc.category)
              << std::setw(10) << getPriorityString(inc.priority)
              << std::setw(15) << getStatusString(inc.status)
              << std::setw(13) << inc.date
              << description << "\n";
}

