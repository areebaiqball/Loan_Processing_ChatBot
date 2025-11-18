#include <iostream>
#include <string>
#include "utterances.h"
#include "home.h"
#include "utilities.h"
#include "application.h"
#include "application_collector.h"
#include "file_manager.h"
#include <vector>
#include <iomanip>
#include "car.h"
#include "scooter.h"
using namespace std;

namespace Config {
    const string CHATBOT_NAME = "Dora";
    const string UTTERANCES_FILE = "Utterances.txt";
    const string HOME_LOANS_FILE = "Home.txt";
    const string CAR_LOANS_FILE = "Car.txt";
    const string SCOOTER_LOANS_FILE = "Scooter.txt";
    const int MAX_UTTERANCES = 100;
    const int MAX_LOANS = 100;
    const char DELIMITER = '#';
    const string EXIT_COMMAND = "x";
    const string APPLICATIONS_FILE = "applications.txt";
    const string IMAGES_DIRECTORY = "images/";
}
void displayApplicationStatistics(FileManager& fileManager);
void searchApplicationById(FileManager& fileManager);
bool isValidAreaInput(const string& input) {
    return (input == "1" || input == "2" || input == "3" || input == "4");
}

bool isValidLoanType(const string& input) {
    return (input == "h" || input == "c" || input == "s" || input == "p");
}

bool isValidMakeInput(const string& input, const string& loanType) {
    if (loanType == "car") {
        return (input == "1" || input == "2");
    }
    else if (loanType == "scooter") {
        return (input == "1");
    }
    return false;
}

void displayWelcomeMessage() {
    cout << "==========================================" << endl;
    cout << "   Loan Processing System - " << Config::CHATBOT_NAME << endl;
    cout << "==========================================" << endl << endl;
}

void displayRoleSelection() {
    cout << "Please select your role:" << endl;
    cout << "  Press L for Lender" << endl;
    cout << "  Press U for User/Applicant" << endl;
    cout << "  Press X to exit" << endl;
}

void displayLoanCategories() {
    cout << Config::CHATBOT_NAME << ": Please select the category you want to apply for." << endl;
    cout << "  Press H for a home loan" << endl;
    cout << "  Press C for a car loan" << endl;
    cout << "  Press S for a scooter loan" << endl;
    cout << "  Press P for a personal loan" << endl;
    cout << "  Press X to exit" << endl;
}

void displayGoodbyeMessage() {
    cout << Config::CHATBOT_NAME << ": Thank you for using our service. Goodbye!" << endl;
}

void displayCarMakes() {
    cout << Config::CHATBOT_NAME << ": Please select the car make:" << endl;
    cout << "  Press 1 for Make 1" << endl;
    cout << "  Press 2 for Make 2" << endl;
    cout << "  Press X to exit" << endl;
}

void displayScooterMakes() {
    cout << Config::CHATBOT_NAME << ": Please select the scooter make:" << endl;
    cout << "  Press 1 for Make 1" << endl;
    cout << "  Press X to exit" << endl;
}

void displayLenderMenu() {
    cout << endl << "========== LENDER MENU ==========" << endl;
    cout << "  1. View Pending Applications" << endl;
    cout << "  2. View Approved Applications" << endl;
    cout << "  3. View Rejected Applications" << endl;
    cout << "  4. Review and Process Applications" << endl;
    cout << "  5. Application Statistics" << endl;
    cout << "  6. Search Application by ID" << endl;
    cout << "  X. Exit to Main Menu" << endl;
    cout << "=================================" << endl;
}

