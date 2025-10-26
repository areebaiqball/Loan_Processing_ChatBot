#include "utterances.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <cctype>

/// <summary>
/// Constants improve maintainability and evolvability
/// </summary>
namespace Config {
    extern const string CHATBOT_NAME;
    extern const string UTTERANCES_FILE;
    extern const int MAX_UTTERANCES;
    extern const int MAX_LOANS;
    extern const char DELIMITER;
    extern const string EXIT_COMMAND;
}

// Forward declarations of utility functions
string trim(const string& str);
string toLower(const string& str);
vector<string> splitString(const string& line, char delimiter);

// Utterance class implementation
Utterance::Utterance() {
    input = "";
    response = "";
}

Utterance::Utterance(const string& inp, const string& resp) {
    input = inp;
    response = resp;
}

string Utterance::getInput() const { return input; }
string Utterance::getResponse() const { return response; }

void Utterance::setInput(const string& inp) { input = inp; }
void Utterance::setResponse(const string& resp) { response = resp; }

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
/// Loads utterances from files
/// </summary>
/// <param name="utterances">Array to store utterances</param>
/// <param name="maxSize">Maximum array size</param>
/// <param name="filename">File to load from</param>
/// <returns>No. of utterances loaded</returns>
int loadUtterances(Utterance utterances[], int maxSize, const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return 0;
    }

    string line;
    int count = 0;
    int lineNumber = 0;

    while (getline(file, line) && count < maxSize) {
        lineNumber++;

        if (line.empty()) {
            continue;
        }

        vector<string> parts = splitString(line, Config::DELIMITER);

        if (parts.size() >= 2) {
            utterances[count].setInput(toLower(parts[0]));
            utterances[count].setResponse(parts[1]);
            count++;
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
/// Gets response for user input
/// </summary>
/// <param name="utterances">Array of utterances</param>
/// <param name="size">Array size</param>
/// <param name="input">User input</param>
/// <returns>Response string</returns>
string getResponse(const Utterance utterances[], int size, const string& input) {
    string lowerInput = toLower(trim(input));

    for (int i = 0; i < size; i++) {
        if (utterances[i].getInput() == lowerInput) {
            return utterances[i].getResponse();
        }
    }

    for (int i = 0; i < size; i++) {
        if (utterances[i].getInput() == "*") {
            return utterances[i].getResponse();
        }
    }

    return "Sorry, I didn't understand that. Please try again.";
}