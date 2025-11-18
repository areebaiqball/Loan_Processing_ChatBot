#include "scooter.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>

// ScooterLoan class implementation
ScooterLoan::ScooterLoan() {
    make = "";
    model = "";
    distancePerCharge = "";
    chargingTime = "";
    maxSpeed = "";
    installments = 0;
    price = 0;
    downPayment = 0;
}

ScooterLoan::ScooterLoan(const string& mk, const string& mdl, const string& dist, const string& chargeTime,
    const string& speed, int inst, long long p, long long dp) {
    if (inst < 0 || p < 0 || dp < 0) {
        throw invalid_argument("Loan values cannot be negative");
    }
    make = mk;
    model = mdl;
    distancePerCharge = dist;
    chargingTime = chargeTime;
    maxSpeed = speed;
    installments = inst;
    price = p;
    downPayment = dp;
}

string ScooterLoan::getMake() const { return make; }
string ScooterLoan::getModel() const { return model; }
string ScooterLoan::getDistancePerCharge() const { return distancePerCharge; }
string ScooterLoan::getChargingTime() const { return chargingTime; }
string ScooterLoan::getMaxSpeed() const { return maxSpeed; }
int ScooterLoan::getInstallments() const { return installments; }
long long ScooterLoan::getPrice() const { return price; }
long long ScooterLoan::getDownPayment() const { return downPayment; }

void ScooterLoan::setMake(const string& mk) { make = mk; }
void ScooterLoan::setModel(const string& mdl) { model = mdl; }
void ScooterLoan::setDistancePerCharge(const string& dist) { distancePerCharge = dist; }
void ScooterLoan::setChargingTime(const string& chargeTime) { chargingTime = chargeTime; }
void ScooterLoan::setMaxSpeed(const string& speed) { maxSpeed = speed; }

void ScooterLoan::setInstallments(int inst) {
    if (inst < 0) {
        throw invalid_argument("Installments cannot be negative");
    }
    installments = inst;
}

void ScooterLoan::setPrice(long long p) {
    if (p < 0) {
        throw invalid_argument("Price cannot be negative");
    }
    price = p;
}

void ScooterLoan::setDownPayment(long long dp) {
    if (dp < 0) {
        throw invalid_argument("Down payment cannot be negative");
    }
    downPayment = dp;
}

long long ScooterLoan::calculateMonthlyInstallment() const {
    if (installments <= 0) return 0;
    long long remainingAmount = (price - downPayment);
    return remainingAmount / installments;
}

void ScooterLoan::displayInstallmentPlan() const {
    long long monthlyInstallment = calculateMonthlyInstallment();
    long long remainingBalance = getPrice() - getDownPayment();

    cout << endl << "  SCOOTER LOAN INSTALLMENT PLAN" << endl;
    cout << "========================================" << endl;
    cout << "Scooter Details:" << endl;
    cout << "  Make: " << getMake() << endl;
    cout << "  Model: " << getModel() << endl;
    cout << "  Range: " << getDistancePerCharge() << " KM per charge" << endl;
    cout << "  Charging Time: " << getChargingTime() << " hours" << endl;
    cout << "  Max Speed: " << getMaxSpeed() << " KM/H" << endl;
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
/// Loads scooter loan data from file
/// </summary>
int loadScooterLoans(ScooterLoan loans[], int maxSize, const string& filename) {
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
                loans[count].setDistancePerCharge(tokens[2]);
                loans[count].setChargingTime(tokens[3]);
                loans[count].setMaxSpeed(tokens[4]);
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
/// Displays scooter loan options in tabular format
/// </summary>
bool displayScooterLoanOptionsTable(const ScooterLoan loans[], int size, const string& makeNumber) {
    string makeName = "Make " + makeNumber;
    bool found = false;

    cout << endl << "  Scooter Loan Options for " << makeName << endl;
    cout << "+-----+-----------+------------------+---------------+-------------+---------------+---------------------+" << endl;
    cout << "| Opt |   Model   | Range per Charge | Charging Time | Max Speed   | Total Price   | Down Payment        |" << endl;
    cout << "+-----+-----------+------------------+---------------+-------------+---------------+---------------------+" << endl;

    int optionCount = 0;
    for (int i = 0; i < size; i++) {
        if (loans[i].getMake() == makeName) {
            found = true;
            optionCount++;
            cout << "| " << setw(3) << optionCount << " | "
                << setw(9) << loans[i].getModel() << " | "
                << setw(16) << loans[i].getDistancePerCharge() << " KM | "
                << setw(13) << loans[i].getChargingTime() << " Hrs | "
                << setw(11) << loans[i].getMaxSpeed() << " KM/H | "
                << "PKR " << setw(8) << loans[i].getPrice() << " | "
                << "PKR " << setw(15) << loans[i].getDownPayment() << " |" << endl;
        }
    }

    cout << "+-----+-----------+------------------+---------------+-------------+---------------+---------------------+" << endl;

    // Display monthly installments in a separate table
    if (found) {
        cout << endl << "Monthly Installments:" << endl;
        cout << "+-----+---------------------+" << endl;
        cout << "| Opt | Monthly Installment |" << endl;
        cout << "+-----+---------------------+" << endl;

        optionCount = 0;
        for (int i = 0; i < size; i++) {
            if (loans[i].getMake() == makeName) {
                optionCount++;
                long long monthlyInstallment = loans[i].calculateMonthlyInstallment();
                cout << "| " << setw(3) << optionCount << " | "
                    << "PKR " << setw(15) << monthlyInstallment << " |" << endl;
            }
        }
        cout << "+-----+---------------------+" << endl;
    }

    if (!found) {
        cout << "No scooter options available for this make." << endl;
    }

    return found;
}
/// <summary>
/// Displays installment plan for specific scooter option
/// </summary>
bool displayScooterInstallmentPlan(const ScooterLoan loans[], int size, int optionNumber) {
    if (optionNumber < 1 || optionNumber > size) {
        cout << "Invalid option number. Please try again." << endl;
        return false;
    }

    loans[optionNumber - 1].displayInstallmentPlan();
    return true;
}