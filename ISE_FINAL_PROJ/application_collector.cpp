#include "application_collector.h"

#include <iomanip>
#include <sstream>
#include<fstream>

/// <summary>
/// Main method to collect complete application
/// </summary>
/// 
LoanApplication ApplicationCollector::collectCompleteApplication() {
    LoanApplication application;

    cout << endl << Config::CHATBOT_NAME << ": " << "Starting loan application process..." << endl;
    cout << "I'll need to collect some information from you." << endl << endl;

    try {
        // Step 1: Personal Information
        cout << "STEP 1/5: Personal Information" << endl;
        if (!collectPersonalInfo(application)) {
            throw runtime_error("Personal information collection failed");
        }

        // Step 2: Employment & Financial Information  
        cout << endl << "STEP 2/5: Employment & Financial Information" << endl;
        if (!collectEmploymentAndFinancialInfo(application)) {
            throw runtime_error("Employment and financial information collection failed");
        }

        // Step 3: Existing Loans
        cout << endl << "STEP 3/5: Existing Loans" << endl;
        if (!collectExistingLoansInfo(application)) {
            throw runtime_error("Existing loans information collection failed");
        }

        // Step 4: References
        cout << endl << "STEP 4/5: References" << endl;
        if (!collectReferencesInfo(application)) {
            throw runtime_error("References information collection failed");
        }

        // Step 5: Document Information
        cout << endl << "STEP 5/5: Document Information" << endl;
        collectImagePaths(application);

        // Step 6: Final Validation and Confirmation
        cout << endl << "FINAL STEP: Review and Submit" << endl;
        ValidationResult validation = application.validateCompleteApplication();
        if (!validation.isValid) {
            cout << Config::CHATBOT_NAME << ": " << "Application validation failed:" << endl;
            cout << validation.getReport() << endl;
            cout << "Please correct the errors and try again." << endl;
            throw runtime_error("Application validation failed");
        }

        // Show warnings if any
        if (!validation.warnings.empty()) {
            cout << Config::CHATBOT_NAME << ": " << "Application warnings:" << endl;
            cout << validation.getReport() << endl;
            cout << "You may proceed, but please note these warnings." << endl;
        }

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
LoanApplication ApplicationCollector::collectApplicationForLoan(const string& loanType, const string& loanDetails, FileManager& fileManager) {
    cout << Config::CHATBOT_NAME << ": " << "Starting " << loanType << " loan application process..." << endl;
    cout << "Loan Type: " << loanDetails << endl << endl;

    LoanApplication application;

    try {
        // Step 1: Personal Information
        if (!collectPersonalInfo(application)) {
            throw runtime_error("Personal information collection failed");
        }

        // Step 2: Employment & Financial Information
        if (!collectEmploymentAndFinancialInfo(application)) {
            throw runtime_error("Employment and financial information collection failed");
        }

        // Step 3: Loan-specific information
        collectLoanSpecificInfo(application, loanType, loanDetails);

        // Step 4: Existing Loans
        if (!collectExistingLoansInfo(application)) {
            throw runtime_error("Existing loans information collection failed");
        }

        // Step 5: References
        if (!collectReferencesInfo(application)) {
            throw runtime_error("References information collection failed");
        }

        // Step 6: Document Images
        if (!collectImagePaths(application)) {
            throw runtime_error("Document information collection failed");
        }

        // Step 7: Confirmation
        if (!confirmApplication(application)) {
            throw runtime_error("Application cancelled by user");
        }

        // Save the application
        if (!fileManager.saveApplication(application)) {
            throw runtime_error("Failed to save application");
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
/// 
bool ApplicationCollector::collectEmploymentAndFinancialInfo(LoanApplication& application) {
    cout << endl << "=== EMPLOYMENT & FINANCIAL INFORMATION ===" << endl;

    try {
        // Employment Status with detailed options
        cout << Config::CHATBOT_NAME << ": " << "Let's collect your employment and financial details." << endl;

        vector<string> employmentOptions = {
            "Self-employed",
            "Salaried (Regular Job)",
            "Retired",
            "Unemployed"
        };
        string employmentStatus = getSelectionFromOptions(
            "Please select your current employment status:",
            employmentOptions
        );
        application.setEmploymentStatus(employmentStatus);

        // Employment-specific details
        if (employmentStatus == "Self-employed") {
            string businessType = getValidatedString(
                "Please describe your business/occupation: ",
                "Business type",
                2, 100
            );
            // You could store this in additional fields if needed
        }
        else if (employmentStatus == "Salaried (Regular Job)") {
            string employerName = getValidatedString(
                "Please enter your employer's name: ",
                "Employer name",
                2, 100
            );
            // Store employer name if needed
        }

        // Personal Details
        cout << endl << "--- Personal Details ---" << endl;

        vector<string> maritalOptions = { "Single", "Married", "Divorced", "Widowed" };
        string maritalStatus = getSelectionFromOptions(
            "Please select your marital status:",
            maritalOptions
        );
        application.setMaritalStatus(maritalStatus);

        vector<string> genderOptions = { "Male", "Female", "Other" };
        string gender = getSelectionFromOptions(
            "Please select your gender:",
            genderOptions
        );
        application.setGender(gender);

        // Number of Dependents with better validation
        bool validDependents = false;
        int dependents = 0;

        while (!validDependents) {
            cout << Config::CHATBOT_NAME << ": " << "Please enter the number of people who depend on you financially: ";
            string dependentsInput;
            getline(cin, dependentsInput);
            dependentsInput = trim(dependentsInput);

            try {
                dependents = stoi(dependentsInput);
                if (dependents >= 0 && dependents <= 20) {
                    validDependents = true;
                }
                else {
                    cout << Config::CHATBOT_NAME << ": " << "Please enter a number between 0 and 20." << endl;
                }
            }
            catch (const exception&) {
                cout << Config::CHATBOT_NAME << ": " << "Invalid number. Please enter a valid number (0-20): ";
            }
        }
        application.setNumberOfDependents(dependents);

        // Financial Information
        cout << endl << "--- Financial Information ---" << endl;

        // Annual Income with contextual guidance
        cout << Config::CHATBOT_NAME << ": " << "Now let's collect your financial information." << endl;

        long long annualIncome = 0;
        if (employmentStatus == "Unemployed") {
            cout << Config::CHATBOT_NAME << ": " << "Since you're unemployed, please enter your household income or other sources of income." << endl;
            annualIncome = getValidatedNumeric(
                "Annual household/support income (PKR without commas): ",
                "Annual income",
                0, 50000000
            );
        }
        else {
            annualIncome = getValidatedNumeric(
                "Please enter your annual income in PKR (without commas): ",
                "Annual income",
                100000, 50000000  // Minimum 100,000 PKR for employed
            );
        }
        application.setAnnualIncome(annualIncome);

        // Income validation based on employment status
        if (employmentStatus != "Unemployed" && annualIncome < 200000) {
            cout << Config::CHATBOT_NAME << ": " << "Note: Your income seems low for your employment status." << endl;
        }

        // Electricity Bills with better context
        cout << endl << "--- Utility Bill Information ---" << endl;
        cout << Config::CHATBOT_NAME << ": " << "We need information about your electricity bills for address verification." << endl;

        long long avgElectricityBill = getValidatedNumeric(
            "Average monthly electricity bill for last 12 months (PKR without commas): ",
            "Average electricity bill",
            0, 100000
        );
        application.setAvgElectricityBill(avgElectricityBill);

        long long currentElectricityBill = getValidatedNumeric(
            "Current electricity bill amount (PKR without commas): ",
            "Current electricity bill",
            0, 100000
        );
        application.setCurrentElectricityBill(currentElectricityBill);

        // Validate bill consistency
        if (currentElectricityBill > avgElectricityBill * 2) {
            cout << Config::CHATBOT_NAME << ": " << "Note: Your current bill is significantly higher than your average." << endl;
        }

        // Additional Financial Information (if needed)
        if (annualIncome > 1000000) { // For higher income applicants
            cout << endl << "--- Additional Financial Details ---" << endl;

            vector<string> incomeSourceOptions = {
                "Salary/Wages",
                "Business Income",
                "Investments",
                "Rental Income",
                "Other"
            };

            string primaryIncomeSource = getSelectionFromOptions(
                "Primary source of income:",
                incomeSourceOptions
            );

            // You could store this in additional application fields
        }

        // Summary and Confirmation
        cout << endl << "--- Summary ---" << endl;
        cout << Config::CHATBOT_NAME << ": " << "Let me confirm your employment and financial details:" << endl;
        cout << "  Employment Status: " << employmentStatus << endl;
        cout << "  Marital Status: " << maritalStatus << endl;
        cout << "  Gender: " << gender << endl;
        cout << "  Dependents: " << dependents << endl;
        cout << "  Annual Income: PKR " << annualIncome << endl;
        cout << "  Avg Electricity Bill: PKR " << avgElectricityBill << endl;
        cout << "  Current Electricity Bill: PKR " << currentElectricityBill << endl;

        cout << Config::CHATBOT_NAME << ": " << "Is this information correct? (yes/no): ";
        string confirmation;
        getline(cin, confirmation);
        confirmation = toLower(trim(confirmation));

        if (confirmation == "yes" || confirmation == "y") {
            cout << Config::CHATBOT_NAME << ": " << "Employment and financial information collected successfully!" << endl;
            return true;
        }
        else {
            cout << Config::CHATBOT_NAME << ": " << "Let's collect this information again." << endl;
            return collectEmploymentAndFinancialInfo(application); // Recursive retry
        }

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
        cout << Config::CHATBOT_NAME << ": " << "Do you have any existing loans from banks or financial institutions? (yes/no): ";
        string hasLoans;
        getline(cin, hasLoans);
        hasLoans = toLower(trim(hasLoans));

        if (hasLoans == "yes" || hasLoans == "y") {
            bool addMoreLoans = true;
            int loanCount = 0;

            while (addMoreLoans && loanCount < 10) { // Limit to 10 loans
                loanCount++;
                ExistingLoan loan;

                cout << endl << "--- Existing Loan #" << loanCount << " ---" << endl;

                // Loan active status
                vector<string> activeOptions = { "Yes, this loan is currently active", "No, this loan is closed" };
                string activeChoice = getSelectionFromOptions("Is this loan currently active?: ", activeOptions);
                loan.isActive = (activeChoice.find("Yes") != string::npos);

                // Loan amounts with better validation
                loan.totalAmount = getValidatedNumeric("Total original loan amount (PKR): ", "Total loan amount", 1000, 1000000000);

                if (loan.isActive) {
                    loan.amountReturned = getValidatedNumeric("Amount paid back so far (PKR): ", "Amount returned", 0, loan.totalAmount);
                    loan.amountDue = getValidatedNumeric("Remaining amount to pay (PKR): ", "Amount due", 0, loan.totalAmount);

                    // Auto-calculate if amounts don't add up
                    if (loan.amountReturned + loan.amountDue != loan.totalAmount) {
                        cout << Config::CHATBOT_NAME << ": " << "Note: The amounts don't add up correctly." << endl;
                        cout << "Total: " << loan.totalAmount << " = Paid: " << loan.amountReturned << " + Due: " << loan.amountDue << endl;
                        cout << "Auto-calculating due amount as: " << (loan.totalAmount - loan.amountReturned) << endl;
                        loan.amountDue = loan.totalAmount - loan.amountReturned;
                    }
                }
                else {
                    loan.amountReturned = loan.totalAmount;
                    loan.amountDue = 0;
                    cout << Config::CHATBOT_NAME << ": " << "Marking as fully paid since loan is closed." << endl;
                }

                loan.bankName = getValidatedString("Bank or financial institution name: ", "Bank name", 2, 100);

                vector<string> loanCategories = { "Car Loan", "Home Loan", "Personal Loan", "Business Loan", "Education Loan", "Other" };
                loan.loanCategory = getSelectionFromOptions("Type of loan: ", loanCategories);

                // Validate this loan before adding
                if (!loan.validate()) {
                    cout << Config::CHATBOT_NAME << ": " << "Invalid loan information. Please re-enter this loan." << endl;
                    loanCount--;
                    continue;
                }

                application.addExistingLoan(loan);
                cout << Config::CHATBOT_NAME << ": " << "✓ Loan #" << loanCount << " recorded successfully!" << endl;

                if (loanCount < 10) {
                    cout << Config::CHATBOT_NAME << ": " << "Do you have another existing loan to add? (yes/no): ";
                    string anotherLoan;
                    getline(cin, anotherLoan);
                    addMoreLoans = (toLower(trim(anotherLoan)) == "yes" || toLower(trim(anotherLoan)) == "y");
                }
                else {
                    cout << Config::CHATBOT_NAME << ": " << "Maximum of 10 loans reached." << endl;
                    addMoreLoans = false;
                }
            }

            cout << Config::CHATBOT_NAME << ": " << "Total " << loanCount << " loan(s) recorded." << endl;
        }
        else {
            cout << Config::CHATBOT_NAME << ": " << "No existing loans recorded." << endl;
        }

        cout << Config::CHATBOT_NAME << ": " << " Existing loans information completed!" << endl;
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

    // Personal Information
    cout << "PERSONAL INFORMATION:" << endl;
    cout << "  Full Name: " << application.getFullName() << endl;
    cout << "  Father's Name: " << application.getFathersName() << endl;
    cout << "  CNIC: " << application.getCnicNumber() << endl;
    cout << "  Contact: " << application.getContactNumber() << endl;
    cout << "  Email: " << application.getEmailAddress() << endl;
    cout << "  Address: " << application.getPostalAddress() << endl;

    // Employment & Financial Information
    cout << endl << "EMPLOYMENT & FINANCIAL INFORMATION:" << endl;
    cout << "  Employment Status: " << application.getEmploymentStatus() << endl;
    cout << "  Marital Status: " << application.getMaritalStatus() << endl;
    cout << "  Gender: " << application.getGender() << endl;
    cout << "  Dependents: " << application.getNumberOfDependents() << endl;
    cout << "  Annual Income: PKR " << application.getAnnualIncome() << endl;
    cout << "  Avg Electricity Bill: PKR " << application.getAvgElectricityBill() << endl;
    cout << "  Current Electricity Bill: PKR " << application.getCurrentElectricityBill() << endl;

    // Existing Loans
    cout << endl << "EXISTING LOANS: " << application.getExistingLoansCount() << " loan(s)" << endl;
    auto existingLoans = application.getExistingLoans();
    for (size_t i = 0; i < existingLoans.size(); i++) {
        cout << "  Loan " << (i + 1) << ": " << existingLoans[i].bankName
            << " - PKR " << existingLoans[i].totalAmount
            << " (" << (existingLoans[i].isActive ? "Active" : "Closed") << ")" << endl;
    }

    // References
    Reference ref1 = application.getReference1();
    Reference ref2 = application.getReference2();
    cout << endl << "REFERENCES:" << endl;
    cout << "  Reference 1: " << ref1.name << " (" << ref1.phoneNumber << ")" << endl;
    cout << "  Reference 2: " << ref2.name << " (" << ref2.phoneNumber << ")" << endl;

    cout << endl << Config::CHATBOT_NAME << ": " << "Do you confirm this information is correct and ready for submission? (yes/no): ";
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

        // Allow empty for optional fields with minValue = 0
        if (input.empty() && minValue == 0) {
            return 0;
        }

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

            // Additional validation for specific fields
            if (fieldName.find("income") != string::npos && value > 0 && value < 10000) {
                cout << Config::CHATBOT_NAME << ": " << "Income seems unusually low. Please confirm this is correct: ";
                string confirm;
                getline(cin, confirm);
                if (toLower(trim(confirm)) != "yes" && toLower(trim(confirm)) != "y") {
                    continue;
                }
            }

            return value;

        }
        catch (const exception&) {
            cout << Config::CHATBOT_NAME << ": " << "Invalid number format. Please enter a valid number without commas." << endl;
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
/// <summary>
/// Collects image paths for document uploads
/// </summary>
bool ApplicationCollector::collectImagePaths(LoanApplication& application) {
    cout << endl << "=== DOCUMENT UPLOAD ===" << endl;

    try {
        cout << Config::CHATBOT_NAME << ": " << "Now we need the file paths for your document images." << endl;
        cout << "Please make sure the image files exist at the paths you provide." << endl;
        cout << "The images will be copied to the project's 'images' folder." << endl << endl;

        // CNIC Front Image
        cout << "--- CNIC Front Side ---" << endl;
        string cnicFrontPath = getImagePath("CNIC Front Side");
        application.setCnicFrontImagePath(cnicFrontPath);

        // CNIC Back Image
        cout << "--- CNIC Back Side ---" << endl;
        string cnicBackPath = getImagePath("CNIC Back Side");
        application.setCnicBackImagePath(cnicBackPath);

        // Electricity Bill Image
        cout << "--- Electricity Bill ---" << endl;
        string electricityBillPath = getImagePath("Recent Electricity Bill");
        application.setElectricityBillImagePath(electricityBillPath);

        // Salary Slip Image
        cout << "--- Salary Slip/Bank Statement ---" << endl;
        string salarySlipPath = getImagePath("Salary Slip or Bank Statement");
        application.setSalarySlipImagePath(salarySlipPath);

        cout << Config::CHATBOT_NAME << ": " << "Document paths recorded successfully!" << endl;
        cout << "Images will be copied to the 'images' folder when you submit the application." << endl;
        return true;

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error in document information: " << e.what() << endl;
        return false;
    }
}

/// <summary>
/// Gets validated image path from user
/// </summary>
string ApplicationCollector::getImagePath(const string& imageType) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << "Enter the full file path for " << imageType << ":" << endl;
        cout << "Example: C:/Users/YourName/Documents/cnic_front.jpg" << endl;
        cout << "You: ";

        string path;
        getline(cin, path);
        path = trim(path);

        if (path.empty()) {
            cout << Config::CHATBOT_NAME << ": " << "Path cannot be empty. Please enter a valid file path." << endl;
            continue;
        }

        // Basic validation - check if file exists
        ifstream testFile(path);
        if (!testFile.is_open()) {
            cout << Config::CHATBOT_NAME << ": " << "Warning: Cannot find file at this path." << endl;
            cout << "Do you want to continue anyway? (yes/no): ";
            string confirm;
            getline(cin, confirm);
            if (toLower(trim(confirm)) != "yes" && toLower(trim(confirm)) != "y") {
                continue;
            }
        }
        else {
            testFile.close();
        }

        return path;
    }
}