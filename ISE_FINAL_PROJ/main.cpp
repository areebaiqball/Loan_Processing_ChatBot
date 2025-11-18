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
    cout << "   Loan Processing Chatbot - " << Config::CHATBOT_NAME << endl << endl;
    cout << Config::CHATBOT_NAME << ": Hello! I'm " << Config::CHATBOT_NAME
        << ", your friendly loan assistant." << endl <<
        "I'm here to help you find the perfect loan for your needs." << endl <<
        "Let's get started on your journey to financial success!" << endl;
}

void displayLoanCategories() {
    cout << Config::CHATBOT_NAME << ": Please select the category you want to apply for." << endl;
    cout << "  Press H for a home loan" << endl;
    cout << "  Press C for a car loan" << endl;
    cout << "  Press S for a scooter loan" << endl;
    cout << "  Press P for a personal loan" << endl;
    cout << "  Press X to exit" << endl;
}

void displayUnavailableLoanMessage() {
    cout << Config::CHATBOT_NAME << ": Sorry, no data available for this loan type at the moment." << endl;
    displayLoanCategories();
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
void handleHomeLoanSelection(const HomeLoan loans[], int loanCount,
    const Utterance utterances[], int utteranceCount, bool& running) {
    cout << Config::CHATBOT_NAME << ": " << getResponse(utterances, utteranceCount, "h") << endl;

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
                    << "Would you like to see the detailed installment plan? Press Y for Yes, N for No" << endl;

                bool waitingForPlanResponse = true;
                while (waitingForPlanResponse && running) {
                    cout << "You: ";
                    getline(cin, userInput);

                    string planInput = toLower(trim(userInput));

                    if (planInput == Config::EXIT_COMMAND) {
                        displayGoodbyeMessage();
                        running = false;
                        return;
                    }
                    else if (planInput == "y" || planInput == "yes") {
                        cout << Config::CHATBOT_NAME << ": Please enter the option number (1, 2, 3, etc.) to see detailed installment plan:" << endl;

                        bool waitingForOption = true;
                        while (waitingForOption && running) {
                            cout << "You: ";
                            getline(cin, userInput);

                            string optionInput = trim(userInput);
                            string lowerOptionInput = toLower(optionInput);

                            if (lowerOptionInput == Config::EXIT_COMMAND) {
                                displayGoodbyeMessage();
                                running = false;
                                return;
                            }

                            try {
                                int optionNumber = stoi(optionInput);
                                if (optionNumber > 0) {
                                    bool planDisplayed = displayInstallmentPlanForOption(loans, loanCount, areaInput, optionNumber);
                                    if (planDisplayed) {
                                        waitingForOption = false;
                                        waitingForPlanResponse = false;
                                        validArea = true;
                                    }
                                }
                                else {
                                    cout << Config::CHATBOT_NAME << ": Please enter a valid positive option number:" << endl;
                                }
                            }
                            catch (const exception&) {
                                cout << Config::CHATBOT_NAME << ": Please enter a valid option number:" << endl;
                            }
                        }
                    }
                    else if (planInput == "n" || planInput == "no") {
                        waitingForPlanResponse = false;
                        validArea = true;
                    }
                    else {
                        cout << Config::CHATBOT_NAME << ": Please enter Y for Yes or N for No:" << endl;
                    }
                }

                if (validArea) {
                    cout << endl << "Press X to exit or any other key to return to main menu." << endl;
                    return;
                }
            }
            else {
                cout << Config::CHATBOT_NAME << ": Please select another area (1, 2, 3, or 4) or press X to exit:" << endl;
            }
        }
        else {
            cout << Config::CHATBOT_NAME << ": Invalid area selection. Please enter 1, 2, 3, or 4:" << endl;
        }
    }
}
/// <summary>
/// Displays car loan options for a specific make
/// </summary>
bool displayCarLoanOptionsByMake(const CarLoan loans[], int size, const string& makeNumber) {
    string makeName = "Make " + makeNumber;
    bool found = false;
    int optionCount = 0;

    cout << endl << "  Car Loan Options for " << makeName << endl << endl;

    for (int i = 0; i < size; i++) {
        if (loans[i].getMake() == makeName) {
            found = true;
            optionCount++;
            long long monthlyInstallment = loans[i].calculateMonthlyInstallment();
            cout << "Option " << optionCount << ":" << endl;
            cout << "  Model: " << loans[i].getModel() << endl;
            cout << "  Engine: " << loans[i].getEngine() << "cc" << endl;
            cout << "  Condition: " << loans[i].getUsed() << endl;
            cout << "  Year: " << loans[i].getManufacturingYear() << endl;
            cout << "  Installments: " << loans[i].getInstallments() << " months" << endl;
            cout << "  Total Price: PKR " << loans[i].getPrice() << endl;
            cout << "  Down Payment: PKR " << loans[i].getDownPayment() << endl;
            cout << "  Monthly Installment: PKR " << monthlyInstallment << endl;
            cout << "----------------------------------------" << endl;
        }
    }

    if (!found) {
        cout << "Sorry, no car options available for this make at the moment." << endl;
    }

    return found;
}

