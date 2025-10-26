#include "home.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cctype>

/// <summary>
/// Constants improve maintainability and evolvability
/// </summary>
namespace Config {
    extern const string CHATBOT_NAME;
    extern const string UTTERANCES_FILE;
    extern const string HOME_LOANS_FILE;
    extern const int MAX_UTTERANCES;
    extern const int MAX_LOANS;
    extern const char DELIMITER;
    extern const string EXIT_COMMAND;
}

// Forward declarations of utility functions
string trim(const string& str);
vector<string> splitString(const string& line, char delimiter);

// HomeLoan class implementation
HomeLoan::HomeLoan() {
    area = "";
    size = "";
    installments = 0;
    price = 0;
    downPayment = 0;
}

HomeLoan::HomeLoan(const string& a, const string& s, int inst, long long p, long long dp) {
    if (inst < 0 || p < 0 || dp < 0) {
        throw invalid_argument("Loan values cannot be negative");
    }
    area = a;
    size = s;
    installments = inst;
    price = p;
    downPayment = dp;
}

string HomeLoan::getArea() const { return area; }
string HomeLoan::getSize() const { return size; }
int HomeLoan::getInstallments() const { return installments; }
long long HomeLoan::getPrice() const { return price; }
long long HomeLoan::getDownPayment() const { return downPayment; }

void HomeLoan::setArea(const string& a) { area = a; }
void HomeLoan::setSize(const string& s) { size = s; }

void HomeLoan::setInstallments(int inst) {
    if (inst < 0) {
        throw invalid_argument("Installments cannot be negative");
    }
    installments = inst;
}

void HomeLoan::setPrice(long long p) {
    if (p < 0) {
        throw invalid_argument("Price cannot be negative");
    }
    price = p;
}

void HomeLoan::setDownPayment(long long dp) {
    if (dp < 0) {
        throw invalid_argument("Down payment cannot be negative");
    }
    downPayment = dp;
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
/// splits string by delimitter
/// </summary>
/// <param name="line">input line</param>
/// <param name="delimiter">delimmiter character</param>
/// <returns>vector of tokens</returns>
vector<string> splitString(const string& line, char delimiter) {
    vector<string> tokens;
    string current = "";

    for (char ch : line) {
        if (ch == delimiter) {
            tokens.push_back(trim(current));
            current = "";
        }
        else {
            current += ch;
        }
    }

    if (!current.empty()) {
        tokens.push_back(trim(current));
    }

    return tokens;
}

/// <summary>
/// Loads home loan data from file
/// </summary>
/// <param name="loans">loans Array to store loans</param>
/// <param name="maxSize">Maximum array size</param>
/// <param name="filename">File to load from</param>
/// <returns>Number of loans loaded</returns>
int loadHomeLoans(HomeLoan loans[], int maxSize, const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return 0;
    }

    string line;
    int count = 0;
    int lineNumber = 0;
    bool firstLine = true;

    while (getline(file, line) && count < maxSize) {
        lineNumber++;

        if (firstLine) {
            firstLine = false;
            continue;
        }

        if (line.empty()) {
            continue;
        }

        vector<string> tokens = splitString(line, Config::DELIMITER);

        if (tokens.size() == 5) {
            try {
                loans[count].setArea(tokens[0]);
                loans[count].setSize(tokens[1]);
                loans[count].setInstallments(stoi(tokens[2]));
                loans[count].setPrice(stoll(tokens[3]));
                loans[count].setDownPayment(stoll(tokens[4]));
                count++;
            }
            catch (const exception& e) {
                cerr << "Warning: Invalid data at line " << lineNumber
                    << " in " << filename << ": " << e.what() << endl;
            }
        }
        else {
            cerr << "Warning: Invalid format at line " << lineNumber
                << " in " << filename << endl;
        }
    }

    file.close();
    return count;
}

/// <summary>
/// Displays home loan options for area
/// </summary>
/// <param name="loans">Array of loans</param>
/// <param name="size">Array size</param>
/// <param name="areaNumber">Area identifier</param>
/// <returns>True if data found, false otherwise</returns>
bool displayHomeLoanOptions(const HomeLoan loans[], int size, const string& areaNumber) {
    string areaName = "Area " + areaNumber;
    bool found = false;
    int optionCount = 0;

    cout << endl << "  Home Loan Options for " << areaName << endl << endl;

    for (int i = 0; i < size; i++) {
        if (loans[i].getArea() == areaName) {
            found = true;
            optionCount++;
            cout << "Option " << optionCount << ":" << endl;
            cout << "  Size: " << loans[i].getSize() << endl;
            cout << "  Installments: " << loans[i].getInstallments() << " months" << endl;
            cout << "  Total Price: PKR " << loans[i].getPrice() << endl;
            cout << "  Down Payment: PKR " << loans[i].getDownPayment() << endl;
            cout << "----------------------------------------" << endl;
        }
    }

    if (!found) {
        cout << "Sorry, no loan options available for this area at the moment." << endl;
    }

    return found;
}