#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include "application.h"

using namespace std;

/// <summary>
/// Manages file operations for loan applications and images
/// </summary>
class FileManager {
private:
    string applicationsFile;
    string imagesDirectory;

    /// <summary>
    /// Generates unique application ID
    /// </summary>
    string generateApplicationId() const;

    /// <summary>
    /// Checks if file exists
    /// </summary>
    bool fileExists(const string& filename) const;

    /// <summary>
    /// Copies image file from source to destination
    /// </summary>
    bool copyImageFile(const string& sourcePath, const string& destinationPath) const;

public:
    /// <summary>
    /// Constructor
    /// </summary>
    FileManager(const string& appsFile = "applications.txt",
        const string& imagesDir = "images/");

    /// <summary>
    /// Saves complete application to file system and copies images
    /// </summary>
    bool saveApplication(LoanApplication& application);

    /// <summary>
    /// Loads all applications from file
    /// </summary>
    vector<LoanApplication> loadAllApplications() const;

    /// <summary>
    /// Finds applications by CNIC
    /// </summary>
    vector<LoanApplication> findApplicationsByCNIC(const string& cnic) const;

    /// <summary>
    /// Gets application statistics by CNIC
    /// </summary>
    void getApplicationStatsByCNIC(const string& cnic,
        int& submitted,
        int& approved,
        int& rejected) const;
};

#endif