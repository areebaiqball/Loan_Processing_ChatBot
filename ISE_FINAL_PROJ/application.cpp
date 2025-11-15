#include "application.h"
#include "utilities.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

/// <summary>
/// Default constructor 
/// </summary>
LoanApplication::LoanApplication() {
    applicationId = "";
    status = "submitted";
    submissionDate = "";
    fullName = "";
    fathersName = "";
    postalAddress = "";
    contactNumber = "";
    emailAddress = "";
    cnicNumber = "";
    cnicExpiryDate = "";
    employmentStatus = "";
    maritalStatus = "";
    gender = "";
    numberOfDependents = 0;
    annualIncome = 0;
    avgElectricityBill = 0;
    currentElectricityBill = 0;
    cnicFrontImagePath = "";
    cnicBackImagePath = "";
    electricityBillImagePath = "";
    salarySlipImagePath = "";
}

// Getters implementation
string LoanApplication::getApplicationId() const { return applicationId; }
string LoanApplication::getStatus() const { return status; }
string LoanApplication::getSubmissionDate() const { return submissionDate; }
string LoanApplication::getFullName() const { return fullName; }
string LoanApplication::getFathersName() const { return fathersName; }
string LoanApplication::getPostalAddress() const { return postalAddress; }
string LoanApplication::getContactNumber() const { return contactNumber; }
string LoanApplication::getEmailAddress() const { return emailAddress; }
string LoanApplication::getCnicNumber() const { return cnicNumber; }
string LoanApplication::getCnicExpiryDate() const { return cnicExpiryDate; }
string LoanApplication::getEmploymentStatus() const { return employmentStatus; }
string LoanApplication::getMaritalStatus() const { return maritalStatus; }
string LoanApplication::getGender() const { return gender; }
int LoanApplication::getNumberOfDependents() const { return numberOfDependents; }
long long LoanApplication::getAnnualIncome() const { return annualIncome; }
long long LoanApplication::getAvgElectricityBill() const { return avgElectricityBill; }
long long LoanApplication::getCurrentElectricityBill() const { return currentElectricityBill; }
vector<ExistingLoan> LoanApplication::getExistingLoans() const { return existingLoans; }
Reference LoanApplication::getReference1() const { return reference1; }
Reference LoanApplication::getReference2() const { return reference2; }
string LoanApplication::getCnicFrontImagePath() const { return cnicFrontImagePath; }
string LoanApplication::getCnicBackImagePath() const { return cnicBackImagePath; }
string LoanApplication::getElectricityBillImagePath() const { return electricityBillImagePath; }
string LoanApplication::getSalarySlipImagePath() const { return salarySlipImagePath; }

// Setters implementation 
void LoanApplication::setApplicationId(const string& id) {
    if (id.empty()) {
        throw ValidationException("Application ID cannot be empty");
    }
    applicationId = id;
}

void LoanApplication::setStatus(const string& stat) {
    if (stat != "submitted" && stat != "approved" && stat != "rejected") {
        throw ValidationException("Invalid status value");
    }
    status = stat;
}

void LoanApplication::setSubmissionDate(const string& date) {
    if (date.length() != 10 || date[2] != '-' || date[5] != '-') {
        throw ValidationException("Submission date must be in DD-MM-YYYY format");
    }
    submissionDate = date;
}

void LoanApplication::setFullName(const string& name) {
    if (name.empty()) {
        throw ValidationException("Full name cannot be empty");
    }
    if (name.length() > 100) {
        throw ValidationException("Full name too long");
    }
    fullName = name;
}

void LoanApplication::setFathersName(const string& name) {
    if (name.empty()) {
        throw ValidationException("Father's name cannot be empty");
    }
    fathersName = name;
}

void LoanApplication::setPostalAddress(const string& address) {
    if (address.empty()) {
        throw ValidationException("Postal address cannot be empty");
    }
    postalAddress = address;
}

void LoanApplication::setContactNumber(const string& number) {
    if (number.length() < 10 || number.length() > 15) {
        throw ValidationException("Phone number must be 10-15 digits");
    }
    contactNumber = number;
}