/// <summary>
/// Handles car loan selection workflow with installment plan option
/// </summary>
void handleCarLoanSelection(const CarLoan loans[], int loanCount,
    const Utterance utterances[], int utteranceCount, bool& running,
    ApplicationCollector& collector, FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": " << "You selected car loan. Please select the car make." << endl;
    displayCarMakes();

    bool validMake = false;
    string userInput;

    while (!validMake && running) {
        cout << "You: ";
        getline(cin, userInput);

        string makeInput = trim(userInput);
        string lowerMakeInput = toLower(makeInput);

        if (lowerMakeInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            running = false;
            return;
        }

        if (isValidMakeInput(makeInput, "car")) {
            bool hasData = displayCarLoanOptionsByMake(loans, loanCount, makeInput);

            if (hasData) {
                cout << endl << Config::CHATBOT_NAME << ": "
                    << "Would you like to see the detailed installment plan? Press Y for Yes, or would you like to start the application process? Press A for Application" << endl;

                bool waitingForResponse = true;
                while (waitingForResponse && running) {
                    cout << "You: ";
                    getline(cin, userInput);

                    string responseInput = toLower(trim(userInput));

                    if (responseInput == Config::EXIT_COMMAND) {
                        displayGoodbyeMessage();
                        running = false;
                        return;
                    }
                    else if (responseInput == "y" || responseInput == "yes") {
                        cout << Config::CHATBOT_NAME << ": Please enter the option number (1, 2, 3, etc.) to see detailed installment plan:" << endl;

                        bool waitingForOption = true;
                        while (waitingForOption && running) {
                            cout << "You: ";
                            getline(cin, userInput);

                            string optionInput = trim(userInput);
                            string lowerOptionInput = toLower(optionInput);

                            if (lowerOptionInput == Config::EXIT_COMMAND) {
                                displayGoodbyeMessage();
                                running = false;
                                return;
                            }

                            try {
                                int optionNumber = stoi(optionInput);
                                if (optionNumber > 0) {
                                    bool planDisplayed = displayCarInstallmentPlan(loans, loanCount, optionNumber);
                                    if (planDisplayed) {
                                        waitingForOption = false;
                                        waitingForResponse = false;
                                        validMake = true;
                                    }
                                }
                                else {
                                    cout << Config::CHATBOT_NAME << ": Please enter a valid positive option number:" << endl;
                                }
                            }
                            catch (const exception&) {
                                cout << Config::CHATBOT_NAME << ": Please enter a valid option number:" << endl;
                            }
                        }
                    }
                    else if (responseInput == "a" || responseInput == "apply") {
                        // Start car loan application
                        try {
                            LoanApplication application = collector.collectApplicationForLoan("car", "Car Loan", fileManager);

                            if (fileManager.saveApplication(application)) {
                                cout << Config::CHATBOT_NAME << ": " << "Car loan application completed successfully!" << endl;
                                cout << "Your Application ID: " << application.getApplicationId() << endl;
                            }
                            else {
                                cout << Config::CHATBOT_NAME << ": " << "Failed to save application. Please try again." << endl;
                            }
                        }
                        catch (const exception& e) {
                            cout << Config::CHATBOT_NAME << ": " << "Application failed: " << e.what() << endl;
                        }
                        waitingForResponse = false;
                        validMake = true;
                    }
                    else {
                        cout << Config::CHATBOT_NAME << ": Please enter Y for Installment Plan or A for Application:" << endl;
                    }
                }

                if (validMake) {
                    cout << endl << "Press X to exit or any other key to return to main menu." << endl;
                    return;
                }
            }
            else {
                cout << Config::CHATBOT_NAME << ": Please select another make (1 or 2) or press X to exit:" << endl;
            }
        }
        else {
            cout << Config::CHATBOT_NAME << ": Invalid make selection. Please enter 1 or 2:" << endl;
        }
    }
}
/// <summary>
/// Displays scooter loan options for a specific make
/// </summary>
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
            cout << "  Charging Time: " << loans[i].getChargingTime() << " hours" << endl;
            cout << "  Max Speed: " << loans[i].getMaxSpeed() << " KM/H" << endl;
            cout << "  Installments: " << loans[i].getInstallments() << " months" << endl;
            cout << "  Total Price: PKR " << loans[i].getPrice() << endl;
            cout << "  Down Payment: PKR " << loans[i].getDownPayment() << endl;
            cout << "  Monthly Installment: PKR " << monthlyInstallment << endl;
            cout << "----------------------------------------" << endl;
        }
    }

    if (!found) {
        cout << "Sorry, no scooter options available for this make at the moment." << endl;
    }

    return found;
}
/// <summary>
/// Handles scooter loan selection workflow with installment plan option
/// </summary>
void handleScooterLoanSelection(const ScooterLoan loans[], int loanCount,
    const Utterance utterances[], int utteranceCount, bool& running,
    ApplicationCollector& collector, FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": " << "You selected scooter loan. Please select the scooter make." << endl;
    displayScooterMakes();

    bool validMake = false;
    string userInput;

    while (!validMake && running) {
        cout << "You: ";
        getline(cin, userInput);

        string makeInput = trim(userInput);
        string lowerMakeInput = toLower(makeInput);

        if (lowerMakeInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            running = false;
            return;
        }

        if (isValidMakeInput(makeInput, "scooter")) {
            bool hasData = displayScooterLoanOptionsByMake(loans, loanCount, makeInput);

            if (hasData) {
                cout << endl << Config::CHATBOT_NAME << ": "
                    << "Would you like to see the detailed installment plan? Press Y for Yes, or would you like to start the application process? Press A for Application" << endl;

                bool waitingForResponse = true;
                while (waitingForResponse && running) {
                    cout << "You: ";
                    getline(cin, userInput);

                    string responseInput = toLower(trim(userInput));

                    if (responseInput == Config::EXIT_COMMAND) {
                        displayGoodbyeMessage();
                        running = false;
                        return;
                    }
                    else if (responseInput == "y" || responseInput == "yes") {
                        cout << Config::CHATBOT_NAME << ": Please enter the option number (1, 2, 3, etc.) to see detailed installment plan:" << endl;

                        bool waitingForOption = true;
                        while (waitingForOption && running) {
                            cout << "You: ";
                            getline(cin, userInput);

                            string optionInput = trim(userInput);
                            string lowerOptionInput = toLower(optionInput);

                            if (lowerOptionInput == Config::EXIT_COMMAND) {
                                displayGoodbyeMessage();
                                running = false;
                                return;
                            }

                            try {
                                int optionNumber = stoi(optionInput);
                                if (optionNumber > 0) {
                                    bool planDisplayed = displayScooterInstallmentPlan(loans, loanCount, optionNumber);
                                    if (planDisplayed) {
                                        waitingForOption = false;
                                        waitingForResponse = false;
                                        validMake = true;
                                    }
                                }
                                else {
                                    cout << Config::CHATBOT_NAME << ": Please enter a valid positive option number:" << endl;
                                }
                            }
                            catch (const exception&) {
                                cout << Config::CHATBOT_NAME << ": Please enter a valid option number:" << endl;
                            }
                        }
                    }
                    else if (responseInput == "a" || responseInput == "apply") {
                        // Start scooter loan application
                        try {
                            LoanApplication application = collector.collectApplicationForLoan("scooter", "Scooter Loan", fileManager);

                            if (fileManager.saveApplication(application)) {
                                cout << Config::CHATBOT_NAME << ": " << "Scooter loan application completed successfully!" << endl;
                                cout << "Your Application ID: " << application.getApplicationId() << endl;
                            }
                            else {
                                cout << Config::CHATBOT_NAME << ": " << "Failed to save application. Please try again." << endl;
                            }
                        }
                        catch (const exception& e) {
                            cout << Config::CHATBOT_NAME << ": " << "Application failed: " << e.what() << endl;
                        }
                        waitingForResponse = false;
                        validMake = true;
                    }
                    else {
                        cout << Config::CHATBOT_NAME << ": Please enter Y for Installment Plan or A for Application:" << endl;
                    }
                }

                if (validMake) {
                    cout << endl << "Press X to exit or any other key to return to main menu." << endl;
                    return;
                }
            }
            else {
                cout << Config::CHATBOT_NAME << ": Please select another make (1) or press X to exit:" << endl;
            }
        }
        else {
            cout << Config::CHATBOT_NAME << ": Invalid make selection. Please enter 1:" << endl;
        }
    }
}
void handlePersonalLoanSelection(const Utterance utterances[], int utteranceCount, bool& running,
    ApplicationCollector& collector, FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": " << "You selected personal loan." << endl;
    cout << "Personal loans are available for various purposes with flexible terms." << endl;

    cout << endl << "Personal Loan Options:" << endl;
    cout << "========================================" << endl;
    cout << "Option 1: Small Personal Loan" << endl;
    cout << "  Amount: PKR 500,000" << endl;
    cout << "  Installments: 12 months" << endl;
    cout << "  Monthly Installment: PKR 45,000" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Option 2: Medium Personal Loan" << endl;
    cout << "  Amount: PKR 1,000,000" << endl;
    cout << "  Installments: 24 months" << endl;
    cout << "  Monthly Installment: PKR 48,000" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Option 3: Large Personal Loan" << endl;
    cout << "  Amount: PKR 2,000,000" << endl;
    cout << "  Installments: 36 months" << endl;
    cout << "  Monthly Installment: PKR 62,000" << endl;
    cout << "========================================" << endl;

    cout << endl << Config::CHATBOT_NAME << ": "
        << "Would you like to start the application process for a personal loan? (yes/no): ";

    string confirmation;
    getline(cin, confirmation);
    confirmation = toLower(trim(confirmation));

    if (confirmation == "yes" || confirmation == "y") {
        try {
            LoanApplication application = collector.collectApplicationForLoan("personal", "Personal Loan", fileManager);

            if (fileManager.saveApplication(application)) {
                cout << Config::CHATBOT_NAME << ": " << "Personal loan application completed successfully!" << endl;
                cout << "Your Application ID: " << application.getApplicationId() << endl;
            }
            else {
                cout << Config::CHATBOT_NAME << ": " << "Failed to save application. Please try again." << endl;
            }
        }
        catch (const exception& e) {
            cout << Config::CHATBOT_NAME << ": " << "Application failed: " << e.what() << endl;
        }
    }
    else {
        cout << Config::CHATBOT_NAME << ": " << "Returning to main menu." << endl;
    }
}
void handleApplicationSubmission(ApplicationCollector& collector, FileManager& fileManager, bool& running) {
    cout << Config::CHATBOT_NAME << ": " << "Starting loan application submission process..." << endl;
    cout << "I'll guide you through collecting all required information." << endl;
    cout << "You can type 'cancel' at any time to exit this process." << endl << endl;

    try {
        LoanApplication application = collector.collectCompleteApplication();

        ValidationResult validation = application.validateCompleteApplication();

        if (!validation.isValid) {
            cout << Config::CHATBOT_NAME << ": " << "❌ Application validation failed!" << endl;
            cout << validation.getReport() << endl;
            cout << "Please correct the errors and try again." << endl;
            return;
        }

        if (!validation.warnings.empty()) {
            cout << Config::CHATBOT_NAME << ": " << "⚠️ Application warnings:" << endl;
            cout << validation.getReport() << endl;
        }

        cout << endl << Config::CHATBOT_NAME << ": " << "Are you ready to submit your application? (yes/no): ";
        string confirmation;
        getline(cin, confirmation);
        confirmation = toLower(trim(confirmation));

        if (confirmation != "yes" && confirmation != "y") {
            cout << Config::CHATBOT_NAME << ": " << "Application submission cancelled." << endl;
            return;
        }

        if (fileManager.saveApplication(application)) {
            cout << Config::CHATBOT_NAME << ": " << "✅ Application submitted successfully!" << endl;
            cout << "📋 Your Application ID: " << application.getApplicationId() << endl;
            cout << "📅 Submission Date: " << application.getSubmissionDate() << endl;
            cout << "📁 All documents have been saved to the system." << endl;
            cout << "You can check your application status anytime using your CNIC." << endl;
        }
        else {
            cout << Config::CHATBOT_NAME << ": " << "❌ Failed to save application. Please try again." << endl;
        }

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Application process cancelled: " << e.what() << endl;
    }
}

