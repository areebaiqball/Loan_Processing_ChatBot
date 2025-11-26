#include "personal.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <vector>

using namespace std;

PersonalLoan::PersonalLoan() {
    category = "";
    purpose = "";
    description = "";
    installments = 0;
    amount = 0;
    downPayment = 0;
}

PersonalLoan::PersonalLoan(const string& cat, const string& purp, const string& desc,
    int inst, long long amt, long long dp) {
    if (inst < 0 || amt < 0 || dp < 0) {
        throw invalid_argument("Loan values cannot be negative");
    }
    category = cat;
    purpose = purp;
    description = desc;
    installments = inst;
    amount = amt;
    downPayment = dp;
}

string PersonalLoan::getCategory() const { return category; }
string PersonalLoan::getPurpose() const { return purpose; }
string PersonalLoan::getDescription() const { return description; }
int PersonalLoan::getInstallments() const { return installments; }
long long PersonalLoan::getAmount() const { return amount; }
long long PersonalLoan::getDownPayment() const { return downPayment; }

void PersonalLoan::setCategory(const string& cat) { category = cat; }
void PersonalLoan::setPurpose(const string& purp) { purpose = purp; }
void PersonalLoan::setDescription(const string& desc) { description = desc; }

void PersonalLoan::setInstallments(int inst) {
    if (inst < 0) {
        throw invalid_argument("Installments cannot be negative");
    }
    installments = inst;
}

void PersonalLoan::setAmount(long long amt) {
    if (amt < 0) {
        throw invalid_argument("Amount cannot be negative");
    }
    amount = amt;
}

void PersonalLoan::setDownPayment(long long dp) {
    if (dp < 0) {
        throw invalid_argument("Down payment cannot be negative");
    }
    downPayment = dp;
}

long long PersonalLoan::calculateMonthlyInstallment() const {
    if (installments <= 0) return 0;
    long long remainingAmount = (amount - downPayment);
    return remainingAmount / installments;
}

void PersonalLoan::displayInstallmentPlan() const {
    long long monthlyInstallment = calculateMonthlyInstallment();
    long long remainingBalance = getAmount() - getDownPayment();

    cout << endl << "  PERSONAL LOAN INSTALLMENT PLAN" << endl;
    cout << "========================================" << endl;
    cout << "Loan Details:" << endl;
    cout << "  Category: " << getCategory() << endl;
    cout << "  Purpose: " << getPurpose() << endl;
    cout << "  Description: " << getDescription() << endl;
    cout << "  Total Amount: PKR " << getAmount() << endl;
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

int loadPersonalLoans(PersonalLoan loans[], int maxSize, const string& filename) {
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

        if (tokens.size() == 6) {
            try {
                loans[count].setCategory(tokens[0]);
                loans[count].setPurpose(tokens[1]);
                loans[count].setDescription(tokens[2]);
                loans[count].setInstallments(stoi(removeCommas(tokens[3])));
                loans[count].setAmount(stoll(removeCommas(tokens[4])));
                loans[count].setDownPayment(stoll(removeCommas(tokens[5])));

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

vector<string> getPersonalLoanCategories(const PersonalLoan loans[], int size) {
    vector<string> categories;

    for (int i = 0; i < size; i++) {
        string cat = loans[i].getCategory();

        // Manually check if category already exists (no algorithm library)
        bool found = false;
        for (size_t j = 0; j < categories.size(); j++) {
            if (categories[j] == cat) {
                found = true;
                break;
            }
        }

        if (!found) {
            categories.push_back(cat);
        }
    }

    // Manual bubble sort (no algorithm library)
    for (size_t i = 0; i < categories.size(); i++) {
        for (size_t j = i + 1; j < categories.size(); j++) {
            if (categories[i] > categories[j]) {
                string temp = categories[i];
                categories[i] = categories[j];
                categories[j] = temp;
            }
        }
    }

    return categories;
}

bool displayPersonalLoanOptionsByCategory(const PersonalLoan loans[], int size,
    const string& categoryIndex) {

    vector<string> categories = getPersonalLoanCategories(loans, size);

    int catNum = 0;
    try {
        catNum = stoi(categoryIndex);
    }
    catch (...) {
        cout << "Invalid category selection." << endl;
        return false;
    }

    if (catNum < 1 || catNum > static_cast<int>(categories.size())) {
        cout << "Invalid category number." << endl;
        return false;
    }

    string selectedCategory = categories[catNum - 1];
    bool found = false;

    cout << endl << "  Personal Loan Options for: " << selectedCategory << endl;
    cout << "+-----+------------------+---------------------+---------------+-------------+---------------+" << endl;
    cout << "| Opt | Purpose          | Description         | Installments  | Amount (PKR)| Down Payment  |" << endl;
    cout << "+-----+------------------+---------------------+---------------+-------------+---------------+" << endl;

    int optionCount = 0;
    for (int i = 0; i < size; i++) {
        if (loans[i].getCategory() == selectedCategory) {
            found = true;
            optionCount++;

            string purpose = loans[i].getPurpose();
            string description = loans[i].getDescription();

            if (purpose.length() > 16) purpose = purpose.substr(0, 13) + "...";
            if (description.length() > 19) description = description.substr(0, 16) + "...";

            cout << "| " << setw(3) << optionCount << " | "
                << setw(16) << left << purpose << " | "
                << setw(19) << left << description << " | "
                << setw(13) << loans[i].getInstallments() << " | "
                << setw(11) << loans[i].getAmount() << " | "
                << "PKR " << setw(10) << loans[i].getDownPayment() << " |" << endl;
        }
    }
    cout << "+-----+------------------+---------------------+---------------+-------------+---------------+" << endl;

    if (!found) {
        cout << "No personal loan options available for this category." << endl;
    }

    return found;
}

bool displayPersonalInstallmentPlan(const PersonalLoan loans[], int size,
    int optionNumber) {

    if (optionNumber < 1 || optionNumber > size) {
        cout << "Invalid option number. Please try again." << endl;
        return false;
    }

    loans[optionNumber - 1].displayInstallmentPlan();
    return true;
}