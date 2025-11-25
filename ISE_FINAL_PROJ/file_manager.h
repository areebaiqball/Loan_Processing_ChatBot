#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include "application.h"

using namespace std;

class FileManager {
private:
    string applicationsFile;
    string imagesDirectory;

    bool fileExists(const string& filename) const;
    bool copyImageFile(const string& sourcePath, const string& destinationPath) const;
    string applicationToFileFormat(const LoanApplication& application) const;
    LoanApplication applicationFromFileFormat(const vector<string>& parts) const;

public:
    FileManager(const string& appsFile = "applications.txt",
        const string& imagesDir = "images/");

    string generateApplicationId() const;

    bool saveApplication(LoanApplication& application);
    bool updateApplicationSection(const LoanApplication& application, const string& section);
    vector<LoanApplication> loadAllApplications() const;
    LoanApplication findApplicationById(const string& applicationId) const;

    LoanApplication findIncompleteApplication(const string& applicationId, const string& cnic) const;
    vector<LoanApplication> findUserIncompleteApplications(const string& cnic) const;
    bool canUpdateApplication(const string& applicationId, const string& cnic) const;

    vector<LoanApplication> findApplicationsByCNIC(const string& cnic) const;
    vector<LoanApplication> loadAllApplicationsDetailed() const;

    void getApplicationStatsByCNIC(const string& cnic,
        int& submitted, int& approved, int& rejected) const;
    bool updateApplicationStatus(const string& applicationId, const string& newStatus, const string& rejectionReason = "");
};

#endif