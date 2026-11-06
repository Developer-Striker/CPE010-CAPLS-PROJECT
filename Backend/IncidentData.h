#ifndef INCIDENT_DATA_H
#define INCIDENT_DATA_H

#include <string>

enum Category
{
    HARDWARE,
    SOFTWARE,
    NETWORK
};
enum Status
{
    PENDING,
    IN_PROGRESS,
    RESOLVED
};

struct Incident
{
    int id;
    int compNum;
    Category category;
    int priority;
    Status status;
    std::string date;
    std::string description;
};

// Helper functions for displaying enums
inline std::string getCatString(Category cat)
{
    if (cat == HARDWARE)
        return "Hardware";
    if (cat == SOFTWARE)
        return "Software";
    return "Network";
}

inline std::string getStatusString(Status status)
{
    if (status == PENDING)
        return "Pending";
    if (status == IN_PROGRESS)
        return "In Progress";
    return "Resolved";
}

#endif
