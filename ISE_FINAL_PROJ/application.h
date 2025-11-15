#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

/// <summary>
/// Base exception class for loan application errors
/// </summary>
class LoanApplicationException : public exception {
private:
    string message;
public:
    LoanApplicationException(const string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

/// <summary>
/// Exception for data validation errors
/// </summary>
class ValidationException : public LoanApplicationException {
public:
    ValidationException(const string& msg) : LoanApplicationException("Validation Error: " + msg) {}
};

/// <summary>
/// Represents an existing loan for an applicant
/// </summary>
struct ExistingLoan {
    bool isActive;
    long long totalAmount;
    long long amountReturned;
    long long amountDue;
    string bankName;
    string loanCategory;
};

/// <summary>
/// Represents a reference person for the applicant
/// </summary>
struct Reference {
    string name;
    string cnic;
    string cnicIssueDate;
    string phoneNumber;
    string email;
};

/// <summary>
/// Represents a complete loan application with all applicant data
/// </summary>
class LoanApplication {
private:
    // Application data
    string applicationId;
    string status;
    string submissionDate;

    // Personal Information
    string fullName;
    string fathersName;
    string postalAddress;
    string contactNumber;
    string emailAddress;
    string cnicNumber;
    string cnicExpiryDate;

    // Personal Details
    string employmentStatus;
    string maritalStatus;
    string gender;
    int numberOfDependents;

    // Financial Information
    long long annualIncome;
    long long avgElectricityBill;
    long long currentElectricityBill;

    // Existing Loans
    vector<ExistingLoan> existingLoans;

    // References
    Reference reference1;
    Reference reference2;

    // Image Paths
    string cnicFrontImagePath;
    string cnicBackImagePath;
    string electricityBillImagePath;
    string salarySlipImagePath;

public:
    // Constructors
    LoanApplication();

    // Getters
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

    // Setters
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

    // Existing Loans Management
    void addExistingLoan(const ExistingLoan& loan);
    void clearExistingLoans();
    int getExistingLoansCount() const;
};

#endif