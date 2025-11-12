#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <cctype>
using namespace std;

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

/// <summary>
/// Trims whitespace from string
/// </summary>
/// <param name="str"> input string </param>
/// <returns> trimmed string </returns>
inline string trim(const string& str) {
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
inline string toLower(const string& str) {
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
inline vector<string> splitString(const string& line, char delimiter) {
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
/// Removes commas from a numeric string
/// </summary>
/// <param name="numStr">String containing digits and commas</param>
/// <returns>String with commas removed</returns>
inline string removeCommas(const string& numStr) {
    string result = "";
    for (char ch : numStr) {
        if (ch != ',') result += ch;
    }
    return result;
}

#endif
