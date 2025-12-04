#include "utterances.h"
#include "utilities.h"
#include <fstream>
#include <iostream>

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