void LoanApplication::setEmailAddress(const string& email) {
    if (email.find('@') == string::npos || email.find('.') == string::npos) {
        throw ValidationException("Invalid email format");
    }
    emailAddress = email;
}

void LoanApplication::setCnicNumber(const string& cnic) {
    if (cnic.length() != 13) {
        throw ValidationException("CNIC must be 13 digits");
    }
    for (char c : cnic) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            throw ValidationException("CNIC must contain only digits");
        }
    }
    cnicNumber = cnic;
}

void LoanApplication::setCnicExpiryDate(const string& date) {
    if (date.length() != 10 || date[2] != '-' || date[5] != '-') {
        throw ValidationException("CNIC expiry date must be in DD-MM-YYYY format");
    }
    cnicExpiryDate = date;
}

void LoanApplication::setEmploymentStatus(const string& status) {
    vector<string> validStatuses = { "Self-employed", "Salaried", "Retired", "Unemployed" };
    bool valid = false;
    for (const auto& s : validStatuses) {
        if (s == status) {
            valid = true;
            break;
        }
    }
    if (!valid) throw ValidationException("Invalid employment status");
    employmentStatus = status;
}

void LoanApplication::setMaritalStatus(const string& status) {
    vector<string> validStatuses = { "Single", "Married", "Divorced", "Widowed" };
    bool valid = false;
    for (const auto& s : validStatuses) {
        if (s == status) {
            valid = true;
            break;
        }
    }
    if (!valid) throw ValidationException("Invalid marital status");
    maritalStatus = status;
}

void LoanApplication::setGender(const string& gen) {
    vector<string> validGenders = { "Male", "Female", "Other" };
    bool valid = false;
    for (const auto& g : validGenders) {
        if (g == gen) {
            valid = true;
            break;
        }
    }
    if (!valid) throw ValidationException("Invalid gender");
    gender = gen;
}

void LoanApplication::setNumberOfDependents(int dependents) {
    if (dependents < 0) {
        throw ValidationException("Number of dependents cannot be negative");
    }
    if (dependents > 20) {
        throw ValidationException("Number of dependents too high");
    }
    numberOfDependents = dependents;
}

void LoanApplication::setAnnualIncome(long long income) {
    if (income < 0) {
        throw ValidationException("Annual income cannot be negative");
    }
    annualIncome = income;
}

void LoanApplication::setAvgElectricityBill(long long amount) {
    if (amount < 0) {
        throw ValidationException("Average electricity bill cannot be negative");
    }
    avgElectricityBill = amount;
}

void LoanApplication::setCurrentElectricityBill(long long amount) {
    if (amount < 0) {
        throw ValidationException("Current electricity bill cannot be negative");
    }
    currentElectricityBill = amount;
}

void LoanApplication::setReference1(const Reference& ref) {
    if (ref.name.empty()) {
        throw ValidationException("Reference 1 name cannot be empty");
    }
    reference1 = ref;
}

void LoanApplication::setReference2(const Reference& ref) {
    if (ref.name.empty()) {
        throw ValidationException("Reference 2 name cannot be empty");
    }
    reference2 = ref;
}

void LoanApplication::setCnicFrontImagePath(const string& path) {
    cnicFrontImagePath = path;
}

void LoanApplication::setCnicBackImagePath(const string& path) {
    cnicBackImagePath = path;
}

void LoanApplication::setElectricityBillImagePath(const string& path) {
    electricityBillImagePath = path;
}

void LoanApplication::setSalarySlipImagePath(const string& path) {
    salarySlipImagePath = path;
}

/// <summary>
/// Adds an existing loan to the application
/// </summary>
void LoanApplication::addExistingLoan(const ExistingLoan& loan) {
    existingLoans.push_back(loan);
}

/// <summary>
/// Clears all existing loans from the application
/// </summary>
void LoanApplication::clearExistingLoans() {
    existingLoans.clear();
}

/// <summary>
/// Gets the count of existing loans
/// </summary>
int LoanApplication::getExistingLoansCount() const {
    return existingLoans.size();
}

