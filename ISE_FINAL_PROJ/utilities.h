#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <cctype>
#include <ctime>
using namespace std;

/// <summary>
/// Configuration constants for the loan processing system
/// </summary>
namespace Config {
    extern const string CHATBOT_NAME;
    extern const string UTTERANCES_FILE;
    extern const string HOME_LOANS_FILE;
    extern const int MAX_UTTERANCES;
    extern const int MAX_LOANS;
    extern const char DELIMITER;
    extern const string EXIT_COMMAND;
    extern const string APPLICATIONS_FILE;
    extern const string IMAGES_DIRECTORY;
}

// String Processing Utilities

/// <summary>
/// Removes leading and trailing whitespace from a string
/// </summary>
/// <param name="str">Input string to trim</param>
/// <returns>Trimmed string without leading/trailing whitespace</returns>
inline string trim(const string& str) {
    if (str.empty()) return "";

    size_t start = 0;
    size_t end = str.length() - 1;

    while (start <= end && isspace(static_cast<unsigned char>(str[start])))
        start++;
    while (end >= start && isspace(static_cast<unsigned char>(str[end])))
        end--;

    return (start <= end) ? str.substr(start, end - start + 1) : "";
}

/// <summary>
/// Converts a string to lowercase
/// </summary>
/// <param name="str">Input string to convert</param>
/// <returns>String converted to lowercase</returns>
inline string toLower(const string& str) {
    string result = str;
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(static_cast<unsigned char>(result[i]));
    }
    return result;
}

/// <summary>
/// Splits a string into tokens using specified delimiter
/// </summary>
/// <param name="line">Input string to split</param>
/// <param name="delimiter">Character used to separate tokens</param>
/// <returns>Vector of trimmed token strings</returns>
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
/// Removes commas from numeric strings for parsing
/// </summary>
/// <param name="numStr">String containing numbers with commas</param>
/// <returns>String with commas removed</returns>
inline string removeCommas(const string& numStr) {
    string result = "";
    for (char ch : numStr) {
        if (ch != ',') result += ch;
    }
    return result;
}

// Date and Time Utilities

/// <summary>
/// Gets the full month name from month number
/// </summary>
/// <param name="month">Month number (1-12)</param>
/// <returns>Full month name as string</returns>
inline string getMonthName(int month) {
    const string months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    return (month >= 1 && month <= 12) ? months[month - 1] : "Invalid";
}

/// <summary>
/// Advances to the next month, handling year rollover
/// </summary>
/// <param name="month">Current month (will be modified)</param>
/// <param name="year">Current year (will be modified if month rolls over)</param>
inline void getNextMonth(int& month, int& year) {
    month++;
    if (month > 12) {
        month = 1;
        year++;
    }
}

/// <summary>
/// Gets current system date in DD-MM-YYYY format
/// </summary>
/// <returns>Current date as string in DD-MM-YYYY format</returns>
inline string getCurrentDate() {
    time_t now = time(0);
    struct tm timeinfo = {};

#ifdef _WIN32
    localtime_s(&timeinfo, &now);
#else
    timeinfo = *localtime(&now);
#endif

    string day = (timeinfo.tm_mday < 10) ? "0" + to_string(timeinfo.tm_mday) : to_string(timeinfo.tm_mday);
    string month = (timeinfo.tm_mon + 1 < 10) ? "0" + to_string(timeinfo.tm_mon + 1) : to_string(timeinfo.tm_mon + 1);
    string year = to_string(timeinfo.tm_year + 1900);

    return day + "-" + month + "-" + year;
}

/// <summary>
/// Compares two dates in DD-MM-YYYY format
/// </summary>
/// <param name="date1">First date to compare</param>
/// <param name="date2">Second date to compare</param>
/// <returns>-1 if date1 < date2, 0 if equal, 1 if date1 > date2</returns>
inline int compareDates(const string& date1, const string& date2) {
    int day1 = stoi(date1.substr(0, 2));
    int month1 = stoi(date1.substr(3, 2));
    int year1 = stoi(date1.substr(6, 4));

    int day2 = stoi(date2.substr(0, 2));
    int month2 = stoi(date2.substr(3, 2));
    int year2 = stoi(date2.substr(6, 4));

    if (year1 != year2) return year1 < year2 ? -1 : 1;
    if (month1 != month2) return month1 < month2 ? -1 : 1;
    if (day1 != day2) return day1 < day2 ? -1 : 1;

    return 0;
}

#endif