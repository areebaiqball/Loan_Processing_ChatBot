#include "application_collector.h"
#include <iomanip>
#include <sstream>

/// <summary>
/// Main method to collect complete application
/// </summary>
LoanApplication ApplicationCollector::collectCompleteApplication() {
    LoanApplication application;

    cout << endl << Config::CHATBOT_NAME << ": " << "Starting loan application process..." << endl;
    cout << "I'll need to collect some information from you." << endl << endl;

    try {
        // Step 1: Personal Information
        if (!collectPersonalInfo(application)) {
            throw runtime_error("Personal information collection failed");
        }

        // Step 2: Employment & Financial Information
        if (!collectEmploymentAndFinancialInfo(application)) {
            throw runtime_error("Employment and financial information collection failed");
        }

        // Step 3: Existing Loans
        if (!collectExistingLoansInfo(application)) {
            throw runtime_error("Existing loans information collection failed");
        }

        // Step 4: References
        if (!collectReferencesInfo(application)) {
            throw runtime_error("References information collection failed");
        }

        // Step 5: Confirmation
        if (!confirmApplication(application)) {
            throw runtime_error("Application cancelled by user");
        }

        cout << Config::CHATBOT_NAME << ": " << "Application collected successfully!" << endl;

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error collecting application: " << e.what() << endl;
        throw;
    }

    return application;
}

/// <summary>
/// Collects application for a specific loan type
/// </summary>
LoanApplication ApplicationCollector::collectApplicationForLoan(const string& loanType, const string& loanDetails) {
    cout << Config::CHATBOT_NAME << ": " << "Starting " << loanType << " loan application process..." << endl;
    cout << "Loan Type: " << loanDetails << endl << endl;

    LoanApplication application;

    try {
        
        if (!collectPersonalInfo(application)) {
            throw runtime_error("Personal information collection failed");
        }

         
        if (!collectEmploymentAndFinancialInfo(application)) {
            throw runtime_error("Employment and financial information collection failed");
        }

        
        collectLoanSpecificInfo(application, loanType, loanDetails);

        
        if (!collectExistingLoansInfo(application)) {
            throw runtime_error("Existing loans information collection failed");
        }

      
        if (!collectReferencesInfo(application)) {
            throw runtime_error("References information collection failed");
        }

      
        if (!confirmApplication(application)) {
            throw runtime_error("Application cancelled by user");
        }

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error collecting application: " << e.what() << endl;
        throw;
    }

    return application;
}
/// <summary>
/// Collects basic personal information
/// </summary>
bool ApplicationCollector::collectPersonalInfo(LoanApplication& application) {
    cout << "=== PERSONAL INFORMATION ===" << endl;

    try {
       
        string fullName = getValidatedString(
            "Please enter your full name: ",
            "Full name",
            1, 100
        );
        application.setFullName(fullName);

       
        string fathersName = getValidatedString(
            "Please enter your father's name: ",
            "Father's name",
            1, 100
        );
        application.setFathersName(fathersName);

       
        string postalAddress = getValidatedString(
            "Please enter your complete postal address: ",
            "Postal address",
            1, 255
        );
        application.setPostalAddress(postalAddress);

        
        string contactNumber = getValidatedPhone("Please enter your contact number: ");
            
       
        application.setContactNumber(contactNumber);

      
        string email = getValidatedEmail("Please enter your email address: ");
          
       
        application.setEmailAddress(email);

      
        string cnic = getValidatedCNIC("Please enter your CNIC number (13 digits without dashes): ");
           
      
        application.setCnicNumber(cnic);

      
        string cnicExpiry = getValidatedDate("Please enter your CNIC expiry date (DD-MM-YYYY): ");
            
       
        application.setCnicExpiryDate(cnicExpiry);

        cout << Config::CHATBOT_NAME << ": " << "Personal information collected successfully!" << endl;
        return true;

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error in personal information: " << e.what() << endl;
        return false;
    }
}

