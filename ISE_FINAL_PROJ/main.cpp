#include <iostream>
#include <string>
#include "utterances.h"
#include "home.h"
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
/// Trims whitespace from string
/// </summary>
/// <param name="str"> input string </param>
/// <returns> trimmed string </returns>
string trim(const string& str) {
    if (str.empty()) {
        return "";
    }

    size_t start = 0;
    size_t end = str.length() - 1;

    // Find first non-whitespace character
    while (start <= end && isspace(static_cast<unsigned char>(str[start]))) {
        start++;
    }

    // Find last non-whitespace character
    while (end >= start && isspace(static_cast<unsigned char>(str[end]))) {
        end--;
    }

    return (start <= end) ? str.substr(start, end - start + 1) : "";
}

/// <summary>
/// Converts string to lowercase
/// </summary>
/// <param name="str">input string</param>
/// <returns>lowercase string</returns>
string toLower(const string& str) {
    string result = str;
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(static_cast<unsigned char>(result[i]));
    }
    return result;
}

/// <summary>
/// Handles home loan selection workflow
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
                cout << endl << "Press X to exit or any other key to return to main menu." << endl;
                validArea = true;
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
/// Handles loan type selection workflow
/// </summary>
/// <param name="loans">Array of loans</param>
/// <param name="loanCount">Number of loans</param>
/// <param name="utterances">Array of utterances</param>
/// <param name="utteranceCount">Number of utterances</param>
/// <param name="running">Reference to running flag</param>
/// <param name="inLoanSelection">Reference to loan selection state</param>
/// <param name="userInput">Current user input</param>
void handleLoanTypeSelection(const HomeLoan loans[], int loanCount,
    const Utterance utterances[], int utteranceCount,
    bool& running, bool& inLoanSelection,
    const string& userInput) {
    string lowerInput = toLower(trim(userInput));

    if (lowerInput == "h") {
        handleHomeLoanSelection(loans, loanCount, utterances, utteranceCount, running);
        inLoanSelection = false;
    }
    else if (lowerInput == "s" || lowerInput == "c" || lowerInput == "p") {
        displayUnavailableLoanMessage();
    }
    else {
        cout << Config::CHATBOT_NAME << ": Invalid selection. " << endl;
        displayLoanCategories();
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

   
    string userInput;
    bool running = true;
    bool inLoanSelection = false;

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
            cout << Config::CHATBOT_NAME << ": "
                << getResponse(utterances, utteranceCount, userInput) << endl;
            continue;
        }

        // Handle loan selection workflow
        if (inLoanSelection) {
            handleLoanTypeSelection(homeLoans, loanCount, utterances,
                utteranceCount, running, inLoanSelection, userInput);
            continue;
        }

        // Default response for invalid input at main level
        cout << Config::CHATBOT_NAME << ": "
            << getResponse(utterances, utteranceCount, userInput) << endl;
    }

    return 0;
}