#ifndef APPLICATION_COLLECTOR_H
#define APPLICATION_COLLECTOR_H

#include "application.h"
#include "utilities.h"
#include "file_manager.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

/// <summary>
/// Handles collection of personal information for loan applications
/// </summary>
class ApplicationCollector {
private:
    /// <summary>
    /// Collects basic personal information
    /// </summary>
    bool collectPersonalInfo(LoanApplication& application);

    /// <summary>
    /// Collects employment and financial details
    /// </summary>
    bool collectEmploymentAndFinancialInfo(LoanApplication& application);

    /// <summary>
    /// Collects existing loans information
    /// </summary>
    bool collectExistingLoansInfo(LoanApplication& application);

    /// <summary>
    /// Collects references information
    /// </summary>
    bool collectReferencesInfo(LoanApplication& application);

    /// <summary>
    /// Displays summary and gets confirmation
    /// </summary>
    bool confirmApplication(const LoanApplication& application);

    /// <summary>
    /// Gets validated string input from user
    /// </summary>
    string getValidatedString(const string& prompt, const string& fieldName, size_t minLength = 1, size_t maxLength = 255);

    /// <summary>
    /// Gets validated numeric input from user
    /// </summary>
    long long getValidatedNumeric(const string& prompt, const string& fieldName, long long minValue = 0, long long maxValue = 999999999999);

    /// <summary>
    /// Gets validated CNIC from user
    /// </summary>
    string getValidatedCNIC(const string& prompt);

    /// <summary>
    /// Gets validated email from user
    /// </summary>
    string getValidatedEmail(const string& prompt);

    /// <summary>
    /// Gets validated phone number from user
    /// </summary>
    string getValidatedPhone(const string& prompt);

    /// <summary>
    /// Gets validated date from user
    /// </summary>
    string getValidatedDate(const string& prompt);

    /// <summary>
    /// Gets selection from predefined options
    /// </summary>
    string getSelectionFromOptions(const string& prompt, const vector<string>& options);

    bool collectImagePaths(LoanApplication& application);
    string getImagePath(const string& imageType);
    /// <summary>
   /// Collects reference document images
   /// </summary>
    bool collectReferenceDocuments(LoanApplication& application);

    /// <summary>
    /// Collects additional financial documents
    /// </summary>
    bool collectFinancialDocuments(LoanApplication& application);
    /// <summary>
   /// Gets validated CNIC issue date - must not be in future
   /// </summary>
    string getValidatedCNICIssueDate(const string& prompt);

    /// <summary>
    /// Gets validated CNIC expiry date - must not be expired
    /// </summary>
    string getValidatedCNICExpiryDate(const string& prompt);

public:
    /// <summary>
    /// Main method to collect complete application
    /// </summary>
    LoanApplication collectCompleteApplication();

    /// <summary>
    /// Collects application for a specific loan type
    /// </summary>
    LoanApplication collectApplicationForLoan(const string& loanType, const string& loanDetails, FileManager& fileManager);
    /// <summary>
/// Collects loan-specific information based on loan type
/// </summary>
    void collectLoanSpecificInfo(LoanApplication& application, const string& loanType, const string& loanDetails);
};

#endif