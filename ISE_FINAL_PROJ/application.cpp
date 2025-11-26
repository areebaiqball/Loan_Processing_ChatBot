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
/// Default constructor for LoanApplication
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
    loanType = "";
    loanCategory = "";
    loanAmount = 0;
    downPayment = 0;
    installmentMonths = 0;
    monthlyPayment = 0;
    installmentStartMonth = 0;
    installmentStartYear = 0;
    rejectionReason = ""; 
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
string LoanApplication::getLoanType() const { return loanType; }
string LoanApplication::getLoanCategory() const { return loanCategory; }
long long LoanApplication::getLoanAmount() const { return loanAmount; }
long long LoanApplication::getDownPayment() const { return downPayment; }
int LoanApplication::getInstallmentMonths() const { return installmentMonths; }
long long LoanApplication::getMonthlyPayment() const { return monthlyPayment; }
int LoanApplication::getInstallmentStartMonth() const { return installmentStartMonth; }
void LoanApplication::setInstallmentStartMonth(int month) { installmentStartMonth = month; }
int LoanApplication::getInstallmentStartYear() const { return installmentStartYear; }
void LoanApplication::setInstallmentStartYear(int year) { installmentStartYear = year; }

// Setters implementation 
void LoanApplication::setApplicationId(const string& id) {
    if (id.empty()) {
        throw ValidationException("Application ID cannot be empty");
    }
    applicationId = id;
}

