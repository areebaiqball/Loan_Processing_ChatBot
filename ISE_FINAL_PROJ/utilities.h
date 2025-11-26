#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <cctype>
#include <ctime>
#include <sstream>
#include <unordered_set>
#include <fstream>
using namespace std;

/// <summary>
/// Configuration constants for the loan processing system
/// </summary>
// SE Principles: Namespace organization, Configuration management
namespace Config {
    const string CHATBOT_NAME = "Dora";
    const string UTTERANCES_FILE = "Utterances.txt";
    const string HOME_LOANS_FILE = "Home.txt";
    const string CAR_LOANS_FILE = "Car.txt";
    const string SCOOTER_LOANS_FILE = "Scooter.txt";
    const string PERSONAL_LOANS_FILE = "Personal.txt";
    const int MAX_UTTERANCES = 100;
    const int MAX_LOANS = 100;
    const char DELIMITER = '#';
    const string EXIT_COMMAND = "x";
    const string APPLICATIONS_FILE = "applications.txt";
    const string IMAGES_DIRECTORY = "images/";
}

/// <summary>
/// Structure to hold conversation pairs from human_chat_corpus.txt
/// </summary>
struct ConversationPair {
    string human1;
    string human2;
};

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

/// <summary>
/// Tokenizes a string into words (converts to lowercase and removes punctuation)
/// </summary>
/// <param name="text">Input text to tokenize</param>
/// <returns>Vector of cleaned tokens</returns>
inline vector<string> tokenize(const string& text) {
    vector<string> tokens;
    istringstream iss(text);
    string token;

    while (iss >> token) {
        // Convert to lowercase and remove punctuation
        string cleanToken;
        for (char c : token) {
            if (isalnum(static_cast<unsigned char>(c))) {
                cleanToken += tolower(static_cast<unsigned char>(c));
            }
        }
        if (!cleanToken.empty()) {
            tokens.push_back(cleanToken);
        }
    }
    return tokens;
}

/// <summary>
/// Calculates Intersection over Union (IoU) between two strings
/// </summary>
/// <param name="text1">First text to compare</param>
/// <param name="text2">Second text to compare</param>
/// <returns>IoU score between 0.0 and 1.0</returns>
inline double calculateIoU(const string& text1, const string& text2) {
    auto tokens1 = tokenize(text1);
    auto tokens2 = tokenize(text2);

    unordered_set<string> set1(tokens1.begin(), tokens1.end());
    unordered_set<string> set2(tokens2.begin(), tokens2.end());

    // Calculate intersection
    int intersection = 0;
    for (const auto& token : set1) {
        if (set2.find(token) != set2.end()) {
            intersection++;
        }
    }

    // Calculate union
    unordered_set<string> unionSet;
    unionSet.insert(set1.begin(), set1.end());
    unionSet.insert(set2.begin(), set2.end());
    int unionSize = unionSet.size();

    if (unionSize == 0) return 0.0;

    return static_cast<double>(intersection) / unionSize;
}

/// <summary>
/// Loads conversation corpus from human_chat_corpus.txt file
/// </summary>
/// <param name="filename">Path to the corpus file</param>
/// <returns>Vector of conversation pairs</returns>
inline vector<ConversationPair> loadConversationCorpus(const string& filename) {
    vector<ConversationPair> corpus;
    ifstream file(filename);
    string line;

    ConversationPair currentPair;

    while (getline(file, line)) {
        if (line.find("Human 1:") == 0) {
            if (!currentPair.human1.empty()) {
                corpus.push_back(currentPair);
            }
            currentPair.human1 = line.substr(8); // Remove "Human 1:"
            currentPair.human2 = "";
        }
        else if (line.find("Human 2:") == 0) {
            currentPair.human2 = line.substr(8); // Remove "Human 2:"
            corpus.push_back(currentPair);
            currentPair = ConversationPair();
        }
    }

    // Add the last pair if exists
    if (!currentPair.human1.empty()) {
        corpus.push_back(currentPair);
    }

    return corpus;
}

/// <summary>
/// Gets response using IoU matching with conversation corpus
/// </summary>
/// <param name="corpus">Loaded conversation corpus</param>
/// <param name="userInput">User's input message</param>
/// <returns>Best matching response from corpus</returns>
inline string getResponseByIoU(const vector<ConversationPair>& corpus, const string& userInput) {
    if (corpus.empty()) {
        return "I'm still learning to chat. How can I help with your loan today?";
    }

    double bestIoU = 0.0;
    string bestResponse = corpus[0].human2; // Default response

    for (const auto& pair : corpus) {
        double iou = calculateIoU(userInput, pair.human1);
        if (iou > bestIoU) {
            bestIoU = iou;
            bestResponse = pair.human2;
        }
    }

    // Set a threshold for minimum IoU score
    if (bestIoU < 0.1) {
        return "I'm not sure I understand. Could you rephrase that, or would you like help with a loan application?";
    }

    return bestResponse;
}

#endif