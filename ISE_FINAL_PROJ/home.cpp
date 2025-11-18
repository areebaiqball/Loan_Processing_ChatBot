#include "home.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>

using namespace std;

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

long long HomeLoan::calculateMonthlyInstallment() const {
    if (installments <= 0) return 0;
    long long remainingAmount = (price - downPayment);
    return remainingAmount / installments;
}

/// <summary>
/// Loads home loan data from file
/// </summary>
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
                loans[count].setInstallments(stoi(removeCommas(tokens[2])));
                loans[count].setPrice(stoll(removeCommas(tokens[3])));
                loans[count].setDownPayment(stoll(removeCommas(tokens[4])));

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
bool displayHomeLoanOptions(const HomeLoan loans[], int size, const string& areaNumber) {
    string areaName = "Area " + areaNumber;
    bool found = false;
    int optionCount = 0;

    cout << endl << "  Home Loan Options for " << areaName << endl;
    cout << "+-----+----------+--------------+-------------+---------------+-------------------------+" << endl;
    cout << "| Opt |   Size   | Installments | Total Price | Down Payment  | Monthly Installment     |" << endl;
    cout << "+-----+----------+--------------+-------------+---------------+-------------------------+" << endl;

    for (int i = 0; i < size; i++) {
        if (loans[i].getArea() == areaName) {
            found = true;
            optionCount++;
            long long monthlyInstallment = loans[i].calculateMonthlyInstallment();
            cout << "| " << setw(3) << optionCount << " | "  
                << setw(8) << loans[i].getSize() << " | "
                << setw(12) << loans[i].getInstallments() << " | "
                << "PKR " << setw(8) << loans[i].getPrice() << " | "
                << "PKR " << setw(10) << loans[i].getDownPayment() << " | "
                << "PKR " << setw(17) << monthlyInstallment << " |" << endl;
        }
    }

    cout << "+-----+----------+--------------+-------------+---------------+-------------------------+" << endl;

    if (!found) {
        cout << "Sorry, no loan options available for this area at the moment." << endl;
    }

    return found;
}

/// <summary>
/// Displays detailed installment plan for the loan
/// </summary>
void HomeLoan::displayInstallmentPlan() const {
    long long monthlyInstallment = calculateMonthlyInstallment();
    long long remainingBalance = getPrice() - getDownPayment();

    cout << endl << "  INSTALLMENT PLAN DETAILS" << endl;
    cout << "========================================" << endl;
    cout << "Property Details:" << endl;
    cout << "  Area: " << getArea() << endl;
    cout << "  Size: " << getSize() << endl;
    cout << "  Total Price: PKR " << getPrice() << endl;
    cout << "  Down Payment: PKR " << getDownPayment() << endl;
    cout << "  Loan Amount: PKR " << remainingBalance << endl;
    cout << "  Installment Period: " << getInstallments() << " months" << endl;
    cout << "  Monthly Installment: PKR " << monthlyInstallment << endl;
    cout << "========================================" << endl << endl;

    cout << "Monthly Payment Schedule:" << endl;
    cout << "+-------+----------------+---------------+" << endl;
    cout << "| Month |  Payment Due   | Remaining Bal |" << endl;
    cout << "+-------+----------------+---------------+" << endl;

    long long currentBalance = remainingBalance;

    for (int month = 1; month <= getInstallments(); month++) {
        long long paymentDue = monthlyInstallment;

        if (month == getInstallments()) {
            paymentDue = currentBalance;
        }

        cout << "| " << setw(5) << month << " | PKR " << setw(10) << paymentDue
            << " | PKR " << setw(10) << (currentBalance - paymentDue) << " |" << endl;

        currentBalance -= paymentDue;
        if (currentBalance < 0) currentBalance = 0;
    }

    cout << "+-------+----------------+---------------+" << endl << endl;

    long long totalPayment = getDownPayment() + (monthlyInstallment * getInstallments());
    long long totalInstallments = monthlyInstallment * getInstallments();

    cout << "Payment Summary:" << endl;
    cout << "  Total Payment: PKR " << totalPayment << endl;
    cout << "  Down Payment: PKR " << getDownPayment() << endl;
    cout << "  Total Installments: PKR " << totalInstallments << endl;
}

/// <summary>
/// Displays installment plan for specific loan option
/// </summary>
bool displayInstallmentPlanForOption(const HomeLoan loans[], int size,
    const string& areaNumber, int optionNumber) {
    string areaName = "Area " + areaNumber;
    int currentOption = 0;

    for (int i = 0; i < size; i++) {
        if (loans[i].getArea() == areaName) {
            currentOption++;
            if (currentOption == optionNumber) {
                loans[i].displayInstallmentPlan();
                return true;
            }
        }
    }

    cout << "Invalid option number. Please try again." << endl;
    return false;
}