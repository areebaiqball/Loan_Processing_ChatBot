#ifndef UI_HANDLERS_H
#define UI_HANDLERS_H

#include "application.h"
#include "home.h"
#include "car.h"
#include "scooter.h"
#include "file_manager.h"
#include "utilities.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// ============================================
// SHARED UI FUNCTIONS - Used by both files
// ============================================

inline void displayApplicationDetails(const LoanApplication& app) {
    cout << endl << "========================================" << endl;
    cout << "APPLICATION DETAILS" << endl;
    cout << "========================================" << endl;
    cout << "Application ID: " << app.getApplicationId() << endl;
    cout << "Status: " << app.getStatus() << endl;

    if (app.getStatus() == "rejected" && !app.getRejectionReason().empty()) {
        cout << "Rejection Reason: " << app.getRejectionReason() << endl;
    }

    cout << "Submission Date: " << app.getSubmissionDate() << endl;
    cout << endl << "PERSONAL INFORMATION:" << endl;
    cout << "  Full Name: " << app.getFullName() << endl;
    cout << "  Father's Name: " << app.getFathersName() << endl;
    cout << "  CNIC: " << app.getCnicNumber() << endl;
    cout << "  Contact: " << app.getContactNumber() << endl;
    cout << "  Email: " << app.getEmailAddress() << endl;
    cout << "  Address: " << app.getPostalAddress() << endl;
    cout << endl << "EMPLOYMENT & FINANCIAL:" << endl;
    cout << "  Employment: " << app.getEmploymentStatus() << endl;
    cout << "  Marital Status: " << app.getMaritalStatus() << endl;
    cout << "  Gender: " << app.getGender() << endl;
    cout << "  Dependents: " << app.getNumberOfDependents() << endl;
    cout << "  Annual Income: PKR " << app.getAnnualIncome() << endl;
    cout << "  Avg Electricity Bill: PKR " << app.getAvgElectricityBill() << endl;
    cout << "  Current Electricity Bill: PKR " << app.getCurrentElectricityBill() << endl;

    cout << endl << "LOAN DETAILS:" << endl;

    string loanType = app.getLoanType();
    if (loanType.empty() || loanType == "0" || loanType == "Unknown") {
        loanType = "Personal Loan";
    }
    cout << "  Loan Type: " << loanType << endl;

    cout << "  Category: " << app.getLoanCategory() << endl;
    cout << "  Total Amount: PKR " << app.getLoanAmount() << endl;
    cout << "  Down Payment: PKR " << app.getDownPayment() << endl;
    cout << "  Installment Period: " << app.getInstallmentMonths() << " months" << endl;
    cout << "  Monthly Payment: PKR " << app.getMonthlyPayment() << endl;

    cout << endl << "EXISTING LOANS: " << app.getExistingLoansCount() << " loan(s)" << endl;
    cout << "========================================" << endl;
}

inline void displayGoodbyeMessage() {
    cout << Config::CHATBOT_NAME << ": Thank you for using our service. Goodbye!" << endl;
}

inline bool isValidAreaInput(const string& input) {
    return (input == "1" || input == "2" || input == "3" || input == "4");
}

inline bool isValidMakeInput(const string& input, const string& loanType) {
    if (loanType == "car") return (input == "1" || input == "2");
    if (loanType == "scooter") return (input == "1");
    return false;
}

inline void displayCarMakes() {
    cout << Config::CHATBOT_NAME << ": Please select the car make:" << endl;
    cout << "  Press 1 for Make 1" << endl;
    cout << "  Press 2 for Make 2" << endl;
    cout << "  Press X to exit" << endl;
}

inline void displayScooterMakes() {
    cout << Config::CHATBOT_NAME << ": Please select the scooter make:" << endl;
    cout << "  Press 1 for Make 1" << endl;
    cout << "  Press X to exit" << endl;
}

inline bool displayCarLoanOptionsByMake(const CarLoan loans[], int size, const string& makeNumber) {
    string makeName = "Make " + makeNumber;
    bool found = false;

    cout << endl << "  Car Loan Options for " << makeName << endl;
    cout << "+-----+-----------+---------+----------+-------------+-------------+---------------+---------------------+" << endl;
    cout << "| Opt |   Model   | Engine  | Condition| Year        | Total Price | Down Payment  | Monthly Installment |" << endl;
    cout << "+-----+-----------+---------+----------+-------------+-------------+---------------+---------------------+" << endl;

    int optionCount = 0;
    for (int i = 0; i < size; i++) {
        if (loans[i].getMake() == makeName) {
            found = true;
            optionCount++;
            long long monthlyInstallment = loans[i].calculateMonthlyInstallment();
            cout << "| " << setw(3) << optionCount << " | "
                << setw(9) << loans[i].getModel() << " | "
                << setw(7) << loans[i].getEngine() << "cc | "
                << setw(8) << loans[i].getUsed() << " | "
                << setw(11) << loans[i].getManufacturingYear() << " | "
                << "PKR " << setw(8) << loans[i].getPrice() << " | "
                << "PKR " << setw(10) << loans[i].getDownPayment() << " | "
                << "PKR " << setw(15) << monthlyInstallment << " |" << endl;
        }
    }
    cout << "+-----+-----------+---------+----------+-------------+-------------+---------------+---------------------+" << endl;

    if (!found) {
        cout << "No car options available for this make." << endl;
    }

    return found;
}

