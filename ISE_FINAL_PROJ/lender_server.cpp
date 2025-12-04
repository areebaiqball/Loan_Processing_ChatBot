#include <iostream>
#include <string>
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

    // Remove duplicate application IDs - keep only the latest one
    vector<LoanApplication> uniqueApps;
    vector<string> seenIds;

    // Go through in reverse to keep the latest entry for each ID
    for (int i = allApplications.size() - 1; i >= 0; i--) {
        string appId = allApplications[i].getApplicationId();
        bool alreadySeen = false;

        for (const auto& id : seenIds) {
            if (id == appId) {
                alreadySeen = true;
                break;
            }
        }

        if (!alreadySeen) {
            uniqueApps.insert(uniqueApps.begin(), allApplications[i]);
            seenIds.push_back(appId);
        }
    }

    cout << endl << "========== ALL APPLICATIONS ==========" << endl;
    cout << "Total: " << uniqueApps.size() << endl << endl;

    for (size_t i = 0; i < uniqueApps.size(); i++) {
        const auto& app = uniqueApps[i];

        // Show user-friendly status
        string displayStatus = app.getStatus();
        if (displayStatus == "C1") displayStatus = "Incomplete (Personal Info)";
        else if (displayStatus == "C2") displayStatus = "Incomplete (Financial Info)";
        else if (displayStatus == "C3") displayStatus = "Incomplete (References)";
        else if (displayStatus == "incomplete_documents") displayStatus = "Incomplete (Documents)";

        cout << (i + 1) << ". ID: " << app.getApplicationId()
            << " | " << app.getFullName()
            << " | Status: " << displayStatus
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

    bool reviewing = true;

    while (reviewing) {
        cout << endl << "========== PENDING APPLICATIONS ==========" << endl;
        for (size_t i = 0; i < pendingApps.size(); i++) {
            cout << (i + 1) << ". ID: " << pendingApps[i].getApplicationId()
                << " | " << pendingApps[i].getFullName() << endl;
        }

        cout << endl << "Enter application number to review (or '0' to go back): ";
        string input;
        getline(cin, input);

        // Check if user wants to go back
        if (input == "0") {
            reviewing = false;
            continue;
        }

        try {
            int appNum = stoi(input);
            if (appNum < 1 || appNum > static_cast<int>(pendingApps.size())) {
                cout << "Invalid number. Please enter a valid application number from the list." << endl;
                continue; // Continue the loop to ask again
            }

            LoanApplication selectedApp = pendingApps[appNum - 1];
            LoanApplication fullApp = fileManager.findApplicationById(selectedApp.getApplicationId());

            displayApplicationDetails(fullApp);

            // Handle approval/rejection
            bool actionCompleted = false;
            while (!actionCompleted) {
                cout << endl << "Action: (A)pprove, (R)eject, or (B)ack to list? ";
                getline(cin, input);
                input = toLower(trim(input));

                string newStatus;
                string rejectionReason = "";

                if (input == "a" || input == "approve") {
                    newStatus = "approved";
                    cout << " Application APPROVED." << endl;

                    cout << "Confirm? (Y/N): ";
                    getline(cin, input);

                    if (toLower(trim(input)) == "y") {
                        if (fileManager.updateApplicationStatus(fullApp.getApplicationId(), newStatus, rejectionReason)) {
                            cout << " Status updated!" << endl;
                            // Remove from pending apps since it's processed
                            pendingApps.erase(pendingApps.begin() + (appNum - 1));
                        }
                        actionCompleted = true;
                    }
                    else {
                        cout << "Approval cancelled." << endl;
                        actionCompleted = true;
                    }
                }
                else if (input == "r" || input == "reject") {
                    newStatus = "rejected";
                    cout << "Enter rejection reason: ";
                    getline(cin, rejectionReason);

                    cout << "Confirm rejection? (Y/N): ";
                    getline(cin, input);

                    if (toLower(trim(input)) == "y") {
                        if (fileManager.updateApplicationStatus(fullApp.getApplicationId(), newStatus, rejectionReason)) {
                            cout << " Application REJECTED." << endl;
                           
                            pendingApps.erase(pendingApps.begin() + (appNum - 1));
                        }
                        actionCompleted = true;
                    }
                    else {
                        cout << "Rejection cancelled." << endl;
                        actionCompleted = true;
                    }
                }
                else if (input == "b" || input == "back") {
                    actionCompleted = true; 
                }
                else {
                    cout << "Invalid action. Please enter A, R, or B." << endl;
                }
            }

            if (pendingApps.empty()) {
                cout << endl << "No more pending applications." << endl;
                reviewing = false;
            }

        }
        catch (const exception& e) {
            cout << "Invalid input. Please enter a valid number." << endl;
        }
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

    cout << endl << "" ;
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