void LoanApplication::setStatus(const string& stat) {
    // Allow both final statuses and checkpoint statuses
    vector<string> validStatuses = {
        "submitted", "approved", "rejected",
        "C1", "C2", "C3",
        "incomplete_personal", "incomplete_financial",
        "incomplete_references", "incomplete_documents"
    };

    bool valid = false;
    for (const auto& s : validStatuses) {
        if (s == stat) {
            valid = true;
            break;
        }
    }

    if (!valid) {
        throw ValidationException("Invalid status value: " + stat);
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

void LoanApplication::setLoanType(const string& type) { loanType = type; }
void LoanApplication::setLoanCategory(const string& category) { loanCategory = category; }
void LoanApplication::setLoanAmount(long long amount) { loanAmount = amount; }
void LoanApplication::setDownPayment(long long payment) { downPayment = payment; }
void LoanApplication::setInstallmentMonths(int months) { installmentMonths = months; }
void LoanApplication::setMonthlyPayment(long long payment) { monthlyPayment = payment; }

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
/// <returns>Number of existing loans</returns>
int LoanApplication::getExistingLoansCount() const {
    return existingLoans.size();
}

/// <summary>
/// Validates existing loans data
/// </summary>
/// <returns>Validation result object</returns>
ValidationResult LoanApplication::validateExistingLoans() const {
    ValidationResult result;

    for (size_t i = 0; i < existingLoans.size(); i++) {
        const ExistingLoan& loan = existingLoans[i];

        if (loan.amountReturned + loan.amountDue != loan.totalAmount) {
            result.addError("Loan " + to_string(i + 1) + ": Amount returned + amount due should equal total amount");
        }

        if (loan.amountReturned > loan.totalAmount) {
            result.addError("Loan " + to_string(i + 1) + ": Amount returned cannot exceed total loan amount");
        }

        if (loan.amountDue > loan.totalAmount) {
            result.addError("Loan " + to_string(i + 1) + ": Amount due cannot exceed total loan amount");
        }

        if (!loan.validate()) {
            result.addError("Loan " + to_string(i + 1) + ": Invalid loan data");
        }
    }

    return result;
}



/// <summary>
/// Validates debt-to-income ratio
/// </summary>
/// <param name="result">Validation result to update</param>
/// <returns>True if ratio is valid, false otherwise</returns>
bool LoanApplication::validateDebtToIncomeRatio(ValidationResult& result) const {
    if (annualIncome <= 0) {
        result.addError("Annual income must be positive to calculate debt ratio");
        return false;
    }

    long long totalExistingDebt = 0;
    for (const auto& loan : existingLoans) {
        if (loan.isActive) {
            totalExistingDebt += loan.amountDue;
        }
    }

    double debtRatio = static_cast<double>(totalExistingDebt) / annualIncome;

    if (debtRatio > 0.5) {
        result.addWarning("High debt-to-income ratio: " + to_string(static_cast<int>(debtRatio * 100)) + "%");
        return false;
    }

    return true;
}

/// <summary>
/// Validates complete application data
/// </summary>
/// <returns>Validation result object</returns>
ValidationResult LoanApplication::validateCompleteApplication() const {
    ValidationResult result;

    if (fullName.empty()) result.addError("Full name is required");
    if (cnicNumber.empty()) result.addError("CNIC is required");
    if (contactNumber.empty()) result.addError("Contact number is required");

    ValidationResult loansValidation = validateExistingLoans();
    if (!loansValidation.isValid) {
        for (const auto& error : loansValidation.errors) {
            result.addError(error);
        }
    }

    ValidationResult refsValidation = validateReferences();
    if (!refsValidation.isValid) {
        for (const auto& error : refsValidation.errors) {
            result.addError(error);
        }
    }

    validateDebtToIncomeRatio(result);

    return result;
}

/// <summary>
/// Validates employment and financial information
/// </summary>
/// <returns>Validation result object</returns>
ValidationResult LoanApplication::validateEmploymentAndFinancialInfo() const {
    ValidationResult result;

    if (employmentStatus.empty()) {
        result.addError("Employment status is required");
    }

    if (employmentStatus == "Unemployed" && annualIncome > 0) {
        result.addWarning("Applicant is unemployed but has reported income");
    }

    if (employmentStatus != "Unemployed" && annualIncome < 200000) {
        result.addWarning("Reported income seems low for employment status");
    }

    if (avgElectricityBill < 0 || currentElectricityBill < 0) {
        result.addError("Electricity bills cannot be negative");
    }

    if (currentElectricityBill > avgElectricityBill * 3) {
        result.addWarning("Current electricity bill is unusually high compared to average");
    }

    if (numberOfDependents < 0) {
        result.addError("Number of dependents cannot be negative");
    }

    if (numberOfDependents > 10 && annualIncome < 1000000) {
        result.addWarning("Many dependents with relatively low income");
    }

    return result;
}

/// <summary>
/// Validates income for specific loan type
/// </summary>
/// <param name="loanType">Type of loan</param>
/// <param name="loanAmount">Loan amount to validate</param>
/// <param name="result">Validation result to update</param>
/// <returns>True if income is sufficient, false otherwise</returns>
bool LoanApplication::validateIncomeForLoanType(const string& loanType, long long loanAmount, ValidationResult& result) const {
    if (annualIncome <= 0) {
        result.addError("Annual income must be positive");
        return false;
    }

    double incomeToLoanRatio = static_cast<double>(loanAmount) / annualIncome;

    if (loanType == "home" && incomeToLoanRatio > 5.0) {
        result.addError("Home loan amount cannot exceed 5 times annual income");
        return false;
    }
    else if (loanType == "car" && incomeToLoanRatio > 2.0) {
        result.addError("Car loan amount cannot exceed 2 times annual income");
        return false;
    }
    else if (loanType == "personal" && incomeToLoanRatio > 1.0) {
        result.addError("Personal loan amount cannot exceed annual income");
        return false;
    }

    if (employmentStatus == "Unemployed" && loanAmount > 500000) {
        result.addError("Loan amount too high for unemployed applicant");
        return false;
    }

    if (employmentStatus == "Retired" && loanAmount > 1000000) {
        result.addWarning("Large loan amount for retired applicant");
    }

    return true;
}
string LoanApplication::getRejectionReason() const {
    return rejectionReason;
}

void LoanApplication::setRejectionReason(const string& reason) {
    rejectionReason = reason;
}


bool LoanApplication::isSectionCompleted(const string& section) const {
    return completedSections.find(section) != string::npos;
}

vector<string> LoanApplication::getIncompleteSections() const {
    vector<string> sections = { "personal", "financial", "references", "documents" };
    vector<string> incomplete;

    for (const auto& section : sections) {
        if (!isSectionCompleted(section)) {
            incomplete.push_back(section);
        }
    }

    return incomplete;
}



string LoanApplication::getCompletedSections() const {
    return completedSections;
}

void LoanApplication::setCompletedSections(const string& sections) {
    completedSections = sections;
}

// Validation methods for each section
ValidationResult LoanApplication::validatePersonalInfo() const {
    ValidationResult result;

    if (fullName.empty()) result.addError("Full name is required");
    if (fathersName.empty()) result.addError("Father's name is required");
    if (postalAddress.empty()) result.addError("Postal address is required");
    if (contactNumber.empty()) result.addError("Contact number is required");
    if (emailAddress.empty()) result.addError("Email address is required");
    if (cnicNumber.empty()) result.addError("CNIC is required");
    if (cnicExpiryDate.empty()) result.addError("CNIC expiry date is required");
    if (employmentStatus.empty()) result.addError("Employment status is required");
    if (maritalStatus.empty()) result.addError("Marital status is required");
    if (gender.empty()) result.addError("Gender is required");
    if (numberOfDependents < 0) result.addError("Number of dependents cannot be negative");

    // CNIC validation
    if (cnicNumber.length() != 13) {
        result.addError("CNIC must be exactly 13 digits");
    }
    else {
        for (char c : cnicNumber) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                result.addError("CNIC must contain only digits");
                break;
            }
        }
    }

    // Email validation
    if (emailAddress.find('@') == string::npos || emailAddress.find('.') == string::npos) {
        result.addError("Invalid email format");
    }

    return result;
}

ValidationResult LoanApplication::validateFinancialInfo() const {
    ValidationResult result;

    if (annualIncome < 0) result.addError("Annual income cannot be negative");
    if (avgElectricityBill < 0) result.addError("Average electricity bill cannot be negative");
    if (currentElectricityBill < 0) result.addError("Current electricity bill cannot be negative");

    if (annualIncome > 1000000000) result.addWarning("Annual income seems unusually high");
    if (avgElectricityBill > 100000) result.addWarning("Average electricity bill seems unusually high");
    if (currentElectricityBill > 100000) result.addWarning("Current electricity bill seems unusually high");

    // Validate existing loans
    for (size_t i = 0; i < existingLoans.size(); i++) {
        const ExistingLoan& loan = existingLoans[i];
        if (!loan.validate()) {
            result.addError("Invalid data in existing loan #" + to_string(i + 1));
        }
    }

    return result;
}

ValidationResult LoanApplication::validateReferences() const {
    ValidationResult result;

    if (!reference1.validate()) {
        result.addError("Invalid data for reference 1");
    }
    if (!reference2.validate()) {
        result.addError("Invalid data for reference 2");
    }
    if (reference1.cnic == reference2.cnic) {
        result.addError("References must have different CNIC numbers");
    }

    return result;
}

ValidationResult LoanApplication::validateDocuments() const {
    ValidationResult result;

    if (cnicFrontImagePath.empty()) result.addError("CNIC front image is required");
    if (cnicBackImagePath.empty()) result.addError("CNIC back image is required");
    if (electricityBillImagePath.empty()) result.addError("Electricity bill image is required");
    if (salarySlipImagePath.empty()) result.addError("Salary slip or bank statement is required");

    return result;
}
ValidationResult LoanApplication::validateForLoanType(const string& loanType, long long loanAmount) const {
    ValidationResult result;

    if (!validateIncomeForLoanType(loanType, loanAmount, result)) {
        result.addError("Income insufficient for requested loan amount");
    }

    if (!validateDebtToIncomeRatio(result)) {
        result.addWarning("High debt-to-income ratio detected");
    }

    return result;
}
bool LoanApplication::validateIncomeToLoanRatio(long long loanAmount, ValidationResult& result) const {
    if (annualIncome <= 0) {
        result.addError("Annual income must be positive");
        return false;
    }

    double ratio = static_cast<double>(loanAmount) / annualIncome;

    if (ratio > 5.0) {
        result.addError("Loan amount cannot exceed 5 times annual income");
        return false;
    }

    if (ratio > 3.0) {
        result.addWarning("Loan amount is high relative to income");
    }

    return true;
}


bool LoanApplication::isApplicationComplete() const {
    vector<string> completeStatuses = { "submitted", "approved", "rejected" };
    for (const auto& s : completeStatuses) {
        if (status == s) return true;
    }
    return false;
}

string LoanApplication::getNextIncompleteSection() const {
    if (status == "C1" || status == "incomplete_personal" || !isSectionCompleted("personal")) {
        return "personal";
    }
    if (status == "C2" || status == "incomplete_financial" || !isSectionCompleted("financial")) {
        return "financial";
    }
    if (status == "C3" || !isSectionCompleted("references")) {
        return "references";
    }
    if (status == "incomplete_documents" || !isSectionCompleted("documents")) {
        return "documents";
    }
    return "complete";
}
/// <summary>
/// Marks a section as completed and updates the application status
/// </summary>
/// <param name="section">The section to mark as completed</param>
void LoanApplication::markSectionCompleted(const string& section) {
    if (!isSectionCompleted(section)) {
        if (!completedSections.empty()) {
            completedSections += ",";
        }
        completedSections += section;
    }

    // Update status based on checkpoints
    if (section == "personal") {
        status = "C2"; // Checkpoint 2 - Financial info needed
    }
    else if (section == "financial") {
        status = "C3"; // Checkpoint 3 - References needed
    }
    else if (section == "references") {
        status = "incomplete_documents"; // Documents needed
    }
    else if (section == "documents") {
        status = "submitted"; // Fully submitted
        if (submissionDate.empty()) {
            submissionDate = getCurrentDate();
        }
    }
}