inline void generateUserInstallmentPlan(const LoanApplication& application) {
    cout << endl << " " << Config::CHATBOT_NAME
        << ": Generating your monthly installment plan..." << endl;

    long long loanAmount = application.getLoanAmount();
    long long downPayment = application.getDownPayment();
    int totalMonths = application.getInstallmentMonths();
    long long monthlyPayment = application.getMonthlyPayment();
    string loanType = application.getLoanType();

    if (loanAmount <= 1000 || loanAmount > 1000000000000) {
        loanAmount = 2000000;
    }
    if (downPayment < 0 || downPayment >= loanAmount) {
        downPayment = (long long)(loanAmount * 0.1);
    }
    if (totalMonths <= 0 || totalMonths > 120) {
        totalMonths = 36;
    }
    if (monthlyPayment <= 0 || monthlyPayment > loanAmount) {
        monthlyPayment = (loanAmount - downPayment) / totalMonths;
    }
    if (loanType == "0" || loanType == "1" || loanType.empty()) {
        loanType = "Personal Loan";
    }

    int startMonth = 0;
    while (startMonth < 1 || startMonth > 12) {
        cout << Config::CHATBOT_NAME << ": When would you like to start payments?" << endl;
        cout << "Enter starting month (1=January, ..., 12=December): ";
        string monthInput;
        getline(cin, monthInput);

        try {
            startMonth = stoi(trim(monthInput));
            if (startMonth < 1 || startMonth > 12) {
                cout << " Please enter a valid month (1-12)." << endl;
            }
        }
        catch (...) {
            cout << " Please enter a number between 1 and 12." << endl;
            startMonth = 0;
        }
    }

    int startYear = 0;
    while (startYear < 2024 || startYear > 2100) {
        cout << Config::CHATBOT_NAME << ": Enter starting year (e.g., 2024, 2025): ";
        string yearInput;
        getline(cin, yearInput);

        try {
            startYear = stoi(trim(yearInput));
            if (startYear < 2024 || startYear > 2100) {
                cout << " Please enter a year between 2024 and 2100." << endl;
            }
        }
        catch (...) {
            cout << " Please enter a valid year." << endl;
            startYear = 0;
        }
    }

    long long financedAmount = loanAmount - downPayment;
    long long remainingBalance = financedAmount;

    cout << endl << "===============================================" << endl;
    cout << "         MONTHLY INSTALLMENT PLAN" << endl;
    cout << "===============================================" << endl;
    cout << "Applicant: " << application.getFullName() << endl;
    cout << "CNIC: " << application.getCnicNumber() << endl;
    cout << "Loan Type: " << loanType << endl;
    cout << "Total Loan Amount: PKR " << loanAmount << endl;
    cout << "Down Payment: PKR " << downPayment << endl;
    cout << "Amount to Finance: PKR " << financedAmount << endl;
    cout << "Installment Period: " << totalMonths << " months" << endl;
    cout << "Monthly Installment: PKR " << monthlyPayment << endl;
    cout << "Starting: " << getMonthName(startMonth) << " " << startYear << endl;
    cout << "===============================================" << endl << endl;

    cout << Config::CHATBOT_NAME << ": Here is your complete payment schedule:" << endl << endl;

    cout << "+-------+------------------+------+------------------+------------------+" << endl;
    cout << "| Month | Month Name       | Year | Payment Due      | Remaining Balance|" << endl;
    cout << "+-------+------------------+------+------------------+------------------+" << endl;

    int currentMonth = startMonth;
    int currentYear = startYear;
    long long totalPaid = 0;

    for (int monthNum = 1; monthNum <= totalMonths; monthNum++) {
        long long paymentDue = monthlyPayment;

        if (monthNum == totalMonths) {
            paymentDue = remainingBalance;
        }

        totalPaid += paymentDue;
        remainingBalance -= paymentDue;
        if (remainingBalance < 0) remainingBalance = 0;

        cout << "| " << setw(5) << monthNum << " | "
            << setw(16) << left << getMonthName(currentMonth) << " | "
            << setw(4) << currentYear << " | "
            << "PKR " << setw(12) << right << paymentDue << " | "
            << "PKR " << setw(12) << remainingBalance << " |" << endl;

        getNextMonth(currentMonth, currentYear);
    }

    cout << "+-------+------------------+------+------------------+------------------+" << endl;

    cout << endl << "===============================================" << endl;
    cout << "PAYMENT SUMMARY" << endl;
    cout << "===============================================" << endl;
    cout << "Total Loan Amount:    PKR " << setw(12) << loanAmount << endl;
    cout << "Down Payment:         PKR " << setw(12) << downPayment << endl;
    cout << "Amount Financed:      PKR " << setw(12) << financedAmount << endl;
    cout << "Total Installments:   PKR " << setw(12) << totalPaid << endl;
    cout << "Total Amount Paid:    PKR " << setw(12) << (downPayment + totalPaid) << endl;
    cout << "Completion Date:      " << getMonthName(currentMonth) << " " << currentYear << endl;
    cout << "===============================================" << endl;

    cout << endl << Config::CHATBOT_NAME << ": Your installment plan is ready! ";
    cout << "First payment of PKR " << monthlyPayment << " is due "
        << getMonthName(startMonth) << " " << startYear << "." << endl;
}

#endif // UI_HANDLERS_H