void handleApplicationStatusQuery(FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": " << "To check your application status, please enter your CNIC number (13 digits without dashes): ";
    string cnic;
    getline(cin, cnic);
    cnic = trim(cnic);

    if (cnic.length() != 13) {
        cout << Config::CHATBOT_NAME << ": " << "Invalid CNIC format. Must be 13 digits." << endl;
        return;
    }

    for (char c : cnic) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            cout << Config::CHATBOT_NAME << ": " << "CNIC must contain only digits." << endl;
            return;
        }
    }

    int submitted = 0, approved = 0, rejected = 0;
    fileManager.getApplicationStatsByCNIC(cnic, submitted, approved, rejected);

    cout << endl << Config::CHATBOT_NAME << ": " << "📊 Application Statistics for CNIC: " << cnic << endl;
    cout << "==========================================" << endl;
    cout << "   Submitted: " << submitted << " application(s)" << endl;
    cout << "   Approved:  " << approved << " application(s)" << endl;
    cout << "   Rejected:  " << rejected << " application(s)" << endl;
    cout << "==========================================" << endl;

    auto applications = fileManager.findApplicationsByCNIC(cnic);
    if (!applications.empty()) {
        cout << endl << "Detailed Application List:" << endl;
        for (size_t i = 0; i < applications.size(); i++) {
            cout << "  " << (i + 1) << ". App ID: " << applications[i].getApplicationId()
                << " - Status: " << applications[i].getStatus()
                << " - Date: " << applications[i].getSubmissionDate() << endl;
        }
    }
}