void displayApplicationDetails(const LoanApplication& app) {
    cout << endl << "========================================" << endl;
    cout << "APPLICATION DETAILS" << endl;
    cout << "========================================" << endl;
    cout << "Application ID: " << app.getApplicationId() << endl;
    cout << "Status: " << app.getStatus() << endl;
    cout << "Submission Date: " << app.getSubmissionDate() << endl;
    cout << endl << "PERSONAL INFORMATION:" << endl;
    cout << "  Full Name: " << app.getFullName() << endl;
    cout << "  Father's Name: " << app.getFathersName() << endl;
    cout << "  CNIC: " << app.getCnicNumber() << endl;
    cout << "  Contact: " << app.getContactNumber() << endl;
    cout << "  Email: " << app.getEmailAddress() << endl;
    cout << "  Address: " << app.getPostalAddress() << endl;
    cout << endl << "EMPLOYMENT & FINANCIAL:" << endl;
    cout << "  Employment: " << app.getEmploymentStatus() << endl;
    cout << "  Marital Status: " << app.getMaritalStatus() << endl;
    cout << "  Gender: " << app.getGender() << endl;
    cout << "  Dependents: " << app.getNumberOfDependents() << endl;
    cout << "  Annual Income: PKR " << app.getAnnualIncome() << endl;
    cout << endl << "EXISTING LOANS: " << app.getExistingLoansCount() << " loan(s)" << endl;
    cout << "========================================" << endl;
}
    void viewApplicationsByStatus(FileManager & fileManager, const string & status) {
        auto allApplications = fileManager.loadAllApplications();
        vector<LoanApplication> filteredApps;

        for (size_t i = 0; i < allApplications.size(); i++) {
            if (allApplications[i].getStatus() == status) {
                filteredApps.push_back(allApplications[i]);
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
                << " | Name: " << app.getFullName()
                << " | Date: " << app.getSubmissionDate()
                << " | Income: PKR " << app.getAnnualIncome()
                << " | Employment: " << app.getEmploymentStatus() << endl;
        }
        cout << "========================================" << endl;
    }
void reviewAndProcessApplications(FileManager& fileManager) {
    auto allApplications = fileManager.loadAllApplications();
    vector<LoanApplication> pendingApps;

    // Filter pending applications
    for (size_t i = 0; i < allApplications.size(); i++) {
        if (allApplications[i].getStatus() == "submitted") {
            pendingApps.push_back(allApplications[i]);
        }
    }

    if (pendingApps.empty()) {
        cout << endl << "No pending applications to review." << endl;
        return;
    }

    cout << endl << "========== PENDING APPLICATIONS ==========" << endl;
    for (size_t i = 0; i < pendingApps.size(); i++) {
        cout << (i + 1) << ". ID: " << pendingApps[i].getApplicationId()
            << " | Name: " << pendingApps[i].getFullName()
            << " | Date: " << pendingApps[i].getSubmissionDate()
            << " | Income: PKR " << pendingApps[i].getAnnualIncome() << endl;
    }

    cout << endl << "Enter application number to review (or X to cancel): ";
    string input;
    getline(cin, input);
    input = trim(input);

    if (toLower(input) == "x") {
        return;
    }

    try {
        int appNum = stoi(input);
        if (appNum < 1 || appNum > static_cast<int>(pendingApps.size())) {
            cout << "Invalid application number." << endl;
            return;
        }

        LoanApplication selectedApp = pendingApps[appNum - 1];

        // Load full application details
        LoanApplication fullApp = fileManager.findApplicationById(selectedApp.getApplicationId());
        if (fullApp.getApplicationId().empty()) {
            cout << "Error: Could not load application details." << endl;
            return;
        }

        displayApplicationDetails(fullApp);

        cout << endl << "Action: (A)pprove or (R)eject this application? ";
        getline(cin, input);
        input = toLower(trim(input));

        string newStatus;
        if (input == "a" || input == "approve") {
            newStatus = "approved";
            cout << "Application " << fullApp.getApplicationId() << " will be APPROVED." << endl;
        }
        else if (input == "r" || input == "reject") {
            newStatus = "rejected";
            cout << "Application " << fullApp.getApplicationId() << " will be REJECTED." << endl;
        }
        else {
            cout << "Invalid action. No changes made." << endl;
            return;
        }

        // Ask for confirmation
        cout << "Confirm this action? (Y/N): ";
        getline(cin, input);
        input = toLower(trim(input));

        if (input == "y" || input == "yes") {
            // Update the application status in file
            if (fileManager.updateApplicationStatus(fullApp.getApplicationId(), newStatus)) {
                cout << "Application status updated successfully!" << endl;

                // Optional: Add reason for rejection
                if (newStatus == "rejected") {
                    cout << "Enter reason for rejection (optional): ";
                    getline(cin, input);
                    // You could store this reason in a separate file or log
                    if (!input.empty()) {
                        cout << "Reason noted: " << input << endl;
                    }
                }
            }
            else {
                cout << "Failed to update application status." << endl;
            }
        }
        else {
            cout << "Action cancelled." << endl;
        }

    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}
void handleLenderMode(FileManager& fileManager) {
    bool inLenderMode = true;
    string input;

    while (inLenderMode) {
        displayLenderMenu();
        cout << "Lender: ";
        getline(cin, input);
        input = toLower(trim(input));

        if (input == "x") {
            inLenderMode = false;
        }
        else if (input == "1") {
            viewApplicationsByStatus(fileManager, "submitted");
        }
        else if (input == "2") {
            viewApplicationsByStatus(fileManager, "approved");
        }
        else if (input == "3") {
            viewApplicationsByStatus(fileManager, "rejected");
        }
        else if (input == "4") {
            reviewAndProcessApplications(fileManager);
        }
        else if (input == "5") {
            displayApplicationStatistics(fileManager);
        }
        else if (input == "6") {
            searchApplicationById(fileManager);
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }

        if (inLenderMode) {
            cout << endl << "Press any key to continue...";
            getline(cin, input);
        }
    }
}

void handleHomeLoanSelection(const HomeLoan loans[], int loanCount, bool& running,
    ApplicationCollector& collector, FileManager& fileManager) {

    cout << Config::CHATBOT_NAME << ": You selected home loan. Please select area (1, 2, 3, or 4):" << endl;

    bool validArea = false;
    string userInput;

    while (!validArea && running) {
        cout << "You: ";
        getline(cin, userInput);
        string areaInput = trim(userInput);
        string lowerAreaInput = toLower(areaInput);

        if (lowerAreaInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            running = false;
            return;
        }

        if (isValidAreaInput(areaInput)) {
            bool hasData = displayHomeLoanOptions(loans, loanCount, areaInput);

            if (hasData) {
                cout << endl << Config::CHATBOT_NAME << ": "
                    << "Would you like to see the detailed installment plan? (Y/N): ";

                getline(cin, userInput);
                string planInput = toLower(trim(userInput));

                if (planInput == Config::EXIT_COMMAND) {
                    displayGoodbyeMessage();
                    running = false;
                    return;
                }

                if (planInput == "y" || planInput == "yes") {
                    cout << Config::CHATBOT_NAME << ": Enter option number to see installment plan: ";
                    getline(cin, userInput);

                    if (toLower(trim(userInput)) == Config::EXIT_COMMAND) {
                        displayGoodbyeMessage();
                        running = false;
                        return;
                    }

                    try {
                        int optionNumber = stoi(trim(userInput));
                        displayInstallmentPlanForOption(loans, loanCount, areaInput, optionNumber);
                    }
                    catch (const exception&) {
                        cout << Config::CHATBOT_NAME << ": Invalid option number." << endl;
                        continue;
                    }
                }

                cout << endl << Config::CHATBOT_NAME << ": Would you like to start the loan application process? (Y/N): ";
                getline(cin, userInput);
                string applyInput = toLower(trim(userInput));

                if (applyInput == Config::EXIT_COMMAND) {
                    displayGoodbyeMessage();
                    running = false;
                    return;
                }

                if (applyInput == "y" || applyInput == "yes") {
                    try {
                        LoanApplication application = collector.collectApplicationForLoan("home", "Home Loan", fileManager);

                        cout << endl << Config::CHATBOT_NAME << ": Please review your application details:" << endl;
                        displayApplicationDetails(application);

                        cout << endl << Config::CHATBOT_NAME << ": Do you want to submit this application? (Y/N): ";
                        getline(cin, userInput);
                        string confirmInput = toLower(trim(userInput));

                        if (confirmInput == "y" || confirmInput == "yes") {
                            if (fileManager.saveApplication(application)) {
                                cout << endl << Config::CHATBOT_NAME << ": Application submitted successfully!" << endl;
                                cout << "Your Application ID: " << application.getApplicationId() << endl;

                                // Return after successful submission
                                cout << endl << "Press any key to return to main menu...";
                                getline(cin, userInput);
                                return;
                            }
                            else {
                                cout << Config::CHATBOT_NAME << ": Failed to save application." << endl;
                            }
                        }
                        else {
                            cout << Config::CHATBOT_NAME << ": Application submission cancelled." << endl;
                        }
                    }
                    catch (const exception& e) {
                        cout << Config::CHATBOT_NAME << ": Application failed: " << e.what() << endl;
                    }
                }

                validArea = true;
                cout << endl << "Press X to exit or any other key to return to main menu: ";
                getline(cin, userInput);
                if (toLower(trim(userInput)) == Config::EXIT_COMMAND) {
                    running = false;
                }
                return;
            }
        }
        else {
            cout << Config::CHATBOT_NAME << ": Invalid area. Please enter 1, 2, 3, or 4:" << endl;
        }
    }
}

/// <summary>
/// Displays car loan options in tabular format
/// </summary>
bool displayCarLoanOptionsByMake(const CarLoan loans[], int size, const string& makeNumber) {
    string makeName = "Make " + makeNumber;
    bool found = false;

    cout << endl << "  Car Loan Options for " << makeName << endl;
    cout << "+-----+-----------+---------+----------+-------------+-------------+---------------+---------------------+" << endl;
    cout << "| Opt |   Model   | Engine  | Condition| Year        | Total Price | Down Payment  | Monthly Installment |" << endl;
    cout << "+-----+-----------+---------+----------+-------------+-------------+---------------+---------------------+" << endl;

    int optionCount = 0;
    for (int i = 0; i < size; i++) {
        if (loans[i].getMake() == makeName) {
            found = true;
            optionCount++;
            long long monthlyInstallment = loans[i].calculateMonthlyInstallment();
            cout << "| " << setw(3) << optionCount << " | "
                << setw(9) << loans[i].getModel() << " | "
                << setw(7) << loans[i].getEngine() << "cc | "
                << setw(8) << loans[i].getUsed() << " | "
                << setw(11) << loans[i].getManufacturingYear() << " | "
                << "PKR " << setw(8) << loans[i].getPrice() << " | "
                << "PKR " << setw(10) << loans[i].getDownPayment() << " | "
                << "PKR " << setw(15) << monthlyInstallment << " |" << endl;
        }
    }

    cout << "+-----+-----------+---------+----------+-------------+-------------+---------------+---------------------+" << endl;

    if (!found) {
        cout << "No car options available for this make." << endl;
    }

    return found;
}

void handleCarLoanSelection(const CarLoan loans[], int loanCount, bool& running,
    ApplicationCollector& collector, FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": You selected car loan." << endl;
    displayCarMakes();

    string userInput;
    cout << "You: ";
    getline(cin, userInput);
    string makeInput = trim(userInput);

    if (toLower(makeInput) == Config::EXIT_COMMAND) {
        displayGoodbyeMessage();
        running = false;
        return;
    }

    if (isValidMakeInput(makeInput, "car")) {
        bool hasData = displayCarLoanOptionsByMake(loans, loanCount, makeInput);

        if (hasData) {
            cout << endl << Config::CHATBOT_NAME << ": Would you like to see the detailed installment plan? (Y/N): ";
            getline(cin, userInput);
            string planInput = toLower(trim(userInput));

            if (planInput == Config::EXIT_COMMAND) {
                displayGoodbyeMessage();
                running = false;
                return;
            }

            if (planInput == "y" || planInput == "yes") {
                cout << Config::CHATBOT_NAME << ": Enter option number: ";
                getline(cin, userInput);

                if (toLower(trim(userInput)) == Config::EXIT_COMMAND) {
                    displayGoodbyeMessage();
                    running = false;
                    return;
                }

                try {
                    int optionNumber = stoi(trim(userInput));
                    displayCarInstallmentPlan(loans, loanCount, optionNumber);
                }
                catch (const exception&) {
                    cout << Config::CHATBOT_NAME << ": Invalid option number." << endl;
                }
            }

            cout << endl << Config::CHATBOT_NAME << ": Would you like to start the loan application process? (Y/N): ";
            getline(cin, userInput);
            string applyInput = toLower(trim(userInput));

            if (applyInput == "y" || applyInput == "yes") {
                try {
                    LoanApplication application = collector.collectApplicationForLoan("car", "Car Loan", fileManager);

                    cout << endl << Config::CHATBOT_NAME << ": Please review your application:" << endl;
                    displayApplicationDetails(application);

                    cout << endl << Config::CHATBOT_NAME << ": Submit this application? (Y/N): ";
                    getline(cin, userInput);

                    if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                        if (fileManager.saveApplication(application)) {
                            cout << endl << Config::CHATBOT_NAME << ": Application submitted successfully!" << endl;
                            cout << "Your Application ID: " << application.getApplicationId() << endl;

                            // Return after successful submission
                            cout << endl << "Press any key to return to main menu...";
                            getline(cin, userInput);
                            return;
                        }
                        else {
                            cout << Config::CHATBOT_NAME << ": Failed to save application." << endl;
                        }
                    }
                    else {
                        cout << Config::CHATBOT_NAME << ": Application cancelled." << endl;
                    }
                }
                catch (const exception& e) {
                    cout << Config::CHATBOT_NAME << ": Application failed: " << e.what() << endl;
                }
            }

            cout << endl << "Returning to main menu..." << endl;
            return;
        }
    }
}

bool displayScooterLoanOptionsByMake(const ScooterLoan loans[], int size, const string& makeNumber) {
    string makeName = "Make " + makeNumber;
    bool found = false;
    int optionCount = 0;

    cout << endl << "  Scooter Loan Options for " << makeName << endl << endl;

    for (int i = 0; i < size; i++) {
        if (loans[i].getMake() == makeName) {
            found = true;
            optionCount++;
            long long monthlyInstallment = loans[i].calculateMonthlyInstallment();
            cout << "Option " << optionCount << ":" << endl;
            cout << "  Model: " << loans[i].getModel() << endl;
            cout << "  Range: " << loans[i].getDistancePerCharge() << " KM per charge" << endl;
            cout << "  Total Price: PKR " << loans[i].getPrice() << endl;
            cout << "  Down Payment: PKR " << loans[i].getDownPayment() << endl;
            cout << "  Monthly Installment: PKR " << monthlyInstallment << endl;
            cout << "----------------------------------------" << endl;
        }
    }

    if (!found) {
        cout << "Sorry, no scooter options available." << endl;
    }

    return found;
}

void handleScooterLoanSelection(const ScooterLoan loans[], int loanCount, bool& running,
    ApplicationCollector& collector, FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": You selected scooter loan." << endl;
    displayScooterMakes();

    string userInput;
    cout << "You: ";
    getline(cin, userInput);
    string makeInput = trim(userInput);

    if (toLower(makeInput) == Config::EXIT_COMMAND) {
        displayGoodbyeMessage();
        running = false;
        return;
    }

    if (isValidMakeInput(makeInput, "scooter")) {
        // Use the new tabular display
        bool hasData = displayScooterLoanOptionsTable(loans, loanCount, makeInput);

        if (hasData) {
            cout << endl << Config::CHATBOT_NAME << ": Would you like to see the detailed installment plan? (Y/N): ";
            getline(cin, userInput);

            if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                cout << Config::CHATBOT_NAME << ": Enter option number: ";
                getline(cin, userInput);

                try {
                    int optionNumber = stoi(trim(userInput));
                    displayScooterInstallmentPlan(loans, loanCount, optionNumber);
                }
                catch (const exception&) {
                    cout << Config::CHATBOT_NAME << ": Invalid option number." << endl;
                }
            }

            cout << endl << Config::CHATBOT_NAME << ": Would you like to start the loan application process? (Y/N): ";
            getline(cin, userInput);

            if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                try {
                    LoanApplication application = collector.collectApplicationForLoan("scooter", "Scooter Loan", fileManager);

                    displayApplicationDetails(application);

                    cout << endl << Config::CHATBOT_NAME << ": Submit application? (Y/N): ";
                    getline(cin, userInput);

                    if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                        if (fileManager.saveApplication(application)) {
                            cout << endl << Config::CHATBOT_NAME << ": Application submitted!" << endl;
                            cout << "Application ID: " << application.getApplicationId() << endl;
                            return;
                        }
                    }
                }
                catch (const exception& e) {
                    cout << Config::CHATBOT_NAME << ": Application failed: " << e.what() << endl;
                }
            }

            cout << endl << "Press X to exit or any key for main menu: ";
            getline(cin, userInput);
            if (toLower(trim(userInput)) == Config::EXIT_COMMAND) {
                running = false;
            }
            return;
        }
    }
    else {
        cout << Config::CHATBOT_NAME << ": Invalid make selection." << endl;
    }
}void handlePersonalLoanSelection(bool& running, ApplicationCollector& collector, FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": Personal loan selected." << endl;
    cout << "Personal loans available with flexible terms." << endl;

    cout << endl << Config::CHATBOT_NAME << ": Start application? (Y/N): ";
    string userInput;
    getline(cin, userInput);

    if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
        try {
            LoanApplication application = collector.collectApplicationForLoan("personal", "Personal Loan", fileManager);

            displayApplicationDetails(application);

            cout << endl << Config::CHATBOT_NAME << ": Submit? (Y/N): ";
            getline(cin, userInput);

            if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                if (fileManager.saveApplication(application)) {
                    cout << Config::CHATBOT_NAME << ": Application submitted!" << endl;
                    cout << "ID: " << application.getApplicationId() << endl;
                    return;
                }
            }
        }
        catch (const exception& e) {
            cout << Config::CHATBOT_NAME << ": Failed: " << e.what() << endl;
        }
    }

    cout << endl << "Press X to exit or any key for main menu: ";
    getline(cin, userInput);
    if (toLower(trim(userInput)) == Config::EXIT_COMMAND) {
        running = false;
    }
}