/// <summary>
/// Collects employment and financial details
/// </summary>
bool ApplicationCollector::collectEmploymentAndFinancialInfo(LoanApplication& application) {
    cout << endl << "=== EMPLOYMENT & FINANCIAL INFORMATION ===" << endl;

    try {
      
        vector<string> employmentOptions = { "Self-employed", "Salaried", "Retired", "Unemployed" };
        string employmentStatus = getSelectionFromOptions(
            "Please select your employment status: ",
            employmentOptions
        );
        application.setEmploymentStatus(employmentStatus);

        
        vector<string> maritalOptions = { "Single", "Married", "Divorced", "Widowed" };
        string maritalStatus = getSelectionFromOptions("Please select your marital status: ", maritalOptions);
      
        application.setMaritalStatus(maritalStatus);

        vector<string> genderOptions = { "Male", "Female", "Other" };
        string gender = getSelectionFromOptions("Please select your gender: ", genderOptions);
        application.setGender(gender);
            
      
        cout << "You: ";
        string dependentsInput;
        getline(cin, dependentsInput);

        try {
            int dependents = stoi(trim(dependentsInput));
            application.setNumberOfDependents(dependents);
        }
        catch (...) {
            cout << Config::CHATBOT_NAME << ": " << "Invalid number. Please enter a valid number of dependents: ";
            getline(cin, dependentsInput);
            int dependents = stoi(trim(dependentsInput));
            application.setNumberOfDependents(dependents);
        }

      
        long long annualIncome = getValidatedNumeric("Please enter your annual income in PKR (without commas): ", "Annual income", 0, 1000000000);
      
        application.setAnnualIncome(annualIncome);

        long long avgBill = getValidatedNumeric("Please enter your average monthly electricity bill for last 12 months (PKR without commas): ", "Average electricity bill", 0, 1000000);
           
            application.setAvgElectricityBill(avgBill);

      
        long long currentBill = getValidatedNumeric("Please enter your current electricity bill amount (PKR without commas): ", "Current electricity bill", 0, 1000000);
          
      
        application.setCurrentElectricityBill(currentBill);

        cout << Config::CHATBOT_NAME << ": " << "Employment and financial information collected successfully!" << endl;
        return true;

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error in employment information: " << e.what() << endl;
        return false;
    }
}

/// <summary>
/// Collects existing loans information
/// </summary>
bool ApplicationCollector::collectExistingLoansInfo(LoanApplication& application) {
    cout << endl << "=== EXISTING LOANS INFORMATION ===" << endl;

    try {
        cout << Config::CHATBOT_NAME << ": " << "Do you have any existing loans? (yes/no): ";
        string hasLoans;
        getline(cin, hasLoans);
        hasLoans = toLower(trim(hasLoans));

        if (hasLoans == "yes" || hasLoans == "y") {
            bool addMoreLoans = true;

            while (addMoreLoans) {
                ExistingLoan loan;

                cout << endl << "--- Existing Loan Details ---" << endl;

                cout << Config::CHATBOT_NAME << ": " << "Is this loan currently active? (yes/no): ";
                string activeInput;
                getline(cin, activeInput);
                loan.isActive = (toLower(trim(activeInput)) == "yes" || toLower(trim(activeInput)) == "y");

                
                loan.totalAmount = getValidatedNumeric("Total loan amount (PKR without commas): ", "Total loan amount", 0, 1000000000);
             
                loan.amountReturned = getValidatedNumeric("Amount returned so far (PKR without commas): ", "Amount returned", 0, loan.totalAmount, );
              
                loan.amountDue = getValidatedNumeric("Amount still due (PKR without commas): ", "Amount due", 0, loan.totalAmount);
               
                loan.bankName = getValidatedString("Bank name: ", "Bank name", 1, 100);
                 
               vector<string> loanCategories = { "Car", "Home", "Bike" };
                loan.loanCategory = getSelectionFromOptions("Loan category: ", loanCategories);
               
                application.addExistingLoan(loan);

                cout << Config::CHATBOT_NAME << ": " << "Do you have another existing loan to add? (yes/no): ";
                string anotherLoan;
                getline(cin, anotherLoan);
                addMoreLoans = (toLower(trim(anotherLoan)) == "yes" || toLower(trim(anotherLoan)) == "y");
            }
        }

        cout << Config::CHATBOT_NAME << ": " << "Existing loans information collected successfully!" << endl;
        return true;

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error in existing loans information: " << e.what() << endl;
        return false;
    }
}

