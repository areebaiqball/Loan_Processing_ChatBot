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

class MultiSessionCollector {
private:
    ApplicationCollector baseCollector;
    FileManager& fileManager;
    LoanApplication currentApplication;

    bool collectPersonalInfo(LoanApplication& application);
    bool collectFinancialInfo(LoanApplication& application);
    bool collectReferencesInfo(LoanApplication& application);
    bool collectDocumentsInfo(LoanApplication& application);
    bool saveSectionAndContinue(LoanApplication& application, const string& section);
    void selectPersonalLoanType(LoanApplication& application);
    // Helper methods for better UX
    string getStatusDescription(const string& status) const;
    string getSectionDisplayName(const string& section) const;

public:
    MultiSessionCollector(FileManager& fm) : fileManager(fm) {}

    void showApplicationMenu();
    void displayApplicationProgress(const LoanApplication& application);
    void startNewApplication();
    void resumeExistingApplication();
};

#endif