void handleUserMode(const HomeLoan homeLoans[], int homeLoanCount,
    const CarLoan carLoans[], int carLoanCount,
    const ScooterLoan scooterLoans[], int scooterLoanCount,
    const Utterance utterances[], int utteranceCount,
    ApplicationCollector& collector, FileManager& fileManager) {

    bool running = true;
    string userInput;

    cout << endl << Config::CHATBOT_NAME << ": Hello! I'm " << Config::CHATBOT_NAME
        << ", your loan assistant." << endl;
    cout << Config::CHATBOT_NAME << ": I'm here to help you find the perfect loan." << endl;

    while (running) {
        cout << Config::CHATBOT_NAME << ": Press A to apply for a loan, or type your message: ";
        getline(cin, userInput);
        string lowerInput = toLower(trim(userInput));

        if (lowerInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            break;
        }

        if (lowerInput == "a") {
            displayLoanCategories();
            cout << "You: ";
            getline(cin, userInput);
            lowerInput = toLower(trim(userInput));

            if (lowerInput == Config::EXIT_COMMAND) {
                displayGoodbyeMessage();
                break;
            }

            if (lowerInput == "h") {
                handleHomeLoanSelection(homeLoans, homeLoanCount, running, collector, fileManager);
            }
            else if (lowerInput == "c") {
                handleCarLoanSelection(carLoans, carLoanCount, running, collector, fileManager);
            }
            else if (lowerInput == "s") {
                handleScooterLoanSelection(scooterLoans, scooterLoanCount, running, collector, fileManager);
            }
            else if (lowerInput == "p") {
                handlePersonalLoanSelection(running, collector, fileManager);
            }
            else {
                cout << Config::CHATBOT_NAME << ": Invalid selection." << endl;
            }
        }
        else {
            cout << Config::CHATBOT_NAME << ": " << getResponse(utterances, utteranceCount, userInput) << endl;
        }
    }
}
void displayApplicationStatistics(FileManager& fileManager) {
    auto allApplications = fileManager.loadAllApplications();

    int submitted = 0, approved = 0, rejected = 0, total = 0;

    for (const auto& app : allApplications) {
        string status = app.getStatus();
        if (status == "submitted") submitted++;
        else if (status == "approved") approved++;
        else if (status == "rejected") rejected++;
        total++;
    }

    cout << endl << "========== APPLICATION STATISTICS ==========" << endl;
    cout << "Total Applications: " << total << endl;
    cout << "Pending Review: " << submitted << endl;
    cout << "Approved: " << approved << endl;
    cout << "Rejected: " << rejected << endl;

    if (total > 0) {
        cout << fixed << setprecision(1);
        cout << "Approval Rate: " << (approved * 100.0 / total) << "%" << endl;
        cout << "Rejection Rate: " << (rejected * 100.0 / total) << "%" << endl;
    }
    cout << "============================================" << endl;
}

