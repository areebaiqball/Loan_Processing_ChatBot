#include <iostream>
#include <string>
#include "utterances.h"
#include "home.h"
#include "utilities.h"
#include "application.h"
#include "application_collector.h"
using namespace std;

/// <summary>
/// Constants improve maintainability and evolvability
/// </summary>
namespace Config {
    const string CHATBOT_NAME = "Dora";
    const string UTTERANCES_FILE = "Utterances.txt";
    const string HOME_LOANS_FILE = "Home.txt";
    const int MAX_UTTERANCES = 100;
    const int MAX_LOANS = 100;
    const char DELIMITER = '#';
    const string EXIT_COMMAND = "x";
}

/// <summary>
/// Validates area input
/// </summary>
/// <param name="input">User input</param>
/// <returns>True if valid, false otherwise</returns>
bool isValidAreaInput(const string& input) {
    return (input == "1" || input == "2" || input == "3" || input == "4");
}

/// <summary>
///  Validates loan type input
/// </summary>
/// <param name="input">user input</param>
/// <returns>True if valid, false otherwise</returns>
bool isValidLoanType(const string& input) {
    return (input == "h" || input == "c" || input == "s" || input == "p");
}

/// <summary>
/// Displays welcome message
/// </summary>
void displayWelcomeMessage() {
    cout << "   Loan Processing Chatbot - " << Config::CHATBOT_NAME << endl << endl;
    cout << Config::CHATBOT_NAME << ": Hello! I'm " << Config::CHATBOT_NAME
        << ", your friendly loan assistant." << endl <<
        "I'm here to help you find the perfect loan for your needs." << endl <<
        "Let's get started on your journey to financial success!" << endl;
}

/// <summary>
/// Displays loan category options
/// </summary>
void displayLoanCategories() {
    cout << Config::CHATBOT_NAME << ": Please select the category you want to apply for." << endl;
    cout << "  Press H for a home loan" << endl;
    cout << "  Press C for a car loan" << endl;
    cout << "  Press S for a scooter loan" << endl;
    cout << "  Press P for a personal loan" << endl;
    cout << "  Press X to exit" << endl;
}

/// <summary>
/// Displays unavailable loan message
/// </summary>
void displayUnavailableLoanMessage() {
    cout << Config::CHATBOT_NAME << ": Sorry, no data available for this loan type at the moment." << endl;
    displayLoanCategories();
}

/// <summary>
/// Displays goodbye message
/// </summary>
void displayGoodbyeMessage() {
    cout << Config::CHATBOT_NAME << ": Thank you for using our service. Goodbye!" << endl;
}

