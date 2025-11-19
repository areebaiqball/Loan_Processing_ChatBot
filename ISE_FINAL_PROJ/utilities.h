#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <cctype>
#include <ctime>
using namespace std;

// SE Principles: Namespace organization, Utility functions

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

inline string toLower(const string& str) {
    string result = str;
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(static_cast<unsigned char>(result[i]));
    }
    return result;
}

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

inline string removeCommas(const string& numStr) {
    string result = "";
    for (char ch : numStr) {
        if (ch != ',') result += ch;
    }
    return result;
}

// Date and Time Utilities
inline string getMonthName(int month) {
    const string months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    return (month >= 1 && month <= 12) ? months[month - 1] : "Invalid";
}

inline void getNextMonth(int& month, int& year) {
    month++;
    if (month > 12) {
        month = 1;
        year++;
    }
}

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