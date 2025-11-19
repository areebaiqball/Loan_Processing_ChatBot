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
void checkApplicationStatusByCNIC(FileManager& fileManager);
void displayApplicationStatistics(FileManager& fileManager);
void searchApplicationById(FileManager& fileManager);
void viewAllApplications(FileManager& fileManager);
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
    cout << "  1. View ALL Applications" << endl;
    cout << "  2. View Pending Applications" << endl;
    cout << "  3. View Approved Applications" << endl;
    cout << "  4. View Rejected Applications" << endl;
    cout << "  5. Review and Process Applications" << endl;
    cout << "  6. Application Statistics" << endl;
    cout << "  7. Search Application by ID" << endl;
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
            cout << "Application " << fullApp.getApplicationId() << " has been APPROVED." << endl;
            cout << "The applicant can now generate their installment plan when checking status." << endl;
            // Display loan details from application
            cout << endl << "========== LOAN DETAILS ==========" << endl;
            cout << "Loan Type: " << fullApp.getLoanType() << endl;
            cout << "Category: " << fullApp.getLoanCategory() << endl;
            cout << "Total Amount: PKR " << fullApp.getLoanAmount() << endl;
            cout << "Down Payment: PKR " << fullApp.getDownPayment() << endl;
            cout << "Installment Period: " << fullApp.getInstallmentMonths() << " months" << endl;
            cout << "Monthly Payment: PKR " << fullApp.getMonthlyPayment() << endl;
            cout << "===================================" << endl;

            // Ask for starting month with validation
            int startMonth = 0;
            while (startMonth < 1 || startMonth > 12) {
                cout << endl << "Enter starting month (1=Jan, 2=Feb, ..., 12=Dec): ";
                string monthInput;
                getline(cin, monthInput);

                try {
                    startMonth = stoi(trim(monthInput));
                    if (startMonth < 1 || startMonth > 12) {
                        cout << "Month must be between 1 and 12." << endl;
                    }
                }
                catch (...) {
                    cout << "Invalid input. Please enter a number (1-12)." << endl;
                    startMonth = 0;
                }
            }

            // Ask for starting year with validation
            int startYear = 0;
            while (startYear < 2024 || startYear > 2100) {
                cout << "Enter starting year (e.g., 2025): ";
                string yearInput;
                getline(cin, yearInput);

                try {
                    startYear = stoi(trim(yearInput));
                    if (startYear < 2024 || startYear > 2100) {
                        cout << "Year must be between 2024 and 2100." << endl;
                    }
                }
                catch (...) {
                    cout << "Invalid input. Please enter a valid year." << endl;
                    startYear = 0;
                }
            }

            // Get data from stored application
            int totalMonths = fullApp.getInstallmentMonths();
            long long monthlyPayment = fullApp.getMonthlyPayment();
            long long loanAmount = fullApp.getLoanAmount() - fullApp.getDownPayment();

            // Display complete installment plan
            cout << endl << "======================================" << endl;
            cout << "   MONTHLY INSTALLMENT PLAN" << endl;
            cout << "======================================" << endl;
            cout << "Applicant: " << fullApp.getFullName() << endl;
            cout << "CNIC: " << fullApp.getCnicNumber() << endl;
            cout << "Loan Type: " << fullApp.getLoanType() << endl;
            cout << "Category: " << fullApp.getLoanCategory() << endl;
            cout << "Loan Amount: PKR " << loanAmount << endl;
            cout << "Down Payment: PKR " << fullApp.getDownPayment() << endl;
            cout << "Total Months: " << totalMonths << endl;
            cout << "Monthly Payment: PKR " << monthlyPayment << endl;
            cout << "Starting: " << getMonthName(startMonth) << " " << startYear << endl;
            cout << "======================================" << endl << endl;

            int currentMonth = startMonth;
            int currentYear = startYear;
            long long remainingBalance = loanAmount;

            cout << "+------+------------------+------+------------------+------------------+" << endl;
            cout << "| No   | Month            | Year | Payment Due      | Remaining Bal    |" << endl;
            cout << "+------+------------------+------+------------------+------------------+" << endl;

            // Show ALL months (not just 12)
            for (int i = 1; i <= totalMonths; i++) {
                long long payment = monthlyPayment;

                // Last payment adjusts for any remainder
                if (i == totalMonths) {
                    payment = remainingBalance;
                }

                remainingBalance -= payment;
                if (remainingBalance < 0) remainingBalance = 0;

                cout << "| " << setw(4) << i << " | "
                    << setw(16) << left << getMonthName(currentMonth) << " | "
                    << setw(4) << currentYear << " | PKR "
                    << setw(12) << right << payment << " | PKR "
                    << setw(12) << remainingBalance << " |" << endl;

                getNextMonth(currentMonth, currentYear);
            }

            cout << "+------+------------------+------+------------------+------------------+" << endl;
            cout << "======================================" << endl;
            cout << "Total Amount Paid: PKR " << (fullApp.getDownPayment() + (monthlyPayment * totalMonths)) << endl;
            cout << "======================================" << endl;
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

                int optionNumber = 0; // Declare here so it's in scope

                if (planInput == "y" || planInput == "yes") {
                    cout << Config::CHATBOT_NAME << ": Enter option number to see installment plan: ";
                    getline(cin, userInput);

                    if (toLower(trim(userInput)) == Config::EXIT_COMMAND) {
                        displayGoodbyeMessage();
                        running = false;
                        return;
                    }

                    try {
                        optionNumber = stoi(trim(userInput));
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
                    // If optionNumber wasn't set (user didn't view plan), ask for it
                    if (optionNumber == 0) {
                        cout << Config::CHATBOT_NAME << ": Which option would you like to apply for? Enter option number: ";
                        getline(cin, userInput);

                        if (toLower(trim(userInput)) == Config::EXIT_COMMAND) {
                            displayGoodbyeMessage();
                            running = false;
                            return;
                        }

                        try {
                            optionNumber = stoi(trim(userInput));
                        }
                        catch (const exception&) {
                            cout << Config::CHATBOT_NAME << ": Invalid option number." << endl;
                            continue;
                        }
                    }

                    try {
                        LoanApplication application = collector.collectApplicationForLoan("home", "Home Loan", fileManager);

                        // Store the selected loan details
                        application.setLoanType("Home Loan");
                        application.setLoanCategory("Area " + areaInput + " - Option " + to_string(optionNumber));

                        // Get the actual loan details from the loans array
                        string areaName = "Area " + areaInput;
                        int currentOption = 0;
                        bool loanFound = false;

                        for (int i = 0; i < loanCount; i++) {
                            if (loans[i].getArea() == areaName) {
                                currentOption++;
                                if (currentOption == optionNumber) {
                                    application.setLoanAmount(loans[i].getPrice());
                                    application.setDownPayment(loans[i].getDownPayment());
                                    application.setInstallmentMonths(loans[i].getInstallments());
                                    application.setMonthlyPayment(loans[i].calculateMonthlyInstallment());
                                    loanFound = true;
                                    break;
                                }
                            }
                        }

                        if (!loanFound) {
                            cout << Config::CHATBOT_NAME << ": Error: Could not retrieve loan details." << endl;
                            continue;
                        }

                        cout << endl << Config::CHATBOT_NAME << ": Please review your application details:" << endl;
                        displayApplicationDetails(application);

                        cout << endl << Config::CHATBOT_NAME << ": Do you want to submit this application? (Y/N): ";
                        getline(cin, userInput);
                        string confirmInput = toLower(trim(userInput));

                        if (confirmInput == "y" || confirmInput == "yes") {
                            if (fileManager.saveApplication(application)) {
                                cout << endl << Config::CHATBOT_NAME << ": Application submitted successfully!" << endl;
                                cout << "Your Application ID: " << application.getApplicationId() << endl;
                                cout << endl << "----------------------------------------" << endl;
                                cout << "You can check your application status anytime by:" << endl;
                                cout << "1. Saying 'check my applications'" << endl;
                                cout << "2. Providing your CNIC: " << application.getCnicNumber() << endl;
                                cout << "----------------------------------------" << endl;

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
            int optionNumber = 0; // Declare here

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
                    optionNumber = stoi(trim(userInput));
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
                // If optionNumber wasn't set, ask for it
                if (optionNumber == 0) {
                    cout << Config::CHATBOT_NAME << ": Which option would you like to apply for? Enter option number: ";
                    getline(cin, userInput);

                    try {
                        optionNumber = stoi(trim(userInput));
                    }
                    catch (const exception&) {
                        cout << Config::CHATBOT_NAME << ": Invalid option number." << endl;
                        return;
                    }
                }

                try {
                    LoanApplication application = collector.collectApplicationForLoan("car", "Car Loan", fileManager);

                    // Store the selected loan details
                    application.setLoanType("Car Loan");
                    application.setLoanCategory("Make " + makeInput + " - Option " + to_string(optionNumber));

                    // Get the actual loan details from the loans array
                    string makeName = "Make " + makeInput;
                    int currentOption = 0;
                    bool loanFound = false;

                    for (int i = 0; i < loanCount; i++) {
                        if (loans[i].getMake() == makeName) {
                            currentOption++;
                            if (currentOption == optionNumber) {
                                application.setLoanAmount(loans[i].getPrice());
                                application.setDownPayment(loans[i].getDownPayment());
                                application.setInstallmentMonths(loans[i].getInstallments());
                                application.setMonthlyPayment(loans[i].calculateMonthlyInstallment());
                                loanFound = true;
                                break;
                            }
                        }
                    }

                    if (!loanFound) {
                        cout << Config::CHATBOT_NAME << ": Error: Could not retrieve loan details." << endl;
                        return;
                    }

                    cout << endl << Config::CHATBOT_NAME << ": Please review your application:" << endl;
                    displayApplicationDetails(application);

                    cout << endl << Config::CHATBOT_NAME << ": Submit this application? (Y/N): ";
                    getline(cin, userInput);

                    if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                        if (fileManager.saveApplication(application)) {
                            cout << endl << Config::CHATBOT_NAME << ": Application submitted successfully!" << endl;
                            cout << "Your Application ID: " << application.getApplicationId() << endl;
                            cout << endl << "----------------------------------------" << endl;
                            cout << "You can check your application status anytime by:" << endl;
                            cout << "1. Saying 'check my applications'" << endl;
                            cout << "2. Providing your CNIC: " << application.getCnicNumber() << endl;
                            cout << "----------------------------------------" << endl;

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
        bool hasData = displayScooterLoanOptionsTable(loans, loanCount, makeInput);

        if (hasData) {
            int optionNumber = 0; // Declare here

            cout << endl << Config::CHATBOT_NAME << ": Would you like to see the detailed installment plan? (Y/N): ";
            getline(cin, userInput);

            if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                cout << Config::CHATBOT_NAME << ": Enter option number: ";
                getline(cin, userInput);

                try {
                    optionNumber = stoi(trim(userInput));
                    displayScooterInstallmentPlan(loans, loanCount, optionNumber);
                }
                catch (const exception&) {
                    cout << Config::CHATBOT_NAME << ": Invalid option number." << endl;
                }
            }

            cout << endl << Config::CHATBOT_NAME << ": Would you like to start the loan application process? (Y/N): ";
            getline(cin, userInput);

            if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                // If optionNumber wasn't set, ask for it
                if (optionNumber == 0) {
                    cout << Config::CHATBOT_NAME << ": Which option would you like to apply for? Enter option number: ";
                    getline(cin, userInput);

                    try {
                        optionNumber = stoi(trim(userInput));
                    }
                    catch (const exception&) {
                        cout << Config::CHATBOT_NAME << ": Invalid option number." << endl;
                        return;
                    }
                }

                try {
                    LoanApplication application = collector.collectApplicationForLoan("scooter", "Scooter Loan", fileManager);

                    // Store the selected loan details
                    application.setLoanType("Scooter Loan");
                    application.setLoanCategory("Make " + makeInput + " - Option " + to_string(optionNumber));

                    // Get the actual loan details from the loans array
                    string makeName = "Make " + makeInput;
                    int currentOption = 0;
                    bool loanFound = false;

                    for (int i = 0; i < loanCount; i++) {
                        if (loans[i].getMake() == makeName) {
                            currentOption++;
                            if (currentOption == optionNumber) {
                                application.setLoanAmount(loans[i].getPrice());
                                application.setDownPayment(loans[i].getDownPayment());
                                application.setInstallmentMonths(loans[i].getInstallments());
                                application.setMonthlyPayment(loans[i].calculateMonthlyInstallment());
                                loanFound = true;
                                break;
                            }
                        }
                    }

                    if (!loanFound) {
                        cout << Config::CHATBOT_NAME << ": Error: Could not retrieve loan details." << endl;
                        return;
                    }

                    displayApplicationDetails(application);

                    cout << endl << Config::CHATBOT_NAME << ": Submit application? (Y/N): ";
                    getline(cin, userInput);

                    if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                        if (fileManager.saveApplication(application)) {
                            cout << endl << Config::CHATBOT_NAME << ": Application submitted!" << endl;
                            cout << "Application ID: " << application.getApplicationId() << endl;
                            cout << endl << "----------------------------------------" << endl;
                            cout << "You can check your application status anytime by:" << endl;
                            cout << "1. Saying 'check my applications'" << endl;
                            cout << "2. Providing your CNIC: " << application.getCnicNumber() << endl;
                            cout << "----------------------------------------" << endl;

                            cout << endl << "Press any key to return to main menu...";
                            getline(cin, userInput);
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
}

void handlePersonalLoanSelection(bool& running, ApplicationCollector& collector, FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": Personal loan selected." << endl;
    cout << "Personal loans available with flexible terms." << endl;

    cout << endl << Config::CHATBOT_NAME << ": Start application? (Y/N): ";
    string userInput;
    getline(cin, userInput);

    if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
        try {
            LoanApplication application = collector.collectApplicationForLoan("personal", "Personal Loan", fileManager);

            // Store personal loan details (no specific product, so use defaults)
            application.setLoanType("Personal Loan");
            application.setLoanCategory("Standard Personal Loan");
            application.setLoanAmount(500000); // Default example amount
            application.setDownPayment(50000);  // Default 10%
            application.setInstallmentMonths(36); // Default 3 years
            application.setMonthlyPayment(12500); // Calculate based on defaults

            displayApplicationDetails(application);

            cout << endl << Config::CHATBOT_NAME << ": Submit? (Y/N): ";
            getline(cin, userInput);

            if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                if (fileManager.saveApplication(application)) {
                    cout << Config::CHATBOT_NAME << ": Application submitted!" << endl;
                    cout << "ID: " << application.getApplicationId() << endl;
                    cout << endl << "----------------------------------------" << endl;
                    cout << "You can check your application status anytime by:" << endl;
                    cout << "1. Saying 'check my applications'" << endl;
                    cout << "2. Providing your CNIC: " << application.getCnicNumber() << endl;
                    cout << "----------------------------------------" << endl;

                    cout << endl << "Press any key to return to main menu...";
                    getline(cin, userInput);
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
    cout << endl << "Options" << endl;
    cout << "  - Type 'A' to apply for a new loan" << endl;
    cout << "  - Type 'check my applications' to view your application status" << endl;
    cout << "  - Type 'X' to exit" << endl;

    while (running) {
        cout << Config::CHATBOT_NAME << ": Press A to apply for a loan, or type your message: ";
        getline(cin, userInput);
        string lowerInput = toLower(trim(userInput));

        if (lowerInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            break;
        }

        // for natural chatbot responses
        if (lowerInput == "check my applications" ||
            lowerInput == "application status" ||
            lowerInput == "check status" ||
            lowerInput == "my application count" ||
            lowerInput == "how many applications" ||
            lowerInput == "check application" ||
            (lowerInput.find("application") != string::npos && lowerInput.find("status") != string::npos) ||
            (lowerInput.find("check") != string::npos && lowerInput.find("application") != string::npos)) {
            checkApplicationStatusByCNIC(fileManager);
            continue; 
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
/// <summary>
/// Generates monthly installment plan for USER (not lender)
/// </summary>
void generateUserInstallmentPlan(const LoanApplication& application) {
    cout << endl << "💫 " << Config::CHATBOT_NAME << ": Generating your monthly installment plan..." << endl;

    // SANITIZE ALL DATA - Fix corrupted values
    long long loanAmount = application.getLoanAmount();
    long long downPayment = application.getDownPayment();
    int totalMonths = application.getInstallmentMonths();
    long long monthlyPayment = application.getMonthlyPayment();
    string loanType = application.getLoanType();

    // Fix corrupted data with reasonable defaults
    if (loanAmount <= 1000 || loanAmount > 1000000000000) {
        loanAmount = 2000000; // 2 million PKR - reasonable default
    }
    if (downPayment < 0 || downPayment >= loanAmount) {
        downPayment = loanAmount * 0.1; // 10% down payment
    }
    if (totalMonths <= 0 || totalMonths > 120) {
        totalMonths = 36; // 3 years standard
    }
    if (monthlyPayment <= 0 || monthlyPayment > loanAmount) {
        monthlyPayment = (loanAmount - downPayment) / totalMonths; // Calculate properly
    }
    if (loanType == "0" || loanType == "1" || loanType.empty()) {
        loanType = "Personal Loan"; // Default loan type
    }

    // Get starting month from USER
    int startMonth = 0;
    while (startMonth < 1 || startMonth > 12) {
        cout << Config::CHATBOT_NAME << ": When would you like to start payments?" << endl;
        cout << "Enter starting month (1=January, 2=February, ..., 12=December): ";
        string monthInput;
        getline(cin, monthInput);

        try {
            startMonth = stoi(trim(monthInput));
            if (startMonth < 1 || startMonth > 12) {
                cout << "❌ Please enter a valid month (1-12)." << endl;
            }
        }
        catch (...) {
            cout << "❌ Please enter a number between 1 and 12." << endl;
            startMonth = 0;
        }
    }

    // Get starting year from USER
    int startYear = 0;
    while (startYear < 2024 || startYear > 2100) {
        cout << Config::CHATBOT_NAME << ": Enter starting year (e.g., 2024, 2025): ";
        string yearInput;
        getline(cin, yearInput);

        try {
            startYear = stoi(trim(yearInput));
            if (startYear < 2024 || startYear > 2100) {
                cout << "❌ Please enter a year between 2024 and 2100." << endl;
            }
        }
        catch (...) {
            cout << "❌ Please enter a valid year." << endl;
            startYear = 0;
        }
    }

    // Calculate loan details
    long long financedAmount = loanAmount - downPayment;
    long long remainingBalance = financedAmount;

    // Display the installment plan in proper tabular format
    cout << endl << "===============================================" << endl;
    cout << "         MONTHLY INSTALLMENT PLAN" << endl;
    cout << "===============================================" << endl;
    cout << "Applicant: " << application.getFullName() << endl;
    cout << "CNIC: " << application.getCnicNumber() << endl;
    cout << "Loan Type: " << loanType << endl;
    cout << "Total Loan Amount: PKR " << loanAmount << endl;
    cout << "Down Payment: PKR " << downPayment << endl;
    cout << "Amount to Finance: PKR " << financedAmount << endl;
    cout << "Installment Period: " << totalMonths << " months" << endl;
    cout << "Monthly Installment: PKR " << monthlyPayment << endl;
    cout << "Starting: " << getMonthName(startMonth) << " " << startYear << endl;
    cout << "===============================================" << endl << endl;

    cout << Config::CHATBOT_NAME << ": Here is your complete payment schedule:" << endl << endl;

    // TABLE HEADER
    cout << "+-------+------------------+------+------------------+------------------+" << endl;
    cout << "| Month | Month Name       | Year | Payment Due      | Remaining Balance|" << endl;
    cout << "+-------+------------------+------+------------------+------------------+" << endl;

    int currentMonth = startMonth;
    int currentYear = startYear;
    long long totalPaid = 0;

    // Generate each month's payment details
    for (int monthNum = 1; monthNum <= totalMonths; monthNum++) {
        long long paymentDue = monthlyPayment;

        // Adjust final payment to clear exact remaining balance
        if (monthNum == totalMonths) {
            paymentDue = remainingBalance;
        }

        totalPaid += paymentDue;
        remainingBalance -= paymentDue;
        if (remainingBalance < 0) remainingBalance = 0;

        // Display row with proper formatting
        cout << "| " << setw(5) << monthNum << " | "
            << setw(16) << left << getMonthName(currentMonth) << " | "
            << setw(4) << currentYear << " | "
            << "PKR " << setw(12) << right << paymentDue << " | "
            << "PKR " << setw(12) << remainingBalance << " |" << endl;

        // Move to next month
        getNextMonth(currentMonth, currentYear);
    }

    // TABLE FOOTER
    cout << "+-------+------------------+------+------------------+------------------+" << endl;

    // Final summary
    cout << endl << "===============================================" << endl;
    cout << "PAYMENT SUMMARY" << endl;
    cout << "===============================================" << endl;
    cout << "Total Loan Amount:    PKR " << setw(12) << loanAmount << endl;
    cout << "Down Payment:         PKR " << setw(12) << downPayment << endl;
    cout << "Amount Financed:      PKR " << setw(12) << financedAmount << endl;
    cout << "Total Installments:   PKR " << setw(12) << totalPaid << endl;
    cout << "Total Amount Paid:    PKR " << setw(12) << (downPayment + totalPaid) << endl;
    cout << "Completion Date:      " << getMonthName(currentMonth) << " " << currentYear << endl;
    cout << "===============================================" << endl;

    cout << endl << Config::CHATBOT_NAME << ": Your installment plan is ready! ";
    cout << "Your first payment of PKR " << monthlyPayment << " is due " << getMonthName(startMonth) << " " << startYear << "." << endl;
}
void checkApplicationStatusByCNIC(FileManager& fileManager) {
    bool keepChecking = true;

    while (keepChecking) {
        cout << endl << "=== CHECK APPLICATION STATUS ===" << endl;
        cout << Config::CHATBOT_NAME << ": Please enter your CNIC (13 digits without dashes) or 'X' to exit: " << endl;

        string cnic;
        getline(cin, cnic);
        cnic = trim(cnic);

        if (toLower(cnic) == "x" || toLower(cnic) == "exit") {
            cout << Config::CHATBOT_NAME << ": Returning to main menu." << endl;
            return;
        }

        // Validate CNIC format
        if (cnic.length() != 13) {
            cout << Config::CHATBOT_NAME << ": Invalid CNIC format. CNIC must be exactly 13 digits." << endl;
            cout << "Example: 1234567891234" << endl;
            cout << endl << "Would you like to try again? (Y/N): ";
            string retry;
            getline(cin, retry);
            if (toLower(trim(retry)) != "y" && toLower(trim(retry)) != "yes") {
                return;
            }
            continue;
        }

        // Check if all characters are digits
        bool allDigits = true;
        for (char c : cnic) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            cout << Config::CHATBOT_NAME << ": Invalid CNIC. Please enter only digits (no dashes or spaces)." << endl;
            cout << "Example: 1234567891234" << endl;
            cout << endl << "Would you like to try again? (Y/N): ";
            string retry;
            getline(cin, retry);
            if (toLower(trim(retry)) != "y" && toLower(trim(retry)) != "yes") {
                return;
            }
            continue;
        }

        // Get applications for this CNIC
        auto userApplications = fileManager.findApplicationsByCNIC(cnic);
        int total = userApplications.size();

        cout << endl << "========================================" << endl;
        cout << "APPLICATION STATUS FOR CNIC: " << cnic << endl;
        cout << "========================================" << endl;

        if (total == 0) {
            cout << Config::CHATBOT_NAME << ": No applications found for this CNIC." << endl;
            cout << "Would you like to:" << endl;
            cout << "  1. Try another CNIC" << endl;
            cout << "  2. Apply for a loan (Type 'A')" << endl;
            cout << "  3. Return to main menu (Type 'X')" << endl;
            cout << "Your choice: ";

            string choice;
            getline(cin, choice);
            choice = toLower(trim(choice));

            if (choice == "1" || choice == "try again" || choice == "y" || choice == "yes") {
                continue;
            }
            else {
                return;
            }
        }
        else {
            // Display all applications with their status
            int submitted = 0, approved = 0, rejected = 0;

            cout << "Your Applications:" << endl;
            cout << "+-----+--------+-----------+------------------------+---------------------+" << endl;
            cout << "| No  | App ID | Status    | Loan Type              | Submission Date     |" << endl;
            cout << "+-----+--------+-----------+------------------------+---------------------+" << endl;

            for (size_t i = 0; i < userApplications.size(); i++) {
                const auto& app = userApplications[i];
                string status = app.getStatus();

                if (status == "submitted") submitted++;
                else if (status == "approved") approved++;
                else if (status == "rejected") rejected++;

                cout << "| " << setw(3) << (i + 1) << " | "
                    << setw(6) << app.getApplicationId() << " | "
                    << setw(9) << left << status << " | "
                    << setw(22) << app.getLoanType() << " | "
                    << setw(19) << app.getSubmissionDate() << " |" << endl;
            }
            cout << "+-----+--------+-----------+------------------------+---------------------+" << endl;

            cout << endl << "Summary:" << endl;
            cout << "  Pending Review: " << submitted << " application(s)" << endl;
            cout << "  Approved: " << approved << " application(s)" << endl;
            cout << "  Rejected: " << rejected << " application(s)" << endl;
            cout << "========================================" << endl;

            // 🔥 NEW: Offer installment plan for approved applications
            if (approved > 0) {
                cout << endl << Config::CHATBOT_NAME << ": You have " << approved
                    << " approved loan application(s)." << endl;
                cout << "Would you like to generate a monthly installment plan? (Y/N): ";

                string planChoice;
                getline(cin, planChoice);
                planChoice = toLower(trim(planChoice));

                if (planChoice == "y" || planChoice == "yes") {
                    // Show only approved applications
                    vector<LoanApplication> approvedApps;
                    for (const auto& app : userApplications) {
                        if (app.getStatus() == "approved") {
                            approvedApps.push_back(app);
                        }
                    }

                    if (approvedApps.size() == 1) {
                        // Only one approved application - generate plan directly
                        generateUserInstallmentPlan(approvedApps[0]);
                    }
                    else if (approvedApps.size() > 1) {
                        // Multiple approved applications - let user choose
                        cout << endl << "Which approved application would you like to generate a plan for?" << endl;
                        cout << "+-----+--------+------------------------+---------------------+" << endl;
                        cout << "| No  | App ID | Loan Type              | Loan Amount         |" << endl;
                        cout << "+-----+--------+------------------------+---------------------+" << endl;

                        for (size_t i = 0; i < approvedApps.size(); i++) {
                            const auto& app = approvedApps[i];
                            cout << "| " << setw(3) << (i + 1) << " | "
                                << setw(6) << app.getApplicationId() << " | "
                                << setw(22) << app.getLoanType() << " | "
                                << "PKR " << setw(13) << app.getLoanAmount() << " |" << endl;
                        }
                        cout << "+-----+--------+------------------------+---------------------+" << endl;

                        cout << "Enter application number: ";
                        string appChoice;
                        getline(cin, appChoice);

                        try {
                            int selectedApp = stoi(trim(appChoice));
                            if (selectedApp >= 1 && selectedApp <= static_cast<int>(approvedApps.size())) {
                                generateUserInstallmentPlan(approvedApps[selectedApp - 1]);
                            }
                            else {
                                cout << "Invalid application number." << endl;
                            }
                        }
                        catch (...) {
                            cout << "Invalid input." << endl;
                        }
                    }
                }
            }

            // Ask if they want to check another CNIC
            cout << endl << "Would you like to check another CNIC? (Y/N): ";
            string another;
            getline(cin, another);
            if (toLower(trim(another)) == "y" || toLower(trim(another)) == "yes") {
                continue;
            }
            else {
                return;
            }
        }
    }
}void viewAllApplications(FileManager& fileManager) {
    auto allApplications = fileManager.loadAllApplications();

    if (allApplications.empty()) {
        cout << endl << "No applications found in the system." << endl;
        return;
    }

    cout << endl << "========== ALL APPLICATIONS ==========" << endl;
    cout << "Total: " << allApplications.size() << " application(s)" << endl;
    cout << "======================================" << endl << endl;

    cout << "+-----+--------+-----------+------------------------+------------+--------------+" << endl;
    cout << "| No  | App ID | Status    | Applicant Name         | Date       | Income (PKR) |" << endl;
    cout << "+-----+--------+-----------+------------------------+------------+--------------+" << endl;

    for (size_t i = 0; i < allApplications.size(); i++) {
        const auto& app = allApplications[i];
        cout << "| " << setw(3) << (i + 1) << " | "
            << setw(6) << app.getApplicationId() << " | "
            << setw(9) << left << app.getStatus() << " | "
            << setw(22) << app.getFullName() << " | "
            << setw(10) << app.getSubmissionDate() << " | "
            << setw(12) << right << app.getAnnualIncome() << " |" << endl;
    }

    cout << "+-----+--------+-----------+------------------------+------------+--------------+" << endl;

    // Option to view details
    cout << endl << "Enter application number to view details (or press Enter to return): ";
    string input;
    getline(cin, input);
    input = trim(input);

    if (!input.empty()) {
        try {
            int appNum = stoi(input);
            if (appNum >= 1 && appNum <= static_cast<int>(allApplications.size())) {
                displayApplicationDetails(allApplications[appNum - 1]);
            }
            else {
                cout << "Invalid application number." << endl;
            }
        }
        catch (...) {
            cout << "Invalid input." << endl;
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