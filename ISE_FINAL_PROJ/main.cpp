//#include <iostream>
//#include <string>
//#include <vector>
//#include <iomanip>
//#include "utterances.h"
//#include "home.h"
//#include "car.h"
//#include "scooter.h"
//#include "utilities.h"
//#include "application.h"
//#include "application_collector.h"
//#include "file_manager.h"
//
//using namespace std;
//
//
//
//// Forward declarations
//void handleLenderMode(FileManager& fileManager);
//void handleUserMode(const HomeLoan[], int, const CarLoan[], int,
//    const ScooterLoan[], int, const Utterance[], int,
//    ApplicationCollector&, FileManager&);
//
//void checkApplicationStatusByCNIC(FileManager& fileManager);
//void displayApplicationStatistics(FileManager& fileManager);
//void searchApplicationById(FileManager& fileManager);
//void viewAllApplications(FileManager& fileManager);
//
//// Input Validation Helpers
//bool isValidAreaInput(const string& input) {
//    return (input == "1" || input == "2" || input == "3" || input == "4");
//}
//
//bool isValidLoanType(const string& input) {
//    return (input == "h" || input == "c" || input == "s" || input == "p");
//}
//
//bool isValidMakeInput(const string& input, const string& loanType) {
//    if (loanType == "car") return (input == "1" || input == "2");
//    if (loanType == "scooter") return (input == "1");
//    return false;
//}
//
//// UI Display Functions
//void displayWelcomeMessage() {
//    cout << "==========================================" << endl;
//    cout << "   Loan Processing System - " << Config::CHATBOT_NAME << endl;
//    cout << "==========================================" << endl << endl;
//}
//
//void displayRoleSelection() {
//    cout << "Please select your role:" << endl;
//    cout << "  Press L for Lender" << endl;
//    cout << "  Press U for User/Applicant" << endl;
//    cout << "  Press X to exit" << endl;
//}
//
//void displayLoanCategories() {
//    cout << Config::CHATBOT_NAME << ": Please select the category you want to apply for." << endl;
//    cout << "  Press H for a home loan" << endl;
//    cout << "  Press C for a car loan" << endl;
//    cout << "  Press S for a scooter loan" << endl;
//    cout << "  Press P for a personal loan" << endl;
//    cout << "  Press X to exit" << endl;
//}
//
//void displayGoodbyeMessage() {
//    cout << Config::CHATBOT_NAME << ": Thank you for using our service. Goodbye!" << endl;
//}
//
//void displayCarMakes() {
//    cout << Config::CHATBOT_NAME << ": Please select the car make:" << endl;
//    cout << "  Press 1 for Make 1" << endl;
//    cout << "  Press 2 for Make 2" << endl;
//    cout << "  Press X to exit" << endl;
//}
//
//void displayScooterMakes() {
//    cout << Config::CHATBOT_NAME << ": Please select the scooter make:" << endl;
//    cout << "  Press 1 for Make 1" << endl;
//    cout << "  Press X to exit" << endl;
//}
//
//void displayLenderMenu() {
//    cout << endl << "========== LENDER MENU ==========" << endl;
//    cout << "  1. View ALL Applications" << endl;
//    cout << "  2. View Pending Applications" << endl;
//    cout << "  3. View Approved Applications" << endl;
//    cout << "  4. View Rejected Applications" << endl;
//    cout << "  5. Review and Process Applications" << endl;
//    cout << "  6. Application Statistics" << endl;
//    cout << "  7. Search Application by ID" << endl;
//    cout << "  X. Exit to Main Menu" << endl;
//    cout << "=================================" << endl;
//}
//
//
//
//// SE Principles: Separation of Concerns, Single Responsibility
//void viewApplicationsByStatus(FileManager& fileManager, const string& status) {
//    auto allApplications = fileManager.loadAllApplications();
//    vector<LoanApplication> filteredApps;
//
//    for (size_t i = 0; i < allApplications.size(); i++) {
//        if (allApplications[i].getStatus() == status) {
//            filteredApps.push_back(allApplications[i]);
//        }
//    }
//
//    if (filteredApps.empty()) {
//        cout << endl << "No " << status << " applications found." << endl;
//        return;
//    }
//
//    cout << endl << "========== " << status << " APPLICATIONS ==========" << endl;
//    for (size_t i = 0; i < filteredApps.size(); i++) {
//        const auto& app = filteredApps[i];
//        cout << (i + 1) << ". ID: " << app.getApplicationId()
//            << " | Name: " << app.getFullName()
//            << " | Date: " << app.getSubmissionDate()
//            << " | Income: PKR " << app.getAnnualIncome()
//            << " | Employment: " << app.getEmploymentStatus() << endl;
//    }
//    cout << "========================================" << endl;
//}
//void reviewAndProcessApplications(FileManager& fileManager) {
//    auto allApplications = fileManager.loadAllApplications();
//    vector<LoanApplication> pendingApps;
//
//    for (size_t i = 0; i < allApplications.size(); i++) {
//        if (allApplications[i].getStatus() == "submitted") {
//            pendingApps.push_back(allApplications[i]);
//        }
//    }
//
//    if (pendingApps.empty()) {
//        cout << endl << "No pending applications to review." << endl;
//        return;
//    }
//
//    cout << endl << "========== PENDING APPLICATIONS ==========" << endl;
//    for (size_t i = 0; i < pendingApps.size(); i++) {
//        cout << (i + 1) << ". ID: " << pendingApps[i].getApplicationId()
//            << " | Name: " << pendingApps[i].getFullName()
//            << " | Date: " << pendingApps[i].getSubmissionDate()
//            << " | Income: PKR " << pendingApps[i].getAnnualIncome() << endl;
//    }
//
//    cout << endl << Config::CHATBOT_NAME << ": Enter application number to review (or X to cancel): ";
//    string input;
//    getline(cin, input);
//    input = trim(input);
//
//    if (toLower(input) == "x") {
//        return;
//    }
//
//    try {
//        int appNum = stoi(input);
//        if (appNum < 1 || appNum > static_cast<int>(pendingApps.size())) {
//            cout << Config::CHATBOT_NAME << ": Invalid application number." << endl;
//            return;
//        }
//
//        LoanApplication selectedApp = pendingApps[appNum - 1];
//
//        // Load full application details
//        LoanApplication fullApp = fileManager.findApplicationById(selectedApp.getApplicationId());
//        if (fullApp.getApplicationId().empty()) {
//            cout << Config::CHATBOT_NAME << ": Error: Could not load application details." << endl;
//            return;
//        }
//
//        displayApplicationDetails(fullApp);
//
//        cout << endl << Config::CHATBOT_NAME << ": Action: (A)pprove or (R)eject this application? ";
//        getline(cin, input);
//        input = toLower(trim(input));
//
//        string newStatus;
//        string rejectionReason = "";
//
//        if (input == "a" || input == "approve") {
//            newStatus = "approved";
//            cout << " Application " << fullApp.getApplicationId() << " has been APPROVED." << endl;
//            cout << Config::CHATBOT_NAME << ": The applicant can now generate their installment plan when checking their application status." << endl;
//        }
//        else if (input == "r" || input == "reject") {
//            newStatus = "rejected";
//            cout << " Application " << fullApp.getApplicationId() << " will be REJECTED." << endl;
//
//            cout << Config::CHATBOT_NAME << ": Enter reason for rejection: ";
//            getline(cin, rejectionReason);
//            rejectionReason = trim(rejectionReason);
//
//            if (rejectionReason.empty()) {
//                cout << Config::CHATBOT_NAME << ": No rejection reason provided. Application will be rejected without specific reason." << endl;
//            }
//        }
//        else {
//            cout << Config::CHATBOT_NAME << ": Invalid action. No changes made." << endl;
//            return;
//        }
//
//        // Ask for confirmation
//        cout << Config::CHATBOT_NAME << ": Confirm this action? (Y/N): ";
//        getline(cin, input);
//        input = toLower(trim(input));
//
//        if (input == "y" || input == "yes") {
//            if (fileManager.updateApplicationStatus(fullApp.getApplicationId(), newStatus, rejectionReason)) {
//                cout << " Application status updated successfully!" << endl;
//            }
//            else {
//                cout << " Failed to update application status." << endl;
//            }
//        }
//        else {
//            cout << Config::CHATBOT_NAME << ": Action cancelled." << endl;
//        }
//
//    }
//    catch (const exception& e) {
//        cout << Config::CHATBOT_NAME << ": Error: " << e.what() << endl;
//    }
//}
//
//void handleLenderMode(FileManager& fileManager) {
//    bool inLenderMode = true;
//    string input;
//
//    while (inLenderMode) {
//        displayLenderMenu();
//        cout << "Lender: ";
//        getline(cin, input);
//        input = toLower(trim(input));
//
//        if (input == "x") {
//            inLenderMode = false;
//        }
//        else if (input == "1") {
//            viewAllApplications(fileManager);
//        }
//        else if (input == "2") {
//            viewApplicationsByStatus(fileManager, "submitted");
//        }
//        else if (input == "3") {
//            viewApplicationsByStatus(fileManager, "approved");
//        }
//        else if (input == "4") {
//            viewApplicationsByStatus(fileManager, "rejected");
//        }
//        else if (input == "5") {
//            reviewAndProcessApplications(fileManager);
//        }
//        else if (input == "6") {
//            displayApplicationStatistics(fileManager);
//        }
//        else if (input == "7") {
//            searchApplicationById(fileManager);
//        }
//        else {
//            cout << "Invalid option. Please try again." << endl;
//        }
//
//        if (inLenderMode) {
//            cout << endl << "Press any key to continue...";
//            getline(cin, input);
//        }
//    }
//}
//
//
//
//
//
//void handleUserMode(const HomeLoan homeLoans[], int homeLoanCount,
//    const CarLoan carLoans[], int carLoanCount,
//    const ScooterLoan scooterLoans[], int scooterLoanCount,
//    const Utterance utterances[], int utteranceCount,
//    ApplicationCollector& collector, FileManager& fileManager) {
//
//    bool running = true;
//    string userInput;
//
//    cout << endl << Config::CHATBOT_NAME << ": Hello! I'm " << Config::CHATBOT_NAME
//        << ", your loan assistant." << endl;
//    cout << Config::CHATBOT_NAME << ": I'm here to help you find the perfect loan." << endl;
//    cout << endl << "Options:" << endl;
//    cout << "  - Type 'A' to apply for a new loan" << endl;
//    cout << "  - Type 'check my applications' to view your status" << endl;
//    cout << "  - Type 'X' to exit" << endl;
//
//    while (running) {
//        cout << endl << "You: ";
//        getline(cin, userInput);
//        string lowerInput = toLower(trim(userInput));
//
//        if (lowerInput == Config::EXIT_COMMAND) {
//            displayGoodbyeMessage();
//            break;
//        }
//
//        if (lowerInput == "check my applications" ||
//            lowerInput == "application status" ||
//            lowerInput == "check status" ||
//            lowerInput == "my application count" ||
//            (lowerInput.find("application") != string::npos && lowerInput.find("status") != string::npos) ||
//            (lowerInput.find("check") != string::npos && lowerInput.find("application") != string::npos)) {
//            checkApplicationStatusByCNIC(fileManager);
//            continue;
//        }
//
//        if (lowerInput == "a") {
//            displayLoanCategories();
//            cout << "You: ";
//            getline(cin, userInput);
//            lowerInput = toLower(trim(userInput));
//
//            if (lowerInput == Config::EXIT_COMMAND) {
//                displayGoodbyeMessage();
//                break;
//            }
//
//            if (lowerInput == "h") {
//                handleHomeLoanSelection(homeLoans, homeLoanCount, running, collector, fileManager);
//            }
//            else if (lowerInput == "c") {
//                handleCarLoanSelection(carLoans, carLoanCount, running, collector, fileManager);
//            }
//            else if (lowerInput == "s") {
//                handleScooterLoanSelection(scooterLoans, scooterLoanCount, running, collector, fileManager);
//            }
//            else if (lowerInput == "p") {
//                handlePersonalLoanSelection(running, collector, fileManager);
//            }
//            else {
//                cout << Config::CHATBOT_NAME << ": Invalid selection." << endl;
//                displayLoanCategories();
//                continue;
//            }
//        }
//        else {
//            cout << Config::CHATBOT_NAME << ": " << getResponse(utterances, utteranceCount, userInput) << endl;
//        }
//    }
//}
//
//// SE Principles: Information Expert Pattern, Data Display
//void displayApplicationStatistics(FileManager& fileManager) {
//    auto allApplications = fileManager.loadAllApplications();
//
//    int submitted = 0, approved = 0, rejected = 0, total = 0;
//
//    for (const auto& app : allApplications) {
//        string status = app.getStatus();
//        if (status == "submitted") submitted++;
//        else if (status == "approved") approved++;
//        else if (status == "rejected") rejected++;
//        total++;
//    }
//
//    cout << endl << "========== APPLICATION STATISTICS ==========" << endl;
//    cout << "Total Applications: " << total << endl;
//    cout << "Pending Review: " << submitted << endl;
//    cout << "Approved: " << approved << endl;
//    cout << "Rejected: " << rejected << endl;
//
//    if (total > 0) {
//        cout << fixed << setprecision(1);
//        cout << "Approval Rate: " << (approved * 100.0 / total) << "%" << endl;
//        cout << "Rejection Rate: " << (rejected * 100.0 / total) << "%" << endl;
//    }
//    cout << "============================================" << endl;
//}
//
//void searchApplicationById(FileManager& fileManager) {
//    cout << endl << "Enter Application ID to search: ";
//    string appId;
//    getline(cin, appId);
//    appId = trim(appId);
//
//    if (appId.empty()) {
//        cout << "Invalid Application ID." << endl;
//        return;
//    }
//
//    LoanApplication app = fileManager.findApplicationById(appId);
//    if (app.getApplicationId().empty()) {
//        cout << "Application with ID " << appId << " not found." << endl;
//        return;
//    }
//
//    displayApplicationDetails(app);
//
//    cout << endl << "Options:" << endl;
//    cout << "1. Change Status" << endl;
//    cout << "2. Return to Menu" << endl;
//    cout << "Choice: ";
//
//    string choice;
//    getline(cin, choice);
//    choice = trim(choice);
//
//    if (choice == "1") {
//        cout << "New Status (approved/rejected/submitted): ";
//        string newStatus;
//        getline(cin, newStatus);
//        newStatus = toLower(trim(newStatus));
//
//        if (newStatus == "approved" || newStatus == "rejected" || newStatus == "submitted") {
//            if (fileManager.updateApplicationStatus(appId, newStatus)) {
//                cout << "Status updated successfully!" << endl;
//            }
//            else {
//                cout << "Failed to update status." << endl;
//            }
//        }
//        else {
//            cout << "Invalid status." << endl;
//        }
//    }
//}
//
//void viewAllApplications(FileManager& fileManager) {
//    auto allApplications = fileManager.loadAllApplications();
//
//    if (allApplications.empty()) {
//        cout << endl << "No applications found in the system." << endl;
//        return;
//    }
//
//    cout << endl << "========== ALL APPLICATIONS ==========" << endl;
//    cout << "Total: " << allApplications.size() << " application(s)" << endl;
//    cout << "======================================" << endl << endl;
//
//    cout << "+-----+--------+-----------+------------------------+------------+--------------+" << endl;
//    cout << "| No  | App ID | Status    | Applicant Name         | Date       | Income (PKR) |" << endl;
//    cout << "+-----+--------+-----------+------------------------+------------+--------------+" << endl;
//
//    for (size_t i = 0; i < allApplications.size(); i++) {
//        const auto& app = allApplications[i];
//        cout << "| " << setw(3) << (i + 1) << " | "
//            << setw(6) << app.getApplicationId() << " | "
//            << setw(9) << left << app.getStatus() << " | "
//            << setw(22) << app.getFullName() << " | "
//            << setw(10) << app.getSubmissionDate() << " | "
//            << setw(12) << right << app.getAnnualIncome() << " |" << endl;
//    }
//
//    cout << "+-----+--------+-----------+------------------------+------------+--------------+" << endl;
//
//    cout << endl << "Enter application number to view details (or press Enter to return): ";
//    string input;
//    getline(cin, input);
//    input = trim(input);
//
//    if (!input.empty()) {
//        try {
//            int appNum = stoi(input);
//            if (appNum >= 1 && appNum <= static_cast<int>(allApplications.size())) {
//                displayApplicationDetails(allApplications[appNum - 1]);
//            }
//            else {
//                cout << "Invalid application number." << endl;
//            }
//        }
//        catch (...) {
//            cout << "Invalid input." << endl;
//        }
//    }
//}
//
//
//
//
//// SE Principles: Dependency Injection, Initialization, Error Handling
//int main() {
//    // Initialize data structures
//    const int MAX_UTTERANCES_LOCAL = 100;
//    const int MAX_LOANS_LOCAL = 100;
//
//    Utterance utterances[MAX_UTTERANCES_LOCAL];
//    HomeLoan homeLoans[MAX_LOANS_LOCAL];
//    CarLoan carLoans[MAX_LOANS_LOCAL];
//    ScooterLoan scooterLoans[MAX_LOANS_LOCAL];
//    FileManager fileManager;
//    ApplicationCollector collector;
//
//    int utteranceCount = loadUtterances(utterances, MAX_UTTERANCES_LOCAL, Config::UTTERANCES_FILE);
//    int homeLoanCount = loadHomeLoans(homeLoans, MAX_LOANS_LOCAL, Config::HOME_LOANS_FILE);
//    int carLoanCount = loadCarLoans(carLoans, MAX_LOANS_LOCAL, Config::CAR_LOANS_FILE);
//    int scooterLoanCount = loadScooterLoans(scooterLoans, MAX_LOANS_LOCAL, Config::SCOOTER_LOANS_FILE);
//    // Validate critical data
//    if (utteranceCount == 0) {
//        cerr << "Failed to load utterances. Exiting." << endl;
//        return 1;
//    }
//    void displayApplicationDetails(const LoanApplication & app) {
//        cout << endl << "========================================" << endl;
//        cout << "APPLICATION DETAILS" << endl;
//        cout << "========================================" << endl;
//        cout << "Application ID: " << app.getApplicationId() << endl;
//        cout << "Status: " << app.getStatus() << endl;
//
//        if (app.getStatus() == "rejected" && !app.getRejectionReason().empty()) {
//            cout << "Rejection Reason: " << app.getRejectionReason() << endl;
//        }
//
//        cout << "Submission Date: " << app.getSubmissionDate() << endl;
//        cout << endl << "PERSONAL INFORMATION:" << endl;
//        cout << "  Full Name: " << app.getFullName() << endl;
//        cout << "  Father's Name: " << app.getFathersName() << endl;
//        cout << "  CNIC: " << app.getCnicNumber() << endl;
//        cout << "  Contact: " << app.getContactNumber() << endl;
//        cout << "  Email: " << app.getEmailAddress() << endl;
//        cout << "  Address: " << app.getPostalAddress() << endl;
//        cout << endl << "EMPLOYMENT & FINANCIAL:" << endl;
//        cout << "  Employment: " << app.getEmploymentStatus() << endl;
//        cout << "  Marital Status: " << app.getMaritalStatus() << endl;
//        cout << "  Gender: " << app.getGender() << endl;
//        cout << "  Dependents: " << app.getNumberOfDependents() << endl;
//        cout << "  Annual Income: PKR " << app.getAnnualIncome() << endl;
//        cout << "  Avg Electricity Bill: PKR " << app.getAvgElectricityBill() << endl;
//        cout << "  Current Electricity Bill: PKR " << app.getCurrentElectricityBill() << endl;
//
//        cout << endl << "LOAN DETAILS:" << endl;
//
//        string loanType = app.getLoanType();
//        if (loanType.empty() || loanType == "0" || loanType == "Unknown") {
//            loanType = "Personal Loan";
//        }
//        cout << "  Loan Type: " << loanType << endl;
//
//        cout << "  Category: " << app.getLoanCategory() << endl;
//        cout << "  Total Amount: PKR " << app.getLoanAmount() << endl;
//        cout << "  Down Payment: PKR " << app.getDownPayment() << endl;
//        cout << "  Installment Period: " << app.getInstallmentMonths() << " months" << endl;
//        cout << "  Monthly Payment: PKR " << app.getMonthlyPayment() << endl;
//
//        cout << endl << "EXISTING LOANS: " << app.getExistingLoansCount() << " loan(s)" << endl;
//        cout << "========================================" << endl;
//    }
//    displayWelcomeMessage();
//
//    // Main application loop
//    bool systemRunning = true;
//    while (systemRunning) {
//        displayRoleSelection();
//
//        string roleInput;
//        cout << "You: ";
//        getline(cin, roleInput);
//        roleInput = toLower(trim(roleInput));
//
//        if (roleInput == Config::EXIT_COMMAND) {
//            displayGoodbyeMessage();
//            break;
//        }
//
//        // Route to appropriate mode
//        if (roleInput == "l" || roleInput == "lender") {
//            handleLenderMode(fileManager);
//        }
//        else if (roleInput == "u" || roleInput == "user") {
//            handleUserMode(homeLoans, homeLoanCount, carLoans, carLoanCount,
//                scooterLoans, scooterLoanCount, utterances, utteranceCount,
//                collector, fileManager);
//        }
//        else {
//            cout << "Invalid selection. Please press L for Lender or U for User." << endl;
//        }
//
//        // Prompt for continuation
//        if (systemRunning) {
//            cout << endl << "Return to role selection? (Y/N): ";
//            getline(cin, roleInput);
//            if (toLower(trim(roleInput)) != "y" && toLower(trim(roleInput)) != "yes") {
//                systemRunning = false;
//                displayGoodbyeMessage();
//            }
//        }
//    }
//
//    return 0;
//}