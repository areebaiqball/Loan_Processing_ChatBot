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

/// <summary>
/// Loads home loan data from file
/// </summary>
/// <param name="loans">loans Array to store loans</param>
/// <param name="maxSize">Maximum array size</param>
/// <param name="filename">File to load from</param>
/// <returns>Number of loans loaded</returns>
int loadHomeLoans(HomeLoan loans[], int maxSize, const string& filename);

/// <summary>
/// Displays home loan options for area
/// </summary>
/// <param name="loans">Array of loans</param>
/// <param name="size">Array size</param>
/// <param name="areaNumber">Area identifier</param>
/// <returns>True if data found, false otherwise</returns>
bool displayHomeLoanOptions(const HomeLoan loans[], int size, const string& areaNumber);

/// <summary>
/// Displays installment plan for selected loan option
/// </summary>
/// <param name="loans">Array of loans</param>
/// <param name="size">Array size</param>
/// <param name="areaNumber">Area identifier</param>
/// <param name="optionNumber">Selected option number</param>
/// <returns>True if option found and plan displayed, false otherwise</returns>
bool displayInstallmentPlanForOption(const HomeLoan loans[], int size,
    const string& areaNumber, int optionNumber);

#endif