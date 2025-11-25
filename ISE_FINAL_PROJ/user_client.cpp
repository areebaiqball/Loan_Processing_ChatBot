#pragma once
#include <iostream>
#include <string>
#include<iomanip>
#include "utilities.h"
#include "home.h"
#include "car.h"
#include "scooter.h"
#include "application.h"
#include "application_collector.h"
#include "file_manager.h"
#include "utterances.h"
#include "ui_handler.h"

using namespace std;
// SE Principles: Namespace organization, Configuration management



// Forward declarations
void handleUserMode(const HomeLoan[], int, const CarLoan[], int,
    const ScooterLoan[], int, const Utterance[], int,
    ApplicationCollector&, FileManager&);
void handleHomeLoanSelection(const HomeLoan[], int, bool&, ApplicationCollector&, FileManager&);
void handleCarLoanSelection(const CarLoan[], int, bool&, ApplicationCollector&, FileManager&);
void handleScooterLoanSelection(const ScooterLoan[], int, bool&, ApplicationCollector&, FileManager&);
void handlePersonalLoanSelection(bool&, ApplicationCollector&, FileManager&);
void checkApplicationStatusByCNIC(FileManager&);
void displayLoanCategories();

void displayLoanCategories() {
    cout << Config::CHATBOT_NAME << ": Please select loan category:" << endl;
    cout << "  H - Home Loan" << endl;
    cout << "  C - Car Loan" << endl;
    cout << "  S - Scooter Loan" << endl;
    cout << "  P - Personal Loan" << endl;
    cout << "  X - Exit" << endl;
}

void handleUserMode(const HomeLoan homeLoans[], int homeLoanCount,
    const CarLoan carLoans[], int carLoanCount,
    const ScooterLoan scooterLoans[], int scooterLoanCount,
    const Utterance utterances[], int utteranceCount,
    ApplicationCollector& collector, FileManager& fileManager) {

    bool running = true;
    string userInput;

    cout << endl << Config::CHATBOT_NAME << ": Hello! I'm your loan assistant." << endl;
    cout << "Options:" << endl;
    cout << "  - Type 'A' to apply for a loan" << endl;
    cout << "  - Type 'check my applications' to view status" << endl;
    cout << "  - Type 'X' to exit" << endl;

    while (running) {
        cout << endl << "You: ";
        getline(cin, userInput);
        string lowerInput = toLower(trim(userInput));

        if (lowerInput == "x" || lowerInput == "exit") {
            cout << Config::CHATBOT_NAME << ": Goodbye!" << endl;
            break;
        }

        if (lowerInput.find("check") != string::npos &&
            lowerInput.find("application") != string::npos) {
            checkApplicationStatusByCNIC(fileManager);
            continue;
        }

        if (lowerInput == "a" || lowerInput == "apply") {
            displayLoanCategories();
            cout << "You: ";
            getline(cin, userInput);
            lowerInput = toLower(trim(userInput));

            if (lowerInput == "x") break;

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
                LoanApplication application = collector.collectApplicationForLoan("home", "Home Loan");

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
            LoanApplication application = collector.collectApplicationForLoan("car", "Car Loan");

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
            LoanApplication application = collector.collectApplicationForLoan("scooter", "Scooter Loan");

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
            LoanApplication application = collector.collectApplicationForLoan("personal", "Personal Loan");

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





int main() {
    // Initialize data
    Utterance utterances[Config::MAX_UTTERANCES];
    HomeLoan homeLoans[Config::MAX_LOANS];
    CarLoan carLoans[Config::MAX_LOANS];
    ScooterLoan scooterLoans[Config::MAX_LOANS];
    FileManager fileManager;
    ApplicationCollector collector;

    int utteranceCount = loadUtterances(utterances, Config::MAX_UTTERANCES, Config::UTTERANCES_FILE);
    int homeLoanCount = loadHomeLoans(homeLoans, Config::MAX_LOANS, Config::HOME_LOANS_FILE);
    int carLoanCount = loadCarLoans(carLoans, Config::MAX_LOANS, Config::CAR_LOANS_FILE);
    int scooterLoanCount = loadScooterLoans(scooterLoans, Config::MAX_LOANS, Config::SCOOTER_LOANS_FILE);

    cout << "╔════════════════════════════════════╗" << endl;
    cout << "║   LOAN PROCESSING SYSTEM           ║" << endl;
    cout << "║   USER CLIENT                      ║" << endl;
    cout << "╚════════════════════════════════════╝" << endl;

    handleUserMode(homeLoans, homeLoanCount, carLoans, carLoanCount,
        scooterLoans, scooterLoanCount, utterances, utteranceCount,
        collector, fileManager);

    return 0;
}