/// <summary>
/// Handles home loan selection workflow with installment plan option
/// </summary>
/// <param name="loans">Array of loans</param>
/// <param name="loanCount">Number of loans</param>
/// <param name="utterances">Array of utterances</param>
/// <param name="utteranceCount">Number of utterances</param>
/// <param name="running">Reference to running flag</param>
void handleHomeLoanSelection(const HomeLoan loans[], int loanCount,
    const Utterance utterances[], int utteranceCount,
    bool& running) {
    cout << Config::CHATBOT_NAME << ": " << getResponse(utterances, utteranceCount, "h") << endl;

    bool validArea = false;
    string userInput;

    while (!validArea && running) {
        cout << "You: ";
        getline(cin, userInput);

        string areaInput = trim(userInput);
        string lowerAreaInput = toLower(areaInput);

        // Check for exit
        if (lowerAreaInput == Config::EXIT_COMMAND) {
            displayGoodbyeMessage();
            running = false;
            return;
        }

        // Validate area input
        if (isValidAreaInput(areaInput)) {
            bool hasData = displayHomeLoanOptions(loans, loanCount, areaInput);

            if (hasData) {
                // Ask if user wants to see installment plan
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
                        // Ask for option number
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
                            catch (const exception& e) {
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
/// Handles loan application collection workflow
/// </summary>
/// <param name="collector">Application collector instance</param>
/// <param name="running">Reference to running flag</param>
void handleLoanApplication(ApplicationCollector& collector, bool& running) {
    cout << Config::CHATBOT_NAME << ": " << "Starting loan application process..." << endl;
    cout << "I'll guide you through the application step by step." << endl;
    cout << "You can type 'cancel' at any time to exit." << endl << endl;

    try {
        LoanApplication application = collector.collectCompleteApplication();
        cout << Config::CHATBOT_NAME << ": " << " Application collected successfully!" << endl;
        cout << "Your application is ready for submission." << endl;
        cout << "Application ID: " << application.getApplicationId() << endl; // Will be set in LPC-14

        // TODO: Save application in LPC-14
        // saveLoanApplication(application, Config::APPLICATIONS_FILE);

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Application process cancelled: " << e.what() << endl;
    }

    cout << endl << "Press X to exit or any other key to return to main menu." << endl;
}
/// <summary>
/// Handles loan type selection workflow
/// </summary>
void handleLoanTypeSelection(const HomeLoan loans[], int loanCount,
    const Utterance utterances[], int utteranceCount,
    bool& running, bool& inLoanSelection,
    const string& userInput, ApplicationCollector& collector) {  // ADD collector parameter
    string lowerInput = toLower(trim(userInput));

    if (lowerInput == "h") {
        handleHomeLoanSelection(loans, loanCount, utterances, utteranceCount, running);
        inLoanSelection = false;
    }
    else if (lowerInput == "s" || lowerInput == "c" || lowerInput == "p") {
        string loanType = (lowerInput == "c" ? "car" : lowerInput == "s" ? "scooter" : "personal");
        string loanDetails = (lowerInput == "c" ? "Car Loan" : lowerInput == "s" ? "Scooter Loan" : "Personal Loan");

        // Show loan type confirmation and start application
        cout << Config::CHATBOT_NAME << ": " << "You selected " << loanType << " loan." << endl;
        cout << "Would you like to proceed with the application? (yes/no): ";

        string confirmation;
        getline(cin, confirmation);
        confirmation = toLower(trim(confirmation));

        if (confirmation == "yes" || confirmation == "y") {
            // Start application for the selected loan type
            LoanApplication application = collector.collectApplicationForLoan(loanType, loanDetails);
            // TODO: Save application in LPC-14
            cout << Config::CHATBOT_NAME << ": " << "✅ " << loanType << " loan application completed!" << endl;
        }
        else {
            cout << Config::CHATBOT_NAME << ": " << "Application cancelled. Returning to loan selection." << endl;
            displayLoanCategories();
            return; // Stay in loan selection
        }

        inLoanSelection = false;
    }
    else {
        cout << Config::CHATBOT_NAME << ": " << "Invalid selection. " << endl;
        displayLoanCategories();
        // Keep inLoanSelection as true
    }
}

/// <summary>
/// main entry point to the program
/// </summary>
/// <returns></returns>
int main() {
    // Initialize data structures
    Utterance utterances[Config::MAX_UTTERANCES];
    HomeLoan homeLoans[Config::MAX_LOANS];

    // Load data from files
    int utteranceCount = loadUtterances(utterances, Config::MAX_UTTERANCES,
        Config::UTTERANCES_FILE);
    int loanCount = loadHomeLoans(homeLoans, Config::MAX_LOANS,
        Config::HOME_LOANS_FILE);

    // Verify data loaded successfully
    if (utteranceCount == 0) {
        cerr << "Failed to load utterances. Please check "
            << Config::UTTERANCES_FILE << " file." << endl;
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

        // Check for loan selection initiation - ONLY "a" is valid, not "aoa"
        if (lowerInput == "a") {
            inLoanSelection = true;
            cout << Config::CHATBOT_NAME << ": "
                << getResponse(utterances, utteranceCount, userInput) << endl;
            continue;
        }
       

       
        if (inLoanSelection) {
            handleLoanTypeSelection(homeLoans, loanCount, utterances,
                utteranceCount, running, inLoanSelection, userInput,collector);
            continue;
        }

        
        cout << Config::CHATBOT_NAME << ": "
            << getResponse(utterances, utteranceCount, userInput) << endl;
    }

    return 0;
}
