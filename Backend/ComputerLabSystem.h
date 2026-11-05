#ifndef COMPUTER_LAB_SYSTEM_H
#define COMPUTER_LAB_SYSTEM_H

#include <vector>
#include <stack>
#include "IncidentData.h"

class ComputerLabSystem {
private:
    std::vector<Incident> database;
    std::stack<Incident> undoStack;
    int nextId;

public:
    ComputerLabSystem(); // Constructor

    // Core Features
    void addIncident(int compNum, Category cat, int priority, const std::string& date, const std::string& desc);
    int searchIncident(int searchId);
    void updateStatus(int id, Status newStatus);
    void undoLastUpdate();
    void processNextIncident();
    
    // Analytics & Display Features
    void displayComputerHistory(int compNum);
    void displayMostProblematicComputers();
    void displayCategorySummary();
    void selectionSort(int criteria);
    void displayAll();
    void printIncident(const Incident& inc);
};

#endif