/// <summary>
/// Collects references information
/// </summary>
bool ApplicationCollector::collectReferencesInfo(LoanApplication& application) {
    cout << endl << "=== REFERENCES INFORMATION ===" << endl;

    try {
        cout << "We need information for two references." << endl;

        // Reference 1
        cout << endl << "--- Reference 1 ---" << endl;
        Reference ref1;
        ref1.name = getValidatedString("Reference 1 full name: ", "Reference name", 1, 100);
        ref1.cnic = getValidatedCNIC("Reference 1 CNIC (13 digits without dashes): ");
        ref1.cnicIssueDate = getValidatedDate("Reference 1 CNIC issue date (DD-MM-YYYY): ");
        ref1.phoneNumber = getValidatedPhone("Reference 1 phone number: ");
        ref1.email = getValidatedEmail("Reference 1 email address: ");
        application.setReference1(ref1);

        // Reference 2
        cout << endl << "--- Reference 2 ---" << endl;
        Reference ref2;
        ref2.name = getValidatedString("Reference 2 full name: ", "Reference name", 1, 100);
        ref2.cnic = getValidatedCNIC("Reference 2 CNIC (13 digits without dashes): ");
        ref2.cnicIssueDate = getValidatedDate("Reference 2 CNIC issue date (DD-MM-YYYY): ");
        ref2.phoneNumber = getValidatedPhone("Reference 2 phone number: ");
        ref2.email = getValidatedEmail("Reference 2 email address: ");
        application.setReference2(ref2);

        cout << Config::CHATBOT_NAME << ": " << "References information collected successfully!" << endl;
        return true;

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error in references information: " << e.what() << endl;
        return false;
    }
}

/// <summary>
/// Displays summary and gets confirmation
/// </summary>
bool ApplicationCollector::confirmApplication(const LoanApplication& application) {
    cout << endl << "=== APPLICATION SUMMARY ===" << endl;
    cout << "Please review your information:" << endl << endl;

    // Display summary
    cout << "Personal Information:" << endl;
    cout << "  Full Name: " << application.getFullName() << endl;
    cout << "  Father's Name: " << application.getFathersName() << endl;
    cout << "  CNIC: " << application.getCnicNumber() << endl;
    cout << "  Contact: " << application.getContactNumber() << endl;
    cout << "  Email: " << application.getEmailAddress() << endl;

    cout << endl << "Employment Information:" << endl;
    cout << "  Status: " << application.getEmploymentStatus() << endl;
    cout << "  Annual Income: PKR " << application.getAnnualIncome() << endl;

    cout << endl << "Existing Loans: " << application.getExistingLoansCount() << endl;

    cout << endl << Config::CHATBOT_NAME << ": " << "Do you confirm this information is correct? (yes/no): ";
    string confirmation;
    getline(cin, confirmation);
    confirmation = toLower(trim(confirmation));

    return (confirmation == "yes" || confirmation == "y");
}


string ApplicationCollector::getValidatedString(const string& prompt, const string& fieldName, size_t minLength, size_t maxLength) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = trim(input);

        if (input.length() < minLength) {
            cout << Config::CHATBOT_NAME << ": " << fieldName << " cannot be empty. Please try again." << endl;
            continue;
        }

        if (input.length() > maxLength) {
            cout << Config::CHATBOT_NAME << ": " << fieldName << " is too long. Maximum " << maxLength << " characters allowed." << endl;
            continue;
        }

        return input;
    }
}

long long ApplicationCollector::getValidatedNumeric(const string& prompt, const string& fieldName, long long minValue, long long maxValue) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = removeCommas(trim(input));

        try {
            long long value = stoll(input);

            if (value < minValue) {
                cout << Config::CHATBOT_NAME << ": " << fieldName << " cannot be less than " << minValue << ". Please try again." << endl;
                continue;
            }

            if (value > maxValue) {
                cout << Config::CHATBOT_NAME << ": " << fieldName << " cannot be more than " << maxValue << ". Please try again." << endl;
                continue;
            }

            return value;

        }
        catch (const exception&) {
            cout << Config::CHATBOT_NAME << ": " << "Invalid number format. Please enter a valid number." << endl;
        }
    }
}

string ApplicationCollector::getValidatedCNIC(const string& prompt) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = removeCommas(trim(input));

        if (input.length() != 13) {
            cout << Config::CHATBOT_NAME << ": " << "CNIC must be exactly 13 digits. Please try again." << endl;
            continue;
        }

        bool allDigits = true;
        for (char c : input) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            cout << Config::CHATBOT_NAME << ": " << "CNIC must contain only digits. Please try again." << endl;
            continue;
        }

        return input;
    }
}