void searchApplicationById(FileManager& fileManager) {
    cout << endl << "Enter Application ID to search: ";
    string appId;
    getline(cin, appId);
    appId = trim(appId);

    if (appId.empty()) {
        cout << "Invalid Application ID." << endl;
        return;
    }

    LoanApplication app = fileManager.findApplicationById(appId);
    if (app.getApplicationId().empty()) {
        cout << "Application with ID " << appId << " not found." << endl;
        return;
    }

    displayApplicationDetails(app);

    cout << endl << "Options:" << endl;
    cout << "1. Change Status" << endl;
    cout << "2. Return to Menu" << endl;
    cout << "Choice: ";

    string choice;
    getline(cin, choice);
    choice = trim(choice);

    if (choice == "1") {
        cout << "New Status (approved/rejected/submitted): ";
        string newStatus;
        getline(cin, newStatus);
        newStatus = toLower(trim(newStatus));

        if (newStatus == "approved" || newStatus == "rejected" || newStatus == "submitted") {
            if (fileManager.updateApplicationStatus(appId, newStatus)) {
                cout << "Status updated successfully!" << endl;
            }
            else {
                cout << "Failed to update status." << endl;
            }
        }
        else {
            cout << "Invalid status." << endl;
        }
    }
}

int main() {
    // Initialize data structures
    Utterance utterances[Config::MAX_UTTERANCES];
    HomeLoan homeLoans[Config::MAX_LOANS];
    CarLoan carLoans[Config::MAX_LOANS];
    ScooterLoan scooterLoans[Config::MAX_LOANS];
    FileManager fileManager;
    ApplicationCollector collector;

    // Load data
    int utteranceCount = loadUtterances(utterances, Config::MAX_UTTERANCES, Config::UTTERANCES_FILE);
    int homeLoanCount = loadHomeLoans(homeLoans, Config::MAX_LOANS, Config::HOME_LOANS_FILE);
    int carLoanCount = loadCarLoans(carLoans, Config::MAX_LOANS, Config::CAR_LOANS_FILE);
    int scooterLoanCount = loadScooterLoans(scooterLoans, Config::MAX_LOANS, Config::SCOOTER_LOANS_FILE);

    if (utteranceCount == 0) {
        cerr << "Failed to load utterances." << endl;
        return 1;
    }

    displayWelcomeMessage();

    bool systemRunning = true;
    while (systemRunning) {
        displayRoleSelection();

        string roleInput;
        cout << "You: ";
        getline(cin, roleInput);
        roleInput = toLower(trim(roleInput));

        if (roleInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            break;
        }

        if (roleInput == "l" || roleInput == "lender") {
            handleLenderMode(fileManager);
        }
        else if (roleInput == "u" || roleInput == "user") {
            handleUserMode(homeLoans, homeLoanCount, carLoans, carLoanCount,
                scooterLoans, scooterLoanCount, utterances, utteranceCount,
                collector, fileManager);
        }
        else {
            cout << "Invalid selection. Please press L for Lender or U for User." << endl;
        }

        cout << endl << "Return to role selection? (Y/N): ";
        getline(cin, roleInput);
        if (toLower(trim(roleInput)) != "y" && toLower(trim(roleInput)) != "yes") {
            systemRunning = false;
            displayGoodbyeMessage();
        }
    }

    return 0;
}