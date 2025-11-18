#ifndef SCOOTER_H
#define SCOOTER_H

#include <string>
using namespace std;

/// <summary>
/// Represents scooter loan details
/// </summary>
class ScooterLoan {
private:
    string make;
    string model;
    string distancePerCharge;
    string chargingTime;
    string maxSpeed;
    int installments;
    long long price;
    long long downPayment;

public:
    // Constructor 
    ScooterLoan();
    ScooterLoan(const string& mk, const string& mdl, const string& dist, const string& chargeTime,
        const string& speed, int inst, long long p, long long dp);

    // Getters 
    string getMake() const;
    string getModel() const;
    string getDistancePerCharge() const;
    string getChargingTime() const;
    string getMaxSpeed() const;
    int getInstallments() const;
    long long getPrice() const;
    long long getDownPayment() const;

    // Setters
    void setMake(const string& mk);
    void setModel(const string& mdl);
    void setDistancePerCharge(const string& dist);
    void setChargingTime(const string& chargeTime);
    void setMaxSpeed(const string& speed);
    void setInstallments(int inst);
    void setPrice(long long p);
    void setDownPayment(long long dp);

    // Business methods
    long long calculateMonthlyInstallment() const;
    void displayInstallmentPlan() const;
};

/// <summary>
/// Loads scooter loan data from file
/// </summary>
int loadScooterLoans(ScooterLoan loans[], int maxSize, const string& filename);

/// <summary>
/// Displays scooter loan options
/// </summary>
bool displayScooterLoanOptionsTable(const ScooterLoan loans[], int size, const string& makeNumber);
/// <summary>
/// Displays installment plan for specific scooter option
/// </summary>
bool displayScooterInstallmentPlan(const ScooterLoan loans[], int size, int optionNumber);

#endif