string ApplicationCollector::getValidatedEmail(const string& prompt) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = trim(input);

        if (input.find('@') == string::npos || input.find('.') == string::npos) {
            cout << Config::CHATBOT_NAME << ": " << "Invalid email format. Please include '@' and '.'" << endl;
            continue;
        }

        return input;
    }
}

string ApplicationCollector::getValidatedPhone(const string& prompt) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = trim(input);

        
        string cleaned;
        for (char c : input) {
            if (c != ' ' && c != '-') {
                cleaned += c;
            }
        }

        if (cleaned.length() < 10 || cleaned.length() > 15) {
            cout << Config::CHATBOT_NAME << ": " << "Phone number must be 10-15 digits. Please try again." << endl;
            continue;
        }

        // Check if all characters are digits (or starts with + for international)
        bool valid = true;
        size_t start = 0;
        if (cleaned[0] == '+') {
            start = 1;
        }

        for (size_t i = start; i < cleaned.length(); i++) {
            if (!isdigit(static_cast<unsigned char>(cleaned[i]))) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            cout << Config::CHATBOT_NAME << ": " << "Phone number can only contain digits and optional '+' prefix. Please try again." << endl;
            continue;
        }

        return cleaned;
    }
}

string ApplicationCollector::getValidatedDate(const string& prompt) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = trim(input);

        if (input.length() != 10 || input[2] != '-' || input[5] != '-') {
            cout << Config::CHATBOT_NAME << ": " << "Date must be in DD-MM-YYYY format. Please try again." << endl;
            continue;
        }

        try {
            int day = stoi(input.substr(0, 2));
            int month = stoi(input.substr(3, 2));
            int year = stoi(input.substr(6, 4));

            if (day < 1 || day > 31) {
                cout << Config::CHATBOT_NAME << ": " << "Day must be between 1-31. Please try again." << endl;
                continue;
            }

            if (month < 1 || month > 12) {
                cout << Config::CHATBOT_NAME << ": " << "Month must be between 1-12. Please try again." << endl;
                continue;
            }

            if (year < 1900 || year > 2100) {
                cout << Config::CHATBOT_NAME << ": " << "Year must be between 1900-2100. Please try again." << endl;
                continue;
            }

            return input;

        }
        catch (const exception&) {
            cout << Config::CHATBOT_NAME << ": " << "Invalid date format. Please use DD-MM-YYYY." << endl;
        }
    }
}

string ApplicationCollector::getSelectionFromOptions(const string& prompt, const vector<string>& options) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt << endl;

        for (size_t i = 0; i < options.size(); i++) {
            cout << "  " << (i + 1) << ". " << options[i] << endl;
        }

        cout << "You: ";
        string input;
        getline(cin, input);
        input = trim(input);

        // Check if input is a number selection
        try {
            int selection = stoi(input);
            if (selection >= 1 && selection <= static_cast<int>(options.size())) {
                return options[selection - 1];
            }
        }
        catch (...) {
            
        }

      
        string lowerInput = toLower(input);
        for (const auto& option : options) {
            if (toLower(option) == lowerInput) {
                return option;
            }
        }

        cout << Config::CHATBOT_NAME << ": " << "Invalid selection. Please choose from the available options." << endl;
    }
}
/// <summary>
/// Collects loan-specific information based on loan type
/// </summary>
void ApplicationCollector::collectLoanSpecificInfo(LoanApplication& application, const string& loanType, const string& loanDetails) {
    cout << endl << "=== " << loanType << " LOAN SPECIFIC INFORMATION ===" << endl;

    if (loanType == "car") {
     
        string carMake = getValidatedString("Car make: ", "Car make", 1, 50);
        string carModel = getValidatedString("Car model: ", "Car model", 1, 50);
       

    }
    else if (loanType == "scooter") {
       
        string scooterModel = getValidatedString("Scooter model: ", "Scooter model", 1, 50);
        int distancePerCharge = getValidatedNumeric("Distance per charge (KM): ", "Distance", 0, 1000);
       

    }
    else if (loanType == "personal") {
      
        string loanPurpose = getValidatedString("Loan purpose: ", "Loan purpose", 1, 100);
     
    }

    cout << Config::CHATBOT_NAME << ": " << loanType << " specific information collected!" << endl;
}