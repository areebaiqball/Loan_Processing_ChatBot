#pragma once
#include <iostream>
#include <string>
#include <thread>
#include "network_utils.h"
#include "file_manager.h"
#include "application.h"
#include "utilities.h"
#include "ui_handler.h"

using namespace std;


void handleLenderCommands(FileManager& fileManager);
void displayLenderMenu();
void viewAllApplications(FileManager& fileManager);
void viewApplicationsByStatus(FileManager& fileManager, const string& status);
void reviewAndProcessApplications(FileManager& fileManager);
void displayApplicationStatistics(FileManager& fileManager);
void searchApplicationById(FileManager& fileManager);
void displayApplicationDetails(const LoanApplication& app);

void displayLenderMenu() {
    cout << endl << "========== LENDER MENU ==========" << endl;
    cout << "  1. View ALL Applications" << endl;
    cout << "  2. View Pending Applications" << endl;
    cout << "  3. View Approved Applications" << endl;
    cout << "  4. View Rejected Applications" << endl;
    cout << "  5. Review and Process Applications" << endl;
    cout << "  6. Application Statistics" << endl;
    cout << "  7. Search Application by ID" << endl;
    cout << "  X. Exit" << endl;
    cout << "=================================" << endl;
}



void viewAllApplications(FileManager& fileManager) {
    auto allApplications = fileManager.loadAllApplications();

    if (allApplications.empty()) {
        cout << endl << "No applications found." << endl;
        return;
    }

    cout << endl << "========== ALL APPLICATIONS ==========" << endl;
    cout << "Total: " << allApplications.size() << endl << endl;

    for (size_t i = 0; i < allApplications.size(); i++) {
        const auto& app = allApplications[i];
        cout << (i + 1) << ". ID: " << app.getApplicationId()
            << " | " << app.getFullName()
            << " | Status: " << app.getStatus()
            << " | Income: PKR " << app.getAnnualIncome() << endl;
    }
}

void viewApplicationsByStatus(FileManager& fileManager, const string& status) {
    auto allApplications = fileManager.loadAllApplications();
    vector<LoanApplication> filteredApps;

    for (const auto& app : allApplications) {
        if (app.getStatus() == status) {
            filteredApps.push_back(app);
        }
    }

    if (filteredApps.empty()) {
        cout << endl << "No " << status << " applications found." << endl;
        return;
    }

    cout << endl << "========== " << status << " APPLICATIONS ==========" << endl;
    for (size_t i = 0; i < filteredApps.size(); i++) {
        const auto& app = filteredApps[i];
        cout << (i + 1) << ". ID: " << app.getApplicationId()
            << " | " << app.getFullName()
            << " | Income: PKR " << app.getAnnualIncome() << endl;
    }
}

void reviewAndProcessApplications(FileManager& fileManager) {
    auto allApplications = fileManager.loadAllApplications();
    vector<LoanApplication> pendingApps;

    for (const auto& app : allApplications) {
        if (app.getStatus() == "submitted") {
            pendingApps.push_back(app);
        }
    }

    if (pendingApps.empty()) {
        cout << endl << "No pending applications." << endl;
        return;
    }

    cout << endl << "========== PENDING APPLICATIONS ==========" << endl;
    for (size_t i = 0; i < pendingApps.size(); i++) {
        cout << (i + 1) << ". ID: " << pendingApps[i].getApplicationId()
            << " | " << pendingApps[i].getFullName() << endl;
    }

    cout << endl << "Enter application number to review: ";
    string input;
    getline(cin, input);

    try {
        int appNum = stoi(input);
        if (appNum < 1 || appNum > static_cast<int>(pendingApps.size())) {
            cout << "Invalid number." << endl;
            return;
        }

        LoanApplication selectedApp = pendingApps[appNum - 1];
        LoanApplication fullApp = fileManager.findApplicationById(selectedApp.getApplicationId());

        displayApplicationDetails(fullApp);

        cout << endl << "Action: (A)pprove or (R)eject? ";
        getline(cin, input);
        input = toLower(trim(input));

        string newStatus;
        string rejectionReason = "";

        if (input == "a" || input == "approve") {
            newStatus = "approved";
            cout << " Application APPROVED." << endl;
        }
        else if (input == "r" || input == "reject") {
            newStatus = "rejected";
            cout << "Enter rejection reason: ";
            getline(cin, rejectionReason);
        }
        else {
            cout << "Invalid action." << endl;
            return;
        }

        cout << "Confirm? (Y/N): ";
        getline(cin, input);

        if (toLower(trim(input)) == "y") {
            if (fileManager.updateApplicationStatus(fullApp.getApplicationId(), newStatus, rejectionReason)) {
                cout << " Status updated!" << endl;
            }
        }
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void displayApplicationStatistics(FileManager& fileManager) {
    auto allApplications = fileManager.loadAllApplications();
    int submitted = 0, approved = 0, rejected = 0;

    for (const auto& app : allApplications) {
        string status = app.getStatus();
        if (status == "submitted") submitted++;
        else if (status == "approved") approved++;
        else if (status == "rejected") rejected++;
    }

    int total = allApplications.size();
    cout << endl << "========== STATISTICS ==========" << endl;
    cout << "Total: " << total << endl;
    cout << "Pending: " << submitted << endl;
    cout << "Approved: " << approved << endl;
    cout << "Rejected: " << rejected << endl;
    if (total > 0) {
        cout << "Approval Rate: " << (approved * 100.0 / total) << "%" << endl;
    }
    cout << "================================" << endl;
}

void searchApplicationById(FileManager& fileManager) {
    cout << endl << "Enter Application ID: ";
    string appId;
    getline(cin, appId);

    LoanApplication app = fileManager.findApplicationById(trim(appId));
    if (app.getApplicationId().empty()) {
        cout << "Application not found." << endl;
        return;
    }

    displayApplicationDetails(app);
}

void handleLenderCommands(FileManager& fileManager) {
    bool running = true;
    string input;

    cout << endl << "" << endl;
    cout << "  LENDER INTERFACE STARTED      " << endl;
    cout << "" << endl;

    while (running) {
        displayLenderMenu();
        cout << "Lender> ";
        getline(cin, input);
        input = toLower(trim(input));

        if (input == "x" || input == "exit") {
            running = false;
        }
        else if (input == "1") {
            viewAllApplications(fileManager);
        }
        else if (input == "2") {
            viewApplicationsByStatus(fileManager, "submitted");
        }
        else if (input == "3") {
            viewApplicationsByStatus(fileManager, "approved");
        }
        else if (input == "4") {
            viewApplicationsByStatus(fileManager, "rejected");
        }
        else if (input == "5") {
            reviewAndProcessApplications(fileManager);
        }
        else if (input == "6") {
            displayApplicationStatistics(fileManager);
        }
        else if (input == "7") {
            searchApplicationById(fileManager);
        }
        else {
            cout << "Invalid option." << endl;
        }

        if (running) {
            cout << endl << "Press Enter to continue...";
            getline(cin, input);
        }
    }

    cout << "Lender interface closed." << endl;
}

int main() {
    FileManager fileManager;

    cout << "╔════════════════════════════════════╗" << endl;
    cout << "║   LOAN PROCESSING SYSTEM           ║" << endl;
    cout << "║   LENDER SERVER                    ║" << endl;
    cout << "╚════════════════════════════════════╝" << endl;

    handleLenderCommands(fileManager);

    return 0;
}