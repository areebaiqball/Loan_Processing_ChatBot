#pragma once
#ifndef UTTERANCES_H
#define UTTERANCES_H

#include <string>
using namespace std;

/// <summary>
/// Represents a chatbot utterance with input and response
/// </summary>
class Utterance {
private:
    string input;
    string response;

public:
    // Constructor 
    Utterance();
    Utterance(const string& inp, const string& resp);

    // Getters 
    string getInput() const;
    string getResponse() const;

    // Setters 
    void setInput(const string& inp);
    void setResponse(const string& resp);
};

/// <summary>
/// Loads utterances from files
/// </summary>
/// <param name="utterances">Array to store utterances</param>
/// <param name="maxSize">Maximum array size</param>
/// <param name="filename">File to load from</param>
/// <returns>No. of utterances loaded</returns>
int loadUtterances(Utterance utterances[], int maxSize, const string& filename);

/// <summary>
/// Gets response for user input
/// </summary>
/// <param name="utterances">Array of utterances</param>
/// <param name="size">Array size</param>
/// <param name="input">User input</param>
/// <returns>Response string</returns>
string getResponse(const Utterance utterances[], int size, const string& input);

#endif