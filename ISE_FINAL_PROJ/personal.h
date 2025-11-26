#ifndef PERSONAL_H
#define PERSONAL_H

#include <string>
#include <vector>
using namespace std;

/// <summary>
/// Represents personal loan details with various purposes
/// </summary>
class PersonalLoan {
private:
    string category;           // e.g., "Standard Personal Loan", "Education Loan"
    string purpose;            // e.g., "General Use", "Education"
    string description;        // Detailed description
    int installments;
    long long amount;          // Total loan amount
    long long downPayment;

public:
    // Constructors
    PersonalLoan();
    PersonalLoan(const string& cat, const string& purp, const string& desc,
        int inst, long long amt, long long dp);

    // Getters
    string getCategory() const;
    string getPurpose() const;
    string getDescription() const;
    int getInstallments() const;
    long long getAmount() const;
    long long getDownPayment() const;

    // Setters
    void setCategory(const string& cat);
    void setPurpose(const string& purp);
    void setDescription(const string& desc);
    void setInstallments(int inst);
    void setAmount(long long amt);
    void setDownPayment(long long dp);

    // Business methods
    long long calculateMonthlyInstallment() const;
    void displayInstallmentPlan() const;
};

/// <summary>
/// Loads personal loan data from file
/// </summary>
int loadPersonalLoans(PersonalLoan loans[], int maxSize, const string& filename);

/// <summary>
/// Displays personal loan options by category
/// </summary>
bool displayPersonalLoanOptionsByCategory(const PersonalLoan loans[], int size,
    const string& categoryIndex);

/// <summary>
/// Displays installment plan for specific option
/// </summary>
bool displayPersonalInstallmentPlan(const PersonalLoan loans[], int size,
    int optionNumber);

/// <summary>
/// Gets all unique categories
/// </summary>
vector<string> getPersonalLoanCategories(const PersonalLoan loans[], int size);

#endif