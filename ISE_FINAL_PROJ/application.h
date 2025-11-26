#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
using namespace std;

// SE PRINCIPLE: Exception Handling

/// <summary>
/// Base exception class for loan application errors
/// SE Principle: Encapsulation of error handling logic
/// </summary>
class LoanApplicationException : public exception {
private:
    string message;
public:
    LoanApplicationException(const string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

/// <summary>
/// Exception thrown when validation fails
/// </summary>
class ValidationException : public LoanApplicationException {
public:
    ValidationException(const string& msg) : LoanApplicationException("Validation Error: " + msg) {}
};


/// <summary>
/// Stores validation results with errors and warnings
/// </summary>
struct ValidationResult {
    bool isValid;
    vector<string> errors;
    vector<string> warnings;

    ValidationResult() : isValid(true) {}

    /// <summary>
    /// Adds an error message and marks validation as failed
    /// </summary>
    void addError(const string& error) {
        errors.push_back(error);
        isValid = false;
    }

    /// <summary>
    /// Adds a warning message without failing validation
    /// </summary>
    void addWarning(const string& warning) {
        warnings.push_back(warning);
    }

    /// <summary>
    /// Generates a formatted report of all errors and warnings
    /// </summary>
    string getReport() const {
        stringstream ss;
        if (!isValid) {
            ss << "VALIDATION FAILED:" << endl;
            for (const auto& error : errors) {
                ss << "  - " << error << endl;
            }
        }
        for (const auto& warning : warnings) {
            ss << "  [WARNING] " << warning << endl;
        }
        if (isValid && warnings.empty()) {
            ss << "All validations passed!" << endl;
        }
        return ss.str();
    }
};

/// <summary>
/// Represents an existing loan from another bank
/// </summary>
struct ExistingLoan {
    bool isActive;
    long long totalAmount;
    long long amountReturned;
    long long amountDue;
    string bankName;
    string loanCategory;

    ExistingLoan() : isActive(false), totalAmount(0), amountReturned(0), amountDue(0) {}

    /// <summary>
    /// Validates loan data integrity
    /// </summary>
    /// <returns>True if loan data is valid</returns>
    bool validate() const {
        if (bankName.empty() || loanCategory.empty()) return false;
        if (totalAmount < 0 || amountReturned < 0 || amountDue < 0) return false;
        if (amountReturned > totalAmount || amountDue > totalAmount) return false;
        return true;
    }
};

/// <summary>
/// Stores reference/guarantor information
/// </summary>
struct Reference {
    string name;
    string cnic;
    string cnicIssueDate;
    string phoneNumber;
    string email;

    /// <summary>
    /// Validates reference data completeness and format
    /// </summary>
    /// <returns>True if all reference data is valid</returns>
    bool validate() const {
        if (name.empty() || name.length() > 100) return false;
        if (cnic.length() != 13) return false;
        for (char c : cnic) {
            if (!isdigit(static_cast<unsigned char>(c))) return false;
        }
        if (cnicIssueDate.length() != 10 || cnicIssueDate[2] != '-' || cnicIssueDate[5] != '-') {
            return false;
        }
        if (phoneNumber.length() < 10 || phoneNumber.length() > 15) return false;
        if (email.find('@') == string::npos || email.find('.') == string::npos) return false;
        return true;
    }
};

// Encapsulates all loan application data and behavior

/// <summary>
/// Main loan application class
/// SE Principles Applied:
/// - Encapsulation: Private data members with public getters/setters
/// - Single Responsibility: Manages loan application data only
/// - Data Validation: Validates data at setter level
/// - Information Hiding: Internal state hidden from external access
/// </summary>
class LoanApplication {
private:
    // Application metadata
    string applicationId;
    string status;
    string submissionDate;
    string completedSections;  

    // Loan details
    string loanType;
    string loanCategory;
    long long loanAmount;
    long long downPayment;
    int installmentMonths;
    long long monthlyPayment;
    int installmentStartMonth;
    int installmentStartYear;

    // Personal information
    string fullName;
    string fathersName;
    string postalAddress;
    string contactNumber;
    string emailAddress;
    string cnicNumber;
    string cnicExpiryDate;

    // Employment and family
    string employmentStatus;
    string maritalStatus;
    string gender;
    int numberOfDependents;

    // Financial information
    long long annualIncome;
    long long avgElectricityBill;
    long long currentElectricityBill;

    // Related entities
    vector<ExistingLoan> existingLoans;
    Reference reference1;
    Reference reference2;

    // Document paths
    string cnicFrontImagePath;
    string cnicBackImagePath;
    string electricityBillImagePath;
    string salarySlipImagePath;

    string rejectionReason;

public:

    /// <summary>
    /// Default constructor - initializes application with safe default values
    /// </summary>
    LoanApplication();



    /// <summary>
    /// Gets unique application identifier
    /// </summary>
    /// <returns>Application ID string</returns>
    string getApplicationId() const;

    /// <summary>
    /// Gets current application status
    /// Possible values: submitted, approved, rejected, C1, C2, C3, incomplete_*
    /// </summary>
    /// <returns>Status string</returns>
    string getStatus() const;

    string getSubmissionDate() const;
    string getFullName() const;
    string getFathersName() const;
    string getPostalAddress() const;
    string getContactNumber() const;
    string getEmailAddress() const;
    string getCnicNumber() const;
    string getCnicExpiryDate() const;
    string getEmploymentStatus() const;
    string getMaritalStatus() const;
    string getGender() const;
    int getNumberOfDependents() const;
    long long getAnnualIncome() const;
    long long getAvgElectricityBill() const;
    long long getCurrentElectricityBill() const;
    vector<ExistingLoan> getExistingLoans() const;
    Reference getReference1() const;
    Reference getReference2() const;
    string getCnicFrontImagePath() const;
    string getCnicBackImagePath() const;
    string getElectricityBillImagePath() const;
    string getSalarySlipImagePath() const;
    string getLoanType() const;
    string getLoanCategory() const;
    long long getLoanAmount() const;
    long long getDownPayment() const;
    int getInstallmentMonths() const;
    long long getMonthlyPayment() const;
    int getInstallmentStartMonth() const;
    int getInstallmentStartYear() const;
    string getRejectionReason() const;
    string getCompletedSections() const;

    /// <summary>
    /// Sets application ID with validation
    /// </summary>
    /// <param name="id">Unique application identifier</param>
    /// <exception cref="ValidationException">Thrown if ID is empty</exception>
    void setApplicationId(const string& id);

    /// <summary>
    /// Sets application status with validation
    /// Valid statuses: submitted, approved, rejected, C1, C2, C3, incomplete_*
    /// </summary>
    /// <param name="stat">Status string</param>
    /// <exception cref="ValidationException">Thrown if status is invalid</exception>
    void setStatus(const string& stat);

    /// <summary>
    /// Sets submission date with format validation
    /// </summary>
    /// <param name="date">Date in DD-MM-YYYY format</param>
    /// <exception cref="ValidationException">Thrown if format is invalid</exception>
    void setSubmissionDate(const string& date);

    /// <summary>
    /// Sets applicant's full name with length validation
    /// </summary>
    /// <param name="name">Full name (1-100 characters)</param>
    /// <exception cref="ValidationException">Thrown if name is empty or too long</exception>
    void setFullName(const string& name);

    void setFathersName(const string& name);
    void setPostalAddress(const string& address);

    /// <summary>
    /// Sets contact number with length validation
    /// </summary>
    /// <param name="number">Phone number (10-15 digits)</param>
    /// <exception cref="ValidationException">Thrown if length is invalid</exception>
    void setContactNumber(const string& number);

    /// <summary>
    /// Sets email with format validation
    /// </summary>
    /// <param name="email">Email address</param>
    /// <exception cref="ValidationException">Thrown if format is invalid</exception>
    void setEmailAddress(const string& email);

    /// <summary>
    /// Sets CNIC with strict validation
    /// </summary>
    /// <param name="cnic">13-digit CNIC without dashes</param>
    /// <exception cref="ValidationException">Thrown if not 13 digits or contains non-digits</exception>
    void setCnicNumber(const string& cnic);

    void setCnicExpiryDate(const string& date);
    void setEmploymentStatus(const string& status);
    void setMaritalStatus(const string& status);
    void setGender(const string& gen);
    void setNumberOfDependents(int dependents);
    void setAnnualIncome(long long income);
    void setAvgElectricityBill(long long amount);
    void setCurrentElectricityBill(long long amount);
    void setReference1(const Reference& ref);
    void setReference2(const Reference& ref);
    void setCnicFrontImagePath(const string& path);
    void setCnicBackImagePath(const string& path);
    void setElectricityBillImagePath(const string& path);
    void setSalarySlipImagePath(const string& path);
    void setLoanType(const string& type);
    void setLoanCategory(const string& category);
    void setLoanAmount(long long amount);
    void setDownPayment(long long payment);
    void setInstallmentMonths(int months);
    void setMonthlyPayment(long long payment);
    void setInstallmentStartMonth(int month);
    void setInstallmentStartYear(int year);
    void setRejectionReason(const string& reason);
    void setCompletedSections(const string& sections);

  

    /// <summary>
    /// Adds an existing loan to the application
    /// </summary>
    /// <param name="loan">Existing loan to add</param>
    void addExistingLoan(const ExistingLoan& loan);

    /// <summary>
    /// Removes all existing loans
    /// </summary>
    void clearExistingLoans();

    /// <summary>
    /// Gets count of existing loans
    /// </summary>
    /// <returns>Number of existing loans</returns>
    int getExistingLoansCount() const;

    /// <summary>
    /// Validates complete application data
    /// </summary>
    /// <returns>ValidationResult with all errors and warnings</returns>
    ValidationResult validateCompleteApplication() const;

    /// <summary>
    /// Validates personal information section
    /// </summary>
    /// <returns>ValidationResult for personal info</returns>
    ValidationResult validatePersonalInfo() const;

    /// <summary>
    /// Validates financial information section
    /// </summary>
    /// <returns>ValidationResult for financial info</returns>
    ValidationResult validateFinancialInfo() const;

    /// <summary>
    /// Validates references section
    /// </summary>
    /// <returns>ValidationResult for references</returns>
    ValidationResult validateReferences() const;

    /// <summary>
    /// Validates documents section
    /// </summary>
    /// <returns>ValidationResult for documents</returns>
    ValidationResult validateDocuments() const;

    /// <summary>
    /// Validates existing loans data
    /// </summary>
    /// <returns>ValidationResult for existing loans</returns>
    ValidationResult validateExistingLoans() const;

    /// <summary>
    /// Validates employment and financial information together
    /// </summary>
    /// <returns>ValidationResult for employment and financial data</returns>
    ValidationResult validateEmploymentAndFinancialInfo() const;

    /// <summary>
    /// Validates loan amount against income
    /// </summary>
    /// <param name="loanType">Type of loan (home, car, personal)</param>
    /// <param name="loanAmount">Requested loan amount</param>
    /// <returns>ValidationResult for income-to-loan ratio</returns>
    ValidationResult validateForLoanType(const string& loanType, long long loanAmount) const;

    /// <summary>
    /// Validates debt-to-income ratio
    /// </summary>
    /// <param name="result">ValidationResult to update with warnings/errors</param>
    /// <returns>True if ratio is acceptable</returns>
    bool validateDebtToIncomeRatio(ValidationResult& result) const;

    /// <summary>
    /// Validates income-to-loan ratio
    /// </summary>
    /// <param name="loanAmount">Requested loan amount</param>
    /// <param name="result">ValidationResult to update</param>
    /// <returns>True if ratio is acceptable</returns>
    bool validateIncomeToLoanRatio(long long loanAmount, ValidationResult& result) const;

    /// <summary>
    /// Validates income for specific loan type
    /// </summary>
    /// <param name="loanType">Type of loan</param>
    /// <param name="loanAmount">Requested amount</param>
    /// <param name="result">ValidationResult to update</param>
    /// <returns>True if income is sufficient</returns>
    bool validateIncomeForLoanType(const string& loanType, long long loanAmount, ValidationResult& result) const;

    /// <summary>
    /// Marks a section as completed and updates status
    /// </summary>
    /// <param name="section">Section name (personal, financial, references, documents)</param>
    void markSectionCompleted(const string& section);

    /// <summary>
    /// Checks if a specific section is completed
    /// </summary>
    /// <param name="section">Section name to check</param>
    /// <returns>True if section is completed</returns>
    bool isSectionCompleted(const string& section) const;

    /// <summary>
    /// Gets list of incomplete sections
    /// </summary>
    /// <returns>Vector of section names that are incomplete</returns>
    vector<string> getIncompleteSections() const;

    /// <summary>
    /// Checks if application is fully completed
    /// </summary>
    /// <returns>True if all sections are done</returns>
    bool isApplicationComplete() const;

    /// <summary>
    /// Gets the next section that needs to be completed
    /// </summary>
    /// <returns>Name of next incomplete section or "complete"</returns>
    string getNextIncompleteSection() const;

    // Allow FileManager to access private members for serialization
    friend class FileManager;
};

#endif