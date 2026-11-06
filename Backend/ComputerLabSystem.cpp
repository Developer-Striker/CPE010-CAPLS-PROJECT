#include <iostream>
#include "ComputerLabSystem.h"

using namespace std;

// Constructor initializes the ID counter
ComputerLabSystem::ComputerLabSystem() {
    nextId = 1;
}

void ComputerLabSystem::addIncident(int compNum, Category cat, int priority, string date, string desc) {
    Incident newIncident = {nextId++, compNum, cat, priority, PENDING, date, desc};
    database.push_back(newIncident);
    cout << "Success: Incident #" << newIncident.id << " added.\n";
}

int ComputerLabSystem::searchIncident(int searchId) {
    cout << "[System Note: Performing Linear Search. O(N) complexity. Future BST implementation recommended for large databases.]\n";
    for (size_t i = 0; i < database.size(); i++) {
        if (database[i].id == searchId) {
            return i;
        }
    }
    return -1;
}

void ComputerLabSystem::updateStatus(int id, Status newStatus) {
    int index = searchIncident(id);
    if (index != -1) {
        undoStack.push(database[index]); 
        database[index].status = newStatus;
        cout << "Success: Incident #" << id << " updated to " << getStatusString(newStatus) << ".\n";
    } else {
        cout << "Error: Incident ID #" << id << " not found!\n";
    }
}

void ComputerLabSystem::undoLastUpdate() {
    if (undoStack.empty()) {
        cout << "No actions to undo.\n";
        return;
    }
    Incident previousState = undoStack.top();
    undoStack.pop();

    int index = searchIncident(previousState.id);
    if (index != -1) {
        database[index] = previousState;
        cout << "Undo successful: Incident #" << previousState.id << " reverted.\n";
    }
}

void ComputerLabSystem::processNextIncident() {
    if (database.empty()) {
        cout << "No incidents in the system.\n";
        return;
    }

    int highestPriorityIdx = -1;
    for (size_t i = 0; i < database.size(); i++) {
        if (database[i].status != RESOLVED) {
            if (highestPriorityIdx == -1 || database[i].priority < database[highestPriorityIdx].priority) {
                highestPriorityIdx = i;
            }
        }
    }

    if (highestPriorityIdx != -1) {
        cout << "\n--- Processing Next Urgent Incident ---\n";
        printIncident(database[highestPriorityIdx]);
        updateStatus(database[highestPriorityIdx].id, IN_PROGRESS);
    } else {
        cout << "All incidents are currently resolved.\n";
    }
}

void ComputerLabSystem::displayComputerHistory(int compNum) {
    cout << "\n--- Incident History for Computer #" << compNum << " ---\n";
    bool found = false;
    for (const auto& inc : database) {
        if (inc.compNum == compNum) {
            printIncident(inc);
            found = true;
        }
    }
    if (!found) cout << "No incidents found for this computer.\n";
}

void ComputerLabSystem::displayMostProblematicComputers() {
    const int MAX_COMPUTERS = 100; 
    int frequencies[MAX_COMPUTERS] = {0}; 

    for (const auto& inc : database) {
        if (inc.compNum >= 0 && inc.compNum < MAX_COMPUTERS) {
            frequencies[inc.compNum]++;
        }
    }

    cout << "\n--- Problem Frequency by Computer ---\n";
    for (int i = 0; i < MAX_COMPUTERS; i++) {
        if (frequencies[i] > 0) {
            cout << "Computer #" << i << ": " << frequencies[i] << " incident(s)\n";
        }
    }
}

void ComputerLabSystem::displayCategorySummary() {
    int hw = 0, sw = 0, nw = 0;
    for (const auto& inc : database) {
        if (inc.category == HARDWARE) hw++;
        else if (inc.category == SOFTWARE) sw++;
        else if (inc.category == NETWORK) nw++;
    }
    cout << "\n--- Category Summary ---\n";
    cout << "Hardware: " << hw << " | Software: " << sw << " | Network: " << nw << "\n";
}

void ComputerLabSystem::selectionSort(int criteria) {
    for (size_t i = 0; i < database.size() - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < database.size(); j++) {
            bool shouldSwap = false;
            if (criteria == 1) shouldSwap = database[j].priority < database[min_idx].priority;
            else if (criteria == 2) shouldSwap = database[j].date < database[min_idx].date;
            else if (criteria == 3) shouldSwap = database[j].status < database[min_idx].status;

            if (shouldSwap) min_idx = j;
        }
        Incident temp = database[i];
        database[i] = database[min_idx];
        database[min_idx] = temp;
    }
    cout << "Records sorted successfully!\n";
}

void ComputerLabSystem::displayAll() {
    if (database.empty()) {
        cout << "No records found.\n";
        return;
    }
    for (const auto& inc : database) {
        printIncident(inc);
    }
}


void ComputerLabSystem::printIncident(const Incident& inc) {
    cout << "ID: " << inc.id << " | Comp: " << inc.compNum 
         << " | Cat: " << getCatString(inc.category) 
         << " | Priority: " << inc.priority 
         << " | Status: " << getStatusString(inc.status) 
         << " | Date: " << inc.date 
         << " | Desc: " << inc.description << "\n";
}
