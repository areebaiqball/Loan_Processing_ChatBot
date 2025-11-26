#ifndef MULTI_SESSION_COLLECTOR_H
#define MULTI_SESSION_COLLECTOR_H

#include "application.h"
#include "application_collector.h"
#include "file_manager.h"
#include "utilities.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// SE PRINCIPLES 
// 1. Composition - uses ApplicationCollector and FileManager
// 2. Separation of Concerns - handles multi-session logic only


/// <summary>
/// Manages multi-session loan application collection
///</summary>
class MultiSessionCollector {
private:
  
    ApplicationCollector baseCollector;  
    FileManager& fileManager;           
    LoanApplication currentApplication;  

    /// <summary>
    /// Saves section progress and asks user to continue or exit
    /// </summary>
    /// <param name="application">Application to save</param>
    /// <param name="section">Section name that was completed</param>
    /// <returns>True if user wants to continue, false if saving and exiting</returns>
    bool saveSectionAndContinue(LoanApplication& application, const string& section);

    /// <summary>
    /// Displays loan type selection for personal loans
    /// </summary>
    /// <param name="application">Application to update with loan type</param>
    void selectPersonalLoanType(LoanApplication& application);

    /// <summary>
    /// Gets user-friendly status description
    /// </summary>
    /// <param name="status">Internal status code</param>
    /// <returns>Human-readable status description</returns>
    string getStatusDescription(const string& status) const;

public:
    /// <summary>
    /// Gets display name for a section
    /// </summary>
    /// <param name="section">Internal section name</param>
    /// <returns>User-friendly section name</returns>
    string getSectionDisplayName(const string& section) const;

    /// <summary>
   /// Collects personal information section
   /// </summary>
   /// <param name="application">Application object to populate</param>
   /// <returns>True if section completed successfully</returns>
    bool collectPersonalInfo(LoanApplication& application);

    /// <summary>
    /// Collects financial information section
    /// </summary>
    /// <param name="application">Application object to populate</param>
    /// <returns>True if section completed successfully</returns>
    bool collectFinancialInfo(LoanApplication& application);

    /// <summary>
    /// Collects references information section
    /// </summary>
    /// <param name="application">Application object to populate</param>
    /// <returns>True if section completed successfully</returns>
    bool collectReferencesInfo(LoanApplication& application);

    /// <summary>
    /// Collects documents information section
    /// </summary>
    /// <param name="application">Application object to populate</param>
    /// <returns>True if section completed and application submitted</returns>
    bool collectDocumentsInfo(LoanApplication& application);


    /// <summary>
    /// Constructs MultiSessionCollector with required dependencies
    /// SE Principle: Dependency Injection - FileManager passed in
    /// </summary>
    /// <param name="fm">FileManager reference for persistence</param>
    MultiSessionCollector(FileManager& fm) : fileManager(fm) {}



    /// <summary>
    /// Displays main application menu
    /// </summary>
    void showApplicationMenu();

    /// <summary>
    /// Displays current application progress
    /// </summary>
    /// <param name="application">Application to display progress for</param>
    void displayApplicationProgress(const LoanApplication& application);

    /// <summary>
    /// Starts a brand new loan application
    /// SE Principle: Workflow Initiation
    /// 
    /// Process:
    /// 1. User selects loan type (Home, Car, Scooter, Personal)
    /// 2. System generates unique application ID
    /// 3. User begins with personal information section
    /// 4. Progress saved at each checkpoint
    /// 5. User can exit and resume later
    /// </summary>
    void startNewApplication();

    /// <summary>
    /// Resumes an incomplete application
    /// SE Principle: Session Continuity
    /// 
    /// Process:
    /// 1. User provides Application ID + CNIC for verification
    /// 2. System loads incomplete application
    /// 3. Displays current progress
    /// 4. Continues from last completed section
    /// 5. Prevents resuming completed/submitted applications
    /// 
    /// Security: CNIC verification prevents unauthorized access
    /// </summary>
    void resumeExistingApplication();
};

#endif