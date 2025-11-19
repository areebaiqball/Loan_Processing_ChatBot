#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include "application.h"

using namespace std;

// SE Principles: Single Responsibility, Encapsulation, Facade Pattern

class FileManager {
private:
    string applicationsFile;
    string imagesDirectory;

    string generateApplicationId() const;
    bool fileExists(const string& filename) const;
    bool copyImageFile(const string& sourcePath, const string& destinationPath) const;

public:
    FileManager(const string& appsFile = "applications.txt",
        const string& imagesDir = "images/");

    // Core file operations
    bool saveApplication(LoanApplication& application);
    vector<LoanApplication> loadAllApplications() const;
    LoanApplication findApplicationById(const string& applicationId) const;

    // Search and filter operations
    vector<LoanApplication> findApplicationsByCNIC(const string& cnic) const;
    vector<LoanApplication> loadAllApplicationsDetailed() const;

    // Statistics and status
    void getApplicationStatsByCNIC(const string& cnic,
        int& submitted, int& approved, int& rejected) const;
    bool updateApplicationStatus(const string& applicationId, const string& newStatus, const string& rejectionReason = "");
};

    


#endif