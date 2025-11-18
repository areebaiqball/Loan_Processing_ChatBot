#include "car.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>

// CarLoan class implementation
CarLoan::CarLoan() {
    make = "";
    model = "";
    engine = "";
    used = "";
    manufacturingYear = "";
    installments = 0;
    price = 0;
    downPayment = 0;
}

CarLoan::CarLoan(const string& mk, const string& mdl, const string& eng, const string& usd,
    const string& year, int inst, long long p, long long dp) {
    if (inst < 0 || p < 0 || dp < 0) {
        throw invalid_argument("Loan values cannot be negative");
    }
    make = mk;
    model = mdl;
    engine = eng;
    used = usd;
    manufacturingYear = year;
    installments = inst;
    price = p;
    downPayment = dp;
}

string CarLoan::getMake() const { return make; }
string CarLoan::getModel() const { return model; }
string CarLoan::getEngine() const { return engine; }
string CarLoan::getUsed() const { return used; }
string CarLoan::getManufacturingYear() const { return manufacturingYear; }
int CarLoan::getInstallments() const { return installments; }
long long CarLoan::getPrice() const { return price; }
long long CarLoan::getDownPayment() const { return downPayment; }

void CarLoan::setMake(const string& mk) { make = mk; }
void CarLoan::setModel(const string& mdl) { model = mdl; }
void CarLoan::setEngine(const string& eng) { engine = eng; }
void CarLoan::setUsed(const string& usd) { used = usd; }
void CarLoan::setManufacturingYear(const string& year) { manufacturingYear = year; }

void CarLoan::setInstallments(int inst) {
    if (inst < 0) {
        throw invalid_argument("Installments cannot be negative");
    }
    installments = inst;
}

void CarLoan::setPrice(long long p) {
    if (p < 0) {
        throw invalid_argument("Price cannot be negative");
    }
    price = p;
}

void CarLoan::setDownPayment(long long dp) {
    if (dp < 0) {
        throw invalid_argument("Down payment cannot be negative");
    }
    downPayment = dp;
}

long long CarLoan::calculateMonthlyInstallment() const {
    if (installments <= 0) return 0;
    long long remainingAmount = (price - downPayment);
    return remainingAmount / installments;
}

void CarLoan::displayInstallmentPlan() const {
    long long monthlyInstallment = calculateMonthlyInstallment();
    long long remainingBalance = getPrice() - getDownPayment();

    cout << endl << "  CAR LOAN INSTALLMENT PLAN" << endl;
    cout << "========================================" << endl;
    cout << "Car Details:" << endl;
    cout << "  Make: " << getMake() << endl;
    cout << "  Model: " << getModel() << endl;
    cout << "  Engine: " << getEngine() << "cc" << endl;
    cout << "  Condition: " << getUsed() << endl;
    cout << "  Year: " << getManufacturingYear() << endl;
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

        // For the last month, adjust payment to clear remaining balance
        if (month == getInstallments()) {
            paymentDue = currentBalance;
        }

        cout << "| " << setw(5) << month << " | PKR " << setw(10) << paymentDue
            << " | PKR " << setw(10) << (currentBalance - paymentDue) << " |" << endl;

        currentBalance -= paymentDue;
        if (currentBalance < 0) currentBalance = 0;
    }

    cout << "+-------+----------------+---------------+" << endl << endl;

    // Display summary
    long long totalPayment = getDownPayment() + (monthlyInstallment * getInstallments());
    long long totalInstallments = monthlyInstallment * getInstallments();

    cout << "Payment Summary:" << endl;
    cout << "  Total Payment: PKR " << totalPayment << endl;
    cout << "  Down Payment: PKR " << getDownPayment() << endl;
    cout << "  Total Installments: PKR " << totalInstallments << endl;
}

/// <summary>
/// Loads car loan data from file
/// </summary>
int loadCarLoans(CarLoan loans[], int maxSize, const string& filename) {
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

        if (tokens.size() == 8) {
            try {
                loans[count].setMake(tokens[0]);
                loans[count].setModel(tokens[1]);
                loans[count].setEngine(tokens[2]);
                loans[count].setUsed(tokens[3]);
                loans[count].setManufacturingYear(tokens[4]);
                loans[count].setInstallments(stoi(removeCommas(tokens[5])));
                loans[count].setPrice(stoll(removeCommas(tokens[6])));
                loans[count].setDownPayment(stoll(removeCommas(tokens[7])));

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
/// Displays car loan options
/// </summary>
void displayCarLoanOptionsTable(const CarLoan loans[], int size, const string& makeNumber) {
    string makeName = "Make " + makeNumber;

    cout << endl << "  Car Loan Options for " << makeName << endl;
    cout << "+-----+-----------+---------+----------+-------------+---------------+" << endl;
    cout << "| Opt |   Model   | Engine  | Condition| Total Price | Down Payment  |" << endl;
    cout << "+-----+-----------+---------+----------+-------------+---------------+" << endl;

    int optionCount = 0;
    for (int i = 0; i < size; i++) {
        if (loans[i].getMake() == makeName) {
            optionCount++;
            cout << "| " << setw(3) << optionCount << " | "
                << setw(9) << loans[i].getModel() << " | "
                << setw(7) << loans[i].getEngine() << " | "
                << setw(8) << loans[i].getUsed() << " | "
                << "PKR " << setw(8) << loans[i].getPrice() << " | "
                << "PKR " << setw(10) << loans[i].getDownPayment() << " |" << endl;
        }
    }
    cout << "+-----+-----------+---------+----------+-------------+---------------+" << endl;
}

/// <summary>
/// Displays installment plan for specific car option
/// </summary>
bool displayCarInstallmentPlan(const CarLoan loans[], int size, int optionNumber) {
    if (optionNumber < 1 || optionNumber > size) {
        cout << "Invalid option number. Please try again." << endl;
        return false;
    }

    loans[optionNumber - 1].displayInstallmentPlan();
    return true;
}