void displayHelpMenu() {
    cout << Config::CHATBOT_NAME << ": " << "Here are the commands I understand:" << endl;
    cout << "==========================================" << endl;
    cout << "  📝 'apply' or 'submit' - Start a new loan application" << endl;
    cout << "  📊 'status' - Check your application status" << endl;
    cout << "  🏠 'h' - Home loan options" << endl;
    cout << "  🚗 'c' - Car loan options" << endl;
    cout << "  🛵 's' - Scooter loan options" << endl;
    cout << "  💰 'p' - Personal loan options" << endl;
    cout << "  ❌ 'x' - Exit the program" << endl;
    cout << "  ❓ 'help' - Show this help menu" << endl;
    cout << "==========================================" << endl;
}

void handleLoanTypeSelection(const HomeLoan homeLoans[], int homeLoanCount,
    const CarLoan carLoans[], int carLoanCount,
    const ScooterLoan scooterLoans[], int scooterLoanCount,
    const Utterance utterances[], int utteranceCount,
    bool& running, bool& inLoanSelection, const string& userInput,
    ApplicationCollector& collector, FileManager& fileManager) {

    string lowerInput = toLower(trim(userInput));

    if (lowerInput == "h") {
        handleHomeLoanSelection(homeLoans, homeLoanCount, utterances, utteranceCount, running);
        inLoanSelection = false;
    }
    else if (lowerInput == "c") {
        handleCarLoanSelection(carLoans, carLoanCount, utterances, utteranceCount, running, collector, fileManager);
        inLoanSelection = false;
    }
    else if (lowerInput == "s") {
        handleScooterLoanSelection(scooterLoans, scooterLoanCount, utterances, utteranceCount, running, collector, fileManager);
        inLoanSelection = false;
    }
    else if (lowerInput == "p") {
        handlePersonalLoanSelection(utterances, utteranceCount, running, collector, fileManager);
        inLoanSelection = false;
    }
    else {
        cout << Config::CHATBOT_NAME << ": " << "Invalid selection. " << endl;
        displayLoanCategories();
    }
}
/// <summary>
/// MAIN ENTRY POINT - Original conversational flow
/// </summary>
int main() {
    // Initialize data structures
    Utterance utterances[Config::MAX_UTTERANCES];
    HomeLoan homeLoans[Config::MAX_LOANS];
    FileManager fileManager;
    CarLoan carLoans[Config::MAX_LOANS];
    ScooterLoan scooterLoans[Config::MAX_LOANS];

    // Load data from files
    int utteranceCount = loadUtterances(utterances, Config::MAX_UTTERANCES, Config::UTTERANCES_FILE);
    int homeLoanCount = loadHomeLoans(homeLoans, Config::MAX_LOANS, Config::HOME_LOANS_FILE);
    int carLoanCount = loadCarLoans(carLoans, Config::MAX_LOANS, Config::CAR_LOANS_FILE);
    int scooterLoanCount = loadScooterLoans(scooterLoans, Config::MAX_LOANS, Config::SCOOTER_LOANS_FILE);
    // Verify data loaded successfully
    if (utteranceCount == 0) {
        cerr << "Failed to load utterances. Please check " << Config::UTTERANCES_FILE << " file." << endl;
        return 1;
    }

    // Display welcome message
    displayWelcomeMessage();

    // Main application loop
    string userInput;
    bool running = true;
    bool inLoanSelection = false;
    ApplicationCollector collector;

    while (running) {
        cout << "You: ";
        getline(cin, userInput);

        string lowerInput = toLower(trim(userInput));

        // Check for exit command
        if (lowerInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            break;
        }

        // Check for loan selection initiation
        if (lowerInput == "a") {
            inLoanSelection = true;
            cout << Config::CHATBOT_NAME << ": " << getResponse(utterances, utteranceCount, userInput) << endl;
            continue;
        }

        // Handle loan type selection
        if (inLoanSelection) {
            handleLoanTypeSelection(homeLoans, homeLoanCount, carLoans, carLoanCount, scooterLoans, scooterLoanCount,
                utterances, utteranceCount, running, inLoanSelection, userInput, collector, fileManager);   
            continue;
        }

        // Handle application submission
        if (lowerInput == "submit" || lowerInput == "apply") {
            handleApplicationSubmission(collector, fileManager, running); 
            continue;
        }

        // Handle status queries
        if (lowerInput == "status" || lowerInput == "check status") {
            handleApplicationStatusQuery(fileManager);
            continue;
        }

        // Handle help
        if (lowerInput == "help") {
            displayHelpMenu();
            continue;
        }

        // Default response from utterances
        cout << Config::CHATBOT_NAME << ": " << getResponse(utterances, utteranceCount, userInput) << endl;
    }

    return 0;
}