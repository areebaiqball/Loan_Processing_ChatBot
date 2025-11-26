#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include "application.h"

using namespace std;


/// <summary>
/// Manages file-based persistence for loan applications
/// SE Principles:
/// - Single Responsibility: Only handles file I/O, no business logic
/// - Data Access Layer Pattern: Abstracts storage mechanism from application
/// - Separation of Concerns: File format and storage separate from domain logic
/// - CRUD Operations: Provides Create, Read, Update, Delete for applications
/// </summary>
class FileManager {
private:

    string applicationsFile;   // Path to applications.txt
    string imagesDirectory;    // Path to images/ folder


    /// <summary>
    /// Checks if a file exists
    /// </summary>
    /// <param name="filename">Path to file</param>
    /// <returns>True if file exists and is accessible</returns>
    bool fileExists(const string& filename) const;

    /// <summary>
    /// Copies an image file from source to destination
    /// </summary>
    /// <param name="sourcePath">Original file location</param>
    /// <param name="destinationPath">Where to copy file</param>
    /// <returns>True if copy successful and verified</returns>
    bool copyImageFile(const string& sourcePath, const string& destinationPath) const;

    /// <summary>
    /// Converts application object to file format string
    /// </summary>
    /// <param name="application">Application object to serialize</param>
    /// <returns>Delimiter-separated string representation</returns>
    string applicationToFileFormat(const LoanApplication& application) const;

    /// <summary>
    /// Converts file format string to application object
    /// </summary>
    /// <param name="parts">Vector of field values from file</param>
    /// <returns>Reconstructed application object</returns>
    LoanApplication applicationFromFileFormat(const vector<string>& parts) const;

    /// <summary>
    /// Handles old format applications (backward compatibility)
    /// </summary>
    /// <param name="parts">Vector of field values in old format</param>
    /// <returns>Application object from old format data</returns>
    LoanApplication handleOldFormat(const vector<string>& parts) const;

public:

    /// <summary>
    /// Constructs FileManager with configurable paths
    /// </summary>
    /// <param name="appsFile">Path to applications file (default: applications.txt)</param>
    /// <param name="imagesDir">Path to images directory (default: images/)</param>
    FileManager(const string& appsFile = "applications.txt",
        const string& imagesDir = "images/");

    /// <summary>
    /// Generates a unique 4-digit application ID
    /// </summary>
    /// <returns>Unique application ID string</returns>
    string generateApplicationId() const;

    /// <summary>
    /// Saves a complete application to file
    /// </summary>
    /// <param name="application">Application to save (modified with ID and paths)</param>
    /// <returns>True if saved successfully</returns>
    bool saveApplication(LoanApplication& application);

    /// <summary>
    /// Updates a section of an application (multi-session support)
    /// </summary>
    /// <param name="application">Application with updated section</param>
    /// <param name="section">Section name that was completed</param>
    /// <returns>True if update successful</returns>
    bool updateApplicationSection(const LoanApplication& application, const string& section);


    /// <summary>
    /// Loads all applications from file
    /// </summary>
    /// <returns>Vector of all applications (empty if file not found)</returns>
    vector<LoanApplication> loadAllApplications() const;

    /// <summary>
    /// Loads all applications with full details
    /// </summary>
    /// <returns>Vector of applications with full details</returns>
    vector<LoanApplication> loadAllApplicationsDetailed() const;

    /// <summary>
    /// Finds a specific application by ID
    /// </summary>
    /// <param name="applicationId">Unique application ID</param>
    /// <returns>Application object (empty if not found)</returns>
    LoanApplication findApplicationById(const string& applicationId) const;

    /// <summary>
    /// Finds all applications for a specific CNIC
    /// </summary>
    /// <param name="cnic">13-digit CNIC number</param>
    /// <returns>Vector of applications by this person</returns>
    vector<LoanApplication> findApplicationsByCNIC(const string& cnic) const;

    /// <summary>
    /// Finds incomplete application for resuming
    /// </summary>
    /// <param name="applicationId">Application ID</param>
    /// <param name="cnic">User's CNIC for verification</param>
    /// <returns>Incomplete application (empty if not found/complete)</returns>
    LoanApplication findIncompleteApplication(const string& applicationId,
        const string& cnic) const;

    /// <summary>
    /// Finds all incomplete applications for a user
    /// </summary>
    /// <param name="cnic">User's CNIC</param>
    /// <returns>Vector of incomplete applications</returns>
    vector<LoanApplication> findUserIncompleteApplications(const string& cnic) const;

    /// <summary>
    /// Checks if user can update a specific application
    /// </summary>
    /// <param name="applicationId">Application ID</param>
    /// <param name="cnic">User's CNIC</param>
    /// <returns>True if user can update this application</returns>
    bool canUpdateApplication(const string& applicationId, const string& cnic) const;

    /// <summary>
    /// Gets application statistics by CNIC
    /// </summary>
    /// <param name="cnic">User's CNIC</param>
    /// <param name="submitted">Output: count of submitted applications</param>
    /// <param name="approved">Output: count of approved applications</param>
    /// <param name="rejected">Output: count of rejected applications</param>
    void getApplicationStatsByCNIC(const string& cnic,
        int& submitted, int& approved, int& rejected) const;


    /// <summary>
    /// Updates application status (approve/reject)
    /// </summary>
    /// <param name="applicationId">Application ID to update</param>
    /// <param name="newStatus">New status (approved/rejected)</param>
    /// <param name="rejectionReason">Reason if rejected (optional)</param>
    /// <returns>True if update successful</returns>
    bool updateApplicationStatus(const string& applicationId,
        const string& newStatus,
        const string& rejectionReason = "");
};

#endif