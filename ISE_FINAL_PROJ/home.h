#ifndef HOME_H
#define HOME_H

#include <string>
using namespace std;

/// <summary>
/// Represents home loan details
/// </summary>
class HomeLoan {
private:
    string area;
    string size;
    int installments;
    long long price;
    long long downPayment;

public:
    // Constructor 
    HomeLoan();
    HomeLoan(const string& a, const string& s, int inst, long long p, long long dp);

    // Getters 
    string getArea() const;
    string getSize() const;
    int getInstallments() const;
    long long getPrice() const;
    long long getDownPayment() const;

    // Setters
    void setArea(const string& a);
    void setSize(const string& s);
    void setInstallments(int inst);
    void setPrice(long long p);
    void setDownPayment(long long dp);
    long long calculateMonthlyInstallment() const;
    void displayInstallmentPlan() const;

};

int loadHomeLoans(HomeLoan loans[], int maxSize, const string& filename);

bool displayHomeLoanOptions(const HomeLoan loans[], int size, const string& areaNumber);

bool displayInstallmentPlanForOption(const HomeLoan loans[], int size,
    const string& areaNumber, int optionNumber);

#endif