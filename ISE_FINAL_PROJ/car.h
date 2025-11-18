#ifndef CAR_H
#define CAR_H

#include <string>
using namespace std;

/// <summary>
/// Represents car loan details
/// </summary>
class CarLoan {
private:
    string make;
    string model;
    string engine;
    string used;
    string manufacturingYear;
    int installments;
    long long price;
    long long downPayment;

public:
    // Constructor 
    CarLoan();
    CarLoan(const string& mk, const string& mdl, const string& eng, const string& usd,
        const string& year, int inst, long long p, long long dp);

    // Getters 
    string getMake() const;
    string getModel() const;
    string getEngine() const;
    string getUsed() const;
    string getManufacturingYear() const;
    int getInstallments() const;
    long long getPrice() const;
    long long getDownPayment() const;

    // Setters
    void setMake(const string& mk);
    void setModel(const string& mdl);
    void setEngine(const string& eng);
    void setUsed(const string& usd);
    void setManufacturingYear(const string& year);
    void setInstallments(int inst);
    void setPrice(long long p);
    void setDownPayment(long long dp);

    // Business methods
    long long calculateMonthlyInstallment() const;
    void displayInstallmentPlan() const;
};

/// <summary>
/// Loads car loan data from file
/// </summary>
int loadCarLoans(CarLoan loans[], int maxSize, const string& filename);

/// <summary>
/// Displays car loan options
/// </summary>
bool displayCarLoanOptions(const CarLoan loans[], int size);

/// <summary>
/// Displays installment plan for specific car option
/// </summary>
bool displayCarInstallmentPlan(const CarLoan loans[], int size, int optionNumber);

#endif