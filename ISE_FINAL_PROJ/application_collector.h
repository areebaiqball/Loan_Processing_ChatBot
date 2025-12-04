#ifndef APPLICATION_COLLECTOR_H
#define APPLICATION_COLLECTOR_H

#include "application.h"
#include "utilities.h"
#include "file_manager.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
class ApplicationCollector {
private:
    bool collectPersonalInfo(LoanApplication& application);
    bool collectEmploymentAndFinancialInfo(LoanApplication& application);
    bool collectExistingLoansInfo(LoanApplication& application);
    bool collectReferencesInfo(LoanApplication& application);
    bool confirmApplication(const LoanApplication& application);
    bool collectImagePaths(LoanApplication& application);
    bool collectFinancialDocuments(LoanApplication& application);
    void collectInstallmentStartDate(LoanApplication& application);
    bool getValidatedYesNo(const string& prompt);

public:
    string getValidatedString(const string& prompt, const string& fieldName, size_t minLength = 1, size_t maxLength = 255);
    long long getValidatedNumeric(const string& prompt, const string& fieldName, long long minValue = 0, long long maxValue = 999999999999);
    string getValidatedCNIC(const string& prompt);
    string getValidatedEmail(const string& prompt);
    string getValidatedPhone(const string& prompt);
    string getValidatedDate(const string& prompt);
    string getSelectionFromOptions(const string& prompt, const vector<string>& options);
    string getImagePath(const string& imageType);
    string getValidatedCNICIssueDate(const string& prompt);
    string getValidatedCNICExpiryDate(const string& prompt);

    LoanApplication collectApplicationForLoan(const string& loanType, const string& loanDetails);
    void collectLoanSpecificInfo(LoanApplication& application, const string& loanType, const string& loanDetails);
};
#endif