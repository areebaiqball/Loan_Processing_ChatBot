#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "utterances.h"
#include "home.h"
#include "car.h"
#include "scooter.h"
#include "utilities.h"
#include "application.h"
#include "application_collector.h"
#include "file_manager.h"

using namespace std;

// SE Principles: Namespace organization, Configuration management
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

// Forward declarations
void handleLenderMode(FileManager& fileManager);
void handleUserMode(const HomeLoan[], int, const CarLoan[], int,
    const ScooterLoan[], int, const Utterance[], int,
    ApplicationCollector&, FileManager&);

void checkApplicationStatusByCNIC(FileManager& fileManager);
void displayApplicationStatistics(FileManager& fileManager);
void searchApplicationById(FileManager& fileManager);
void viewAllApplications(FileManager& fileManager);

// Input Validation Helpers
bool isValidAreaInput(const string& input) {
    return (input == "1" || input == "2" || input == "3" || input == "4");
}

bool isValidLoanType(const string& input) {
    return (input == "h" || input == "c" || input == "s" || input == "p");
}

bool isValidMakeInput(const string& input, const string& loanType) {
    if (loanType == "car") return (input == "1" || input == "2");
    if (loanType == "scooter") return (input == "1");
    return false;
}

// UI Display Functions
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

    if (app.getStatus() == "rejected" && !app.getRejectionReason().empty()) {
        cout << "Rejection Reason: " << app.getRejectionReason() << endl;
    }

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
    cout << "  Avg Electricity Bill: PKR " << app.getAvgElectricityBill() << endl;
    cout << "  Current Electricity Bill: PKR " << app.getCurrentElectricityBill() << endl;

    cout << endl << "LOAN DETAILS:" << endl;

    string loanType = app.getLoanType();
    if (loanType.empty() || loanType == "0" || loanType == "Unknown") {
        loanType = "Personal Loan";
    }
    cout << "  Loan Type: " << loanType << endl;

    cout << "  Category: " << app.getLoanCategory() << endl;
    cout << "  Total Amount: PKR " << app.getLoanAmount() << endl;
    cout << "  Down Payment: PKR " << app.getDownPayment() << endl;
    cout << "  Installment Period: " << app.getInstallmentMonths() << " months" << endl;
    cout << "  Monthly Payment: PKR " << app.getMonthlyPayment() << endl;

    cout << endl << "EXISTING LOANS: " << app.getExistingLoansCount() << " loan(s)" << endl;
    cout << "========================================" << endl;
}

// SE Principles: Separation of Concerns, Single Responsibility
void viewApplicationsByStatus(FileManager& fileManager, const string& status) {
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

    cout << endl << Config::CHATBOT_NAME << ": Enter application number to review (or X to cancel): ";
    string input;
    getline(cin, input);
    input = trim(input);

    if (toLower(input) == "x") {
        return;
    }

    try {
        int appNum = stoi(input);
        if (appNum < 1 || appNum > static_cast<int>(pendingApps.size())) {
            cout << Config::CHATBOT_NAME << ": Invalid application number." << endl;
            return;
        }

        LoanApplication selectedApp = pendingApps[appNum - 1];

        // Load full application details
        LoanApplication fullApp = fileManager.findApplicationById(selectedApp.getApplicationId());
        if (fullApp.getApplicationId().empty()) {
            cout << Config::CHATBOT_NAME << ": Error: Could not load application details." << endl;
            return;
        }

        displayApplicationDetails(fullApp);

        cout << endl << Config::CHATBOT_NAME << ": Action: (A)pprove or (R)eject this application? ";
        getline(cin, input);
        input = toLower(trim(input));

        string newStatus;
        string rejectionReason = "";

        if (input == "a" || input == "approve") {
            newStatus = "approved";
            cout << " Application " << fullApp.getApplicationId() << " has been APPROVED." << endl;
            cout << Config::CHATBOT_NAME << ": The applicant can now generate their installment plan when checking their application status." << endl;
        }
        else if (input == "r" || input == "reject") {
            newStatus = "rejected";
            cout << " Application " << fullApp.getApplicationId() << " will be REJECTED." << endl;

            cout << Config::CHATBOT_NAME << ": Enter reason for rejection: ";
            getline(cin, rejectionReason);
            rejectionReason = trim(rejectionReason);

            if (rejectionReason.empty()) {
                cout << Config::CHATBOT_NAME << ": No rejection reason provided. Application will be rejected without specific reason." << endl;
            }
        }
        else {
            cout << Config::CHATBOT_NAME << ": Invalid action. No changes made." << endl;
            return;
        }

        // Ask for confirmation
        cout << Config::CHATBOT_NAME << ": Confirm this action? (Y/N): ";
        getline(cin, input);
        input = toLower(trim(input));

        if (input == "y" || input == "yes") {
            if (fileManager.updateApplicationStatus(fullApp.getApplicationId(), newStatus, rejectionReason)) {
                cout << " Application status updated successfully!" << endl;
            }
            else {
                cout << " Failed to update application status." << endl;
            }
        }
        else {
            cout << Config::CHATBOT_NAME << ": Action cancelled." << endl;
        }

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Error: " << e.what() << endl;
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

// SE Principles: Strategy Pattern, Composition, Separation of Concerns
void handleHomeLoanSelection(const HomeLoan loans[], int loanCount, bool& running,
    ApplicationCollector& collector, FileManager& fileManager) {

    cout << Config::CHATBOT_NAME << ": You selected home loan. Please select area (1-4):" << endl;

    bool validArea = false;
    string userInput;

    while (!validArea && running) {
        cout << "You: ";
        getline(cin, userInput);
        string areaInput = trim(userInput);

        if (toLower(areaInput) == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            running = false;
            return;
        }

        if (!isValidAreaInput(areaInput)) {
            cout << Config::CHATBOT_NAME << ": Invalid area. Please enter 1, 2, 3, or 4:" << endl;
            continue;
        }

        bool hasData = displayHomeLoanOptions(loans, loanCount, areaInput);

        if (!hasData) continue;

        cout << endl << Config::CHATBOT_NAME << ": Would you like to see the detailed installment plan? (Y/N): ";
        getline(cin, userInput);
        string planInput = toLower(trim(userInput));

        if (planInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            running = false;
            return;
        }

        int optionNumber = 0;

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
                displayInstallmentPlanForOption(loans, loanCount, areaInput, optionNumber);
            }
            catch (const exception&) {
                cout << Config::CHATBOT_NAME << ": Invalid option number." << endl;
                continue;
            }
        }

        cout << endl << Config::CHATBOT_NAME << ": Would you like to start the loan application? (Y/N): ";
        getline(cin, userInput);
        string applyInput = toLower(trim(userInput));

        if (applyInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            running = false;
            return;
        }

        if (applyInput == "y" || applyInput == "yes") {
            if (optionNumber == 0) {
                cout << Config::CHATBOT_NAME << ": Which option would you like to apply for? Enter number: ";
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
                LoanApplication application = collector.collectCompleteApplication();

                application.setLoanType("Home Loan");
                application.setLoanCategory("Area " + areaInput + " - Option " + to_string(optionNumber));

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

                cout << endl << Config::CHATBOT_NAME << ": Please review your application:" << endl;
                displayApplicationDetails(application);

                cout << endl << Config::CHATBOT_NAME << ": Submit this application? (Y/N): ";
                getline(cin, userInput);
                string confirmInput = toLower(trim(userInput));

                if (confirmInput == "y" || confirmInput == "yes") {
                    if (fileManager.saveApplication(application)) {
                        cout << endl << Config::CHATBOT_NAME << ": Application submitted successfully!" << endl;
                        cout << "Your Application ID: " << application.getApplicationId() << endl;
                        cout << endl << "----------------------------------------" << endl;
                        cout << "Check status by: Saying 'check my applications'" << endl;
                        cout << "CNIC: " << application.getCnicNumber() << endl;
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
        cout << endl << "Press X to exit or any key for main menu: ";
        getline(cin, userInput);
        if (toLower(trim(userInput)) == Config::EXIT_COMMAND) {
            running = false;
        }
        return;
    }
}

// SE Principles: Template Method Pattern, Code Reusability
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

    if (!isValidMakeInput(makeInput, "car")) {
        cout << Config::CHATBOT_NAME << ": Invalid make selection." << endl;
        return;
    }

    bool hasData = displayCarLoanOptionsByMake(loans, loanCount, makeInput);
    if (!hasData) return;

    int optionNumber = 0;

    cout << endl << Config::CHATBOT_NAME << ": View detailed installment plan? (Y/N): ";
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

    cout << endl << Config::CHATBOT_NAME << ": Start the loan application? (Y/N): ";
    getline(cin, userInput);
    string applyInput = toLower(trim(userInput));

    if (applyInput == "y" || applyInput == "yes") {
        if (optionNumber == 0) {
            cout << Config::CHATBOT_NAME << ": Which option to apply for? Enter number: ";
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

            application.setLoanType("Car Loan");
            application.setLoanCategory("Make " + makeInput + " - Option " + to_string(optionNumber));

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
                    cout << endl << Config::CHATBOT_NAME << ": Application submitted successfully!" << endl;
                    cout << "Your Application ID: " << application.getApplicationId() << endl;
                    cout << endl << "----------------------------------------" << endl;
                    cout << "Check status: Type 'check my applications'" << endl;
                    cout << "CNIC: " << application.getCnicNumber() << endl;
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

    cout << endl << "Returning to main menu..." << endl;
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

    if (!isValidMakeInput(makeInput, "scooter")) {
        cout << Config::CHATBOT_NAME << ": Invalid make selection." << endl;
        return;
    }

    bool hasData = displayScooterLoanOptionsTable(loans, loanCount, makeInput);
    if (!hasData) return;

    int optionNumber = 0;

    cout << endl << Config::CHATBOT_NAME << ": View detailed installment plan? (Y/N): ";
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

    cout << endl << Config::CHATBOT_NAME << ": Start the loan application? (Y/N): ";
    getline(cin, userInput);

    if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
        if (optionNumber == 0) {
            cout << Config::CHATBOT_NAME << ": Which option to apply for? Enter number: ";
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

            application.setLoanType("Scooter Loan");
            application.setLoanCategory("Make " + makeInput + " - Option " + to_string(optionNumber));

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
                    cout << endl << Config::CHATBOT_NAME << ": Application submitted successfully!" << endl;
                    cout << "Application ID: " << application.getApplicationId() << endl;
                    cout << endl << "----------------------------------------" << endl;
                    cout << "Check status: Type 'check my applications'" << endl;
                    cout << "CNIC: " << application.getCnicNumber() << endl;
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
}
// SE Principles: Encapsulation, Data Validation
void handlePersonalLoanSelection(bool& running, ApplicationCollector& collector, FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": Personal loan selected." << endl;
    cout << "Personal loans available with flexible terms." << endl;

    cout << endl << Config::CHATBOT_NAME << ": Start application? (Y/N): ";
    string userInput;
    getline(cin, userInput);

    if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
        try {
            LoanApplication application = collector.collectApplicationForLoan("personal", "Personal Loan", fileManager);

            application.setLoanType("Personal Loan");
            application.setLoanCategory("Standard Personal Loan");
            application.setLoanAmount(500000);
            application.setDownPayment(50000);
            application.setInstallmentMonths(36);
            application.setMonthlyPayment(12500);

            displayApplicationDetails(application);

            cout << endl << Config::CHATBOT_NAME << ": Submit? (Y/N): ";
            getline(cin, userInput);

            if (toLower(trim(userInput)) == "y" || toLower(trim(userInput)) == "yes") {
                if (fileManager.saveApplication(application)) {
                    cout << Config::CHATBOT_NAME << ": Application submitted successfully!" << endl;
                    cout << "Application ID: " << application.getApplicationId() << endl;
                    cout << endl << "----------------------------------------" << endl;
                    cout << "Check status: Type 'check my applications'" << endl;
                    cout << "CNIC: " << application.getCnicNumber() << endl;
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
    cout << endl << "Options:" << endl;
    cout << "  - Type 'A' to apply for a new loan" << endl;
    cout << "  - Type 'check my applications' to view your status" << endl;
    cout << "  - Type 'X' to exit" << endl;

    while (running) {
        cout << endl << "You: ";
        getline(cin, userInput);
        string lowerInput = toLower(trim(userInput));

        if (lowerInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            break;
        }

        if (lowerInput == "check my applications" ||
            lowerInput == "application status" ||
            lowerInput == "check status" ||
            lowerInput == "my application count" ||
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
                displayLoanCategories();
                continue;
            }
        }
        else {
            cout << Config::CHATBOT_NAME << ": " << getResponse(utterances, utteranceCount, userInput) << endl;
        }
    }
}

// SE Principles: Information Expert Pattern, Data Display
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

void viewAllApplications(FileManager& fileManager) {
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

// SE Principles: Data Validation, Defensive Programming, User Experience
void generateUserInstallmentPlan(const LoanApplication& application) {
    cout << endl << "💫 " << Config::CHATBOT_NAME
        << ": Generating your monthly installment plan..." << endl;

    // Data sanitization for corrupted values
    long long loanAmount = application.getLoanAmount();
    long long downPayment = application.getDownPayment();
    int totalMonths = application.getInstallmentMonths();
    long long monthlyPayment = application.getMonthlyPayment();
    string loanType = application.getLoanType();

    // Apply default values for invalid data
    if (loanAmount <= 1000 || loanAmount > 1000000000000) {
        loanAmount = 2000000;
    }
    if (downPayment < 0 || downPayment >= loanAmount) {
        downPayment = loanAmount * 0.1;
    }
    if (totalMonths <= 0 || totalMonths > 120) {
        totalMonths = 36;
    }
    if (monthlyPayment <= 0 || monthlyPayment > loanAmount) {
        monthlyPayment = (loanAmount - downPayment) / totalMonths;
    }
    if (loanType == "0" || loanType == "1" || loanType.empty()) {
        loanType = "Personal Loan";
    }

    // Get payment start date from user
    int startMonth = 0;
    while (startMonth < 1 || startMonth > 12) {
        cout << Config::CHATBOT_NAME << ": When would you like to start payments?" << endl;
        cout << "Enter starting month (1=January, ..., 12=December): ";
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

    long long financedAmount = loanAmount - downPayment;
    long long remainingBalance = financedAmount;

    // Display installment plan
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

    cout << "+-------+------------------+------+------------------+------------------+" << endl;
    cout << "| Month | Month Name       | Year | Payment Due      | Remaining Balance|" << endl;
    cout << "+-------+------------------+------+------------------+------------------+" << endl;

    int currentMonth = startMonth;
    int currentYear = startYear;
    long long totalPaid = 0;

    for (int monthNum = 1; monthNum <= totalMonths; monthNum++) {
        long long paymentDue = monthlyPayment;

        if (monthNum == totalMonths) {
            paymentDue = remainingBalance;
        }

        totalPaid += paymentDue;
        remainingBalance -= paymentDue;
        if (remainingBalance < 0) remainingBalance = 0;

        cout << "| " << setw(5) << monthNum << " | "
            << setw(16) << left << getMonthName(currentMonth) << " | "
            << setw(4) << currentYear << " | "
            << "PKR " << setw(12) << right << paymentDue << " | "
            << "PKR " << setw(12) << remainingBalance << " |" << endl;

        getNextMonth(currentMonth, currentYear);
    }

    cout << "+-------+------------------+------+------------------+------------------+" << endl;

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
    cout << "First payment of PKR " << monthlyPayment << " is due "
        << getMonthName(startMonth) << " " << startYear << "." << endl;
}

// SE Principles: Data filtering, Search operations
void checkApplicationStatusByCNIC(FileManager& fileManager) {
    bool keepChecking = true;

    while (keepChecking) {
        cout << endl << "=== CHECK APPLICATION STATUS ===" << endl;
        cout << Config::CHATBOT_NAME << ": Enter your CNIC (13 digits without dashes) or 'X' to exit:" << endl;

        string cnic;
        getline(cin, cnic);
        cnic = trim(cnic);

        if (toLower(cnic) == "x" || toLower(cnic) == "exit") {
            cout << Config::CHATBOT_NAME << ": Returning to main menu." << endl;
            return;
        }

        // Validate CNIC format
        if (cnic.length() != 13) {
            cout << Config::CHATBOT_NAME << ": Invalid CNIC format. Must be exactly 13 digits." << endl;
            cout << "Example: 1234567891234" << endl;
            cout << endl << Config::CHATBOT_NAME << ": Try again? (Y/N): ";
            string retry;
            getline(cin, retry);
            if (toLower(trim(retry)) != "y" && toLower(trim(retry)) != "yes") {
                return;
            }
            continue;
        }

        // Validate all digits
        bool allDigits = true;
        for (char c : cnic) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            cout << Config::CHATBOT_NAME << ": Invalid CNIC. Enter only digits (no dashes or spaces)." << endl;
            cout << "Example: 1234567891234" << endl;
            cout << endl << Config::CHATBOT_NAME << ": Try again? (Y/N): ";
            string retry;
            getline(cin, retry);
            if (toLower(trim(retry)) != "y" && toLower(trim(retry)) != "yes") {
                return;
            }
            continue;
        }

        auto userApplications = fileManager.findApplicationsByCNIC(cnic);
        int total = userApplications.size();

        cout << endl << "========================================" << endl;
        cout << "APPLICATION STATUS FOR CNIC: " << cnic << endl;
        cout << "========================================" << endl;

        if (total == 0) {
            cout << Config::CHATBOT_NAME << ": No applications found for this CNIC." << endl;
            cout << Config::CHATBOT_NAME << ": Would you like to:" << endl;
            cout << "  1. Try another CNIC" << endl;
            cout << "  2. Return to main menu (Type 'X')" << endl;
            cout << Config::CHATBOT_NAME << ": Your choice: ";

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
            int submitted = 0, approved = 0, rejected = 0;

            cout << "Your Applications:" << endl;
            cout << "+-----+--------+-----------+------------------------+---------------------+------------------+" << endl;
            cout << "| No  | App ID | Status    | Loan Type              | Submission Date     | Rejection Reason |" << endl;
            cout << "+-----+--------+-----------+------------------------+---------------------+------------------+" << endl;

            for (size_t i = 0; i < userApplications.size(); i++) {
                const auto& app = userApplications[i];
                string status = app.getStatus();

                if (status == "submitted") submitted++;
                else if (status == "approved") approved++;
                else if (status == "rejected") rejected++;

                string displayLoanType = app.getLoanType();
                if (displayLoanType.empty() || displayLoanType == "0" || displayLoanType == "Unknown") {
                    displayLoanType = "Personal Loan";
                }

                string rejectionReason = (status == "rejected" && !app.getRejectionReason().empty())
                    ? app.getRejectionReason()
                    : "N/A";

                cout << "| " << setw(3) << (i + 1) << " | "
                    << setw(6) << app.getApplicationId() << " | "
                    << setw(9) << left << status << " | "
                    << setw(22) << displayLoanType << " | "
                    << setw(19) << app.getSubmissionDate() << " | "
                    << setw(16) << rejectionReason << " |" << endl;
            }

            cout << "+-----+--------+-----------+------------------------+---------------------+------------------+" << endl;

            cout << endl << "Summary:" << endl;
            cout << "  Pending Review: " << submitted << " application(s)" << endl;
            cout << "  Approved: " << approved << " application(s)" << endl;
            cout << "  Rejected: " << rejected << " application(s)" << endl;
            cout << "========================================" << endl;

            // Generate installment plan for approved applications
            if (approved > 0) {
                cout << endl << Config::CHATBOT_NAME << ": You have " << approved
                    << " approved loan application(s)." << endl;
                cout << Config::CHATBOT_NAME << ": Generate monthly installment plan? (Y/N): ";

                string planChoice;
                getline(cin, planChoice);
                planChoice = toLower(trim(planChoice));

                if (planChoice == "y" || planChoice == "yes") {
                    vector<LoanApplication> approvedApps;
                    for (const auto& app : userApplications) {
                        if (app.getStatus() == "approved") {
                            approvedApps.push_back(app);
                        }
                    }

                    if (approvedApps.size() == 1) {
                        generateUserInstallmentPlan(approvedApps[0]);
                    }
                    else if (approvedApps.size() > 1) {
                        cout << endl << Config::CHATBOT_NAME << ": Select an approved application:" << endl;
                        cout << "+-----+--------+------------------------+---------------------+" << endl;
                        cout << "| No  | App ID | Loan Type              | Loan Amount         |" << endl;
                        cout << "+-----+--------+------------------------+---------------------+" << endl;

                        for (size_t i = 0; i < approvedApps.size(); i++) {
                            const auto& app = approvedApps[i];
                            string displayLoanType = app.getLoanType();
                            if (displayLoanType.empty() || displayLoanType == "0" || displayLoanType == "Unknown") {
                                displayLoanType = "Personal Loan";
                            }

                            cout << "| " << setw(3) << (i + 1) << " | "
                                << setw(6) << app.getApplicationId() << " | "
                                << setw(22) << displayLoanType << " | "
                                << "PKR " << setw(13) << app.getLoanAmount() << " |" << endl;
                        }
                        cout << "+-----+--------+------------------------+---------------------+" << endl;

                        cout << Config::CHATBOT_NAME << ": Enter application number: ";
                        string appChoice;
                        getline(cin, appChoice);

                        try {
                            int selectedApp = stoi(trim(appChoice));
                            if (selectedApp >= 1 && selectedApp <= static_cast<int>(approvedApps.size())) {
                                generateUserInstallmentPlan(approvedApps[selectedApp - 1]);
                            }
                            else {
                                cout << Config::CHATBOT_NAME << ": Invalid application number." << endl;
                            }
                        }
                        catch (...) {
                            cout << Config::CHATBOT_NAME << ": Invalid input." << endl;
                        }
                    }
                }
            }

            cout << endl << Config::CHATBOT_NAME << ": Check another CNIC? (Y/N): ";
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
}
// SE Principles: Dependency Injection, Initialization, Error Handling
int main() {
    // Initialize data structures
    Utterance utterances[Config::MAX_UTTERANCES];
    HomeLoan homeLoans[Config::MAX_LOANS];
    CarLoan carLoans[Config::MAX_LOANS];
    ScooterLoan scooterLoans[Config::MAX_LOANS];
    FileManager fileManager;
    ApplicationCollector collector;

    // Load application data
    int utteranceCount = loadUtterances(utterances, Config::MAX_UTTERANCES, Config::UTTERANCES_FILE);
    int homeLoanCount = loadHomeLoans(homeLoans, Config::MAX_LOANS, Config::HOME_LOANS_FILE);
    int carLoanCount = loadCarLoans(carLoans, Config::MAX_LOANS, Config::CAR_LOANS_FILE);
    int scooterLoanCount = loadScooterLoans(scooterLoans, Config::MAX_LOANS, Config::SCOOTER_LOANS_FILE);

    // Validate critical data
    if (utteranceCount == 0) {
        cerr << "Failed to load utterances. Exiting." << endl;
        return 1;
    }

    displayWelcomeMessage();

    // Main application loop
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

        // Route to appropriate mode
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

        // Prompt for continuation
        if (systemRunning) {
            cout << endl << "Return to role selection? (Y/N): ";
            getline(cin, roleInput);
            if (toLower(trim(roleInput)) != "y" && toLower(trim(roleInput)) != "yes") {
                systemRunning = false;
                displayGoodbyeMessage();
            }
        }
    }

    return 0;
}