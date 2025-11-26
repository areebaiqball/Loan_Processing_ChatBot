#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
using namespace std;

class LoanApplicationException : public exception {
private:
    string message;
public:
    LoanApplicationException(const string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class ValidationException : public LoanApplicationException {
public:
    ValidationException(const string& msg) : LoanApplicationException("Validation Error: " + msg) {}
};

struct ValidationResult {
    bool isValid;
    vector<string> errors;
    vector<string> warnings;

    ValidationResult() : isValid(true) {}

    void addError(const string& error) {
        errors.push_back(error);
        isValid = false;
    }

    void addWarning(const string& warning) {
        warnings.push_back(warning);
    }

    string getReport() const {
        stringstream ss;
        if (!isValid) {
            ss << "VALIDATION FAILED:" << endl;
            for (const auto& error : errors) {
                ss << " " << error << endl;
            }
        }
        for (const auto& warning : warnings) {
            ss << " " << warning << endl;
        }
        if (isValid && warnings.empty()) {
            ss << "All validations passed!" << endl;
        }
        return ss.str();
    }
};

struct ExistingLoan {
    bool isActive = false;
    long long totalAmount = 0;
    long long amountReturned = 0;
    long long amountDue = 0;
    string bankName;
    string loanCategory;

    bool validate() const {
        if (bankName.empty()) return false;
        if (loanCategory.empty()) return false;
        if (totalAmount < 0 || amountReturned < 0 || amountDue < 0) return false;
        if (amountReturned > totalAmount) return false;
        if (amountDue > totalAmount) return false;
        return true;
    }
};

struct Reference {
    string name;
    string cnic;
    string cnicIssueDate;
    string phoneNumber;
    string email;

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

class LoanApplication {
private:
    string applicationId;
    string status;
    string submissionDate;
    string completedSections;

    string loanType;
    string loanCategory;
    long long loanAmount;
    long long downPayment;
    int installmentMonths;
    long long monthlyPayment;
    int installmentStartMonth;
    int installmentStartYear;

    string fullName;
    string fathersName;
    string postalAddress;
    string contactNumber;
    string emailAddress;
    string cnicNumber;
    string cnicExpiryDate;

    string employmentStatus;
    string maritalStatus;
    string gender;
    int numberOfDependents;

    long long annualIncome;
    long long avgElectricityBill;
    long long currentElectricityBill;

    vector<ExistingLoan> existingLoans;

    Reference reference1;
    Reference reference2;

    string cnicFrontImagePath;
    string cnicBackImagePath;
    string electricityBillImagePath;
    string salarySlipImagePath;

    string reference1CnicFrontImagePath;
    string reference1CnicBackImagePath;
    string reference2CnicFrontImagePath;
    string reference2CnicBackImagePath;
    string bankStatementImagePath;
    string additionalSalarySlipsImagePath;
    string businessDocumentsImagePath;

    string rejectionReason;

public:
    LoanApplication();

    string getApplicationId() const;
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

    void setApplicationId(const string& id);
    void setStatus(const string& stat);
    void setSubmissionDate(const string& date);
    void setFullName(const string& name);
    void setFathersName(const string& name);
    void setPostalAddress(const string& address);
    void setContactNumber(const string& number);
    void setEmailAddress(const string& email);
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

    string getReference1CnicFrontImagePath() const;
    void setReference1CnicFrontImagePath(const string& path);
    string getReference1CnicBackImagePath() const;
    void setReference1CnicBackImagePath(const string& path);
    string getReference2CnicFrontImagePath() const;
    void setReference2CnicFrontImagePath(const string& path);
    string getReference2CnicBackImagePath() const;
    void setReference2CnicBackImagePath(const string& path);
    string getBankStatementImagePath() const;
    void setBankStatementImagePath(const string& path);
    string getAdditionalSalarySlipsImagePath() const;
    void setAdditionalSalarySlipsImagePath(const string& path);
    string getBusinessDocumentsImagePath() const;
    void setBusinessDocumentsImagePath(const string& path);

    void addExistingLoan(const ExistingLoan& loan);
    void clearExistingLoans();
    int getExistingLoansCount() const;

    ValidationResult validateCompleteApplication() const;
    ValidationResult validateForLoanType(const string& loanType, long long loanAmount) const;
    bool validateIncomeToLoanRatio(long long loanAmount, ValidationResult& result) const;
    bool validateDebtToIncomeRatio(ValidationResult& result) const;
    ValidationResult validateExistingLoans() const;
    ValidationResult validateReferences() const;
    ValidationResult validateEmploymentAndFinancialInfo() const;
    bool validateIncomeForLoanType(const string& loanType, long long loanAmount, ValidationResult& result) const;

    // Multi-session methods
   void markSectionCompleted(const string& section);
    bool isSectionCompleted(const string& section) const;
    vector<string> getIncompleteSections() const;
    bool isApplicationComplete() const;
    string getNextIncompleteSection() const;
    string getCompletedSections() const;
    void setCompletedSections(const string& sections);

    ValidationResult validatePersonalInfo() const;
    ValidationResult validateFinancialInfo() const;
    ValidationResult validateDocuments() const;

   
        void setFieldDirect(const string& field, const string& value);

        friend class FileManager;
};

#endif