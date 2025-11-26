#include "application_collector.h"
#include <iomanip>
#include <sstream>
#include<fstream>

LoanApplication ApplicationCollector::collectApplicationForLoan(const string& loanType, const string& loanDetails) {
    cout << Config::CHATBOT_NAME << ": " << "Starting " << loanType << " loan application process..." << endl;
    cout << "Loan Type: " << loanDetails << endl << endl;

    LoanApplication application;

    try {
        string currentDate = getCurrentDate();
        application.setSubmissionDate(currentDate);

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


    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error collecting application: " << e.what() << endl;
        throw;
    }

    return application;
}

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

        string cnicIssueDate = getValidatedCNICIssueDate("Please enter your CNIC issue date (DD-MM-YYYY): ");  

        string cnicExpiry = getValidatedCNICExpiryDate("Please enter your CNIC expiry date (DD-MM-YYYY): ");  
        application.setCnicExpiryDate(cnicExpiry);

        cout << Config::CHATBOT_NAME << ": Personal information collected successfully!" << endl;
        return true;
    }
    catch (const ValidationException& e) {
        cout << Config::CHATBOT_NAME << ": " << e.what() << endl;
        return false;
    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Error in personal information: " << e.what() << endl;
        return false;
    }
}

bool ApplicationCollector::collectEmploymentAndFinancialInfo(LoanApplication& application) {
    cout << endl << "=== EMPLOYMENT & FINANCIAL INFORMATION ===" << endl;

    try {
        cout << Config::CHATBOT_NAME << ": " << "Let's collect your employment and financial details." << endl;

        vector<string> employmentOptions = {
            "Self-employed",
            "Salaried",
            "Retired",
            "Unemployed"
        };
        string employmentStatus = getSelectionFromOptions(
            "Please select your current employment status:",  
            employmentOptions
        );
        application.setEmploymentStatus(employmentStatus);

        if (employmentStatus == "Self-employed") {
            string businessType = getValidatedString(
                "Please describe your business/occupation: ",
                "Business type",
                2, 100
            );
        }
        else if (employmentStatus == "Salaried") {
            string employerName = getValidatedString(
               "Please enter your employer's name: ",
                "Employer name",
                2, 100
            );
        }

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

        cout << endl << "--- Financial Information ---" << endl;
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
                100000, 50000000
            );
        }
        application.setAnnualIncome(annualIncome);

        if (employmentStatus != "Unemployed" && annualIncome < 200000) {
            cout << Config::CHATBOT_NAME << ": " << "Note: Your income seems low for your employment status." << endl;
        }

        cout << endl << "--- Utility Bill Information ---" << endl;
        cout << Config::CHATBOT_NAME << ": " << "We need information about your electricity bills for address verification." << endl;

        long long avgElectricityBill = getValidatedNumeric(
            Config::CHATBOT_NAME + ": Average monthly electricity bill for last 12 months (PKR without commas): ",
            "Average electricity bill",
            0, 100000
        );
        application.setAvgElectricityBill(avgElectricityBill);

        long long currentElectricityBill = getValidatedNumeric(
            "Current electricity bill amount(PKR without commas) : ",
            "Current electricity bill",
            0, 100000
        );
        application.setCurrentElectricityBill(currentElectricityBill);

        if (currentElectricityBill > avgElectricityBill * 2) {
            cout << Config::CHATBOT_NAME << ": " << "Note: Your current bill is significantly higher than your average." << endl;
        }

        if (annualIncome > 1000000) {
            cout << endl << "--- Additional Financial Details ---" << endl;

            vector<string> incomeSourceOptions = {
                "Salary/Wages",
                "Business Income",
                "Investments",
                "Rental Income",
                "Other"
            };

            string primaryIncomeSource = getSelectionFromOptions(
               "Primary source of income : ",
                incomeSourceOptions
            );
        }

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
            return collectEmploymentAndFinancialInfo(application);
        }
    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error in employment information: " << e.what() << endl;
        return false;
    }
}

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

            while (addMoreLoans && loanCount < 10) {
                loanCount++;
                ExistingLoan loan;

                cout << endl << "--- Existing Loan #" << loanCount << " ---" << endl;

                vector<string> activeOptions = { "Yes, this loan is currently active", "No, this loan is closed" };
                string activeChoice = getSelectionFromOptions("Is this loan currently active?: ", activeOptions);  
                loan.isActive = (activeChoice.find("Yes") != string::npos);

                loan.totalAmount = getValidatedNumeric("Total original loan amount (PKR): ", "Total loan amount", 1000, 1000000000);  

                if (loan.isActive) {
                    loan.amountReturned = getValidatedNumeric(Config::CHATBOT_NAME + ": Amount paid back so far (PKR): ", "Amount returned", 0, loan.totalAmount);
                    loan.amountDue = getValidatedNumeric(Config::CHATBOT_NAME + ": Remaining amount to pay (PKR): ", "Amount due", 0, loan.totalAmount);

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

                loan.bankName = getValidatedString(Config::CHATBOT_NAME + ": Bank or financial institution name: ", "Bank name", 2, 100);

                vector<string> loanCategories = { "Car Loan", "Home Loan", "Personal Loan", "Business Loan", "Education Loan", "Other" };
                loan.loanCategory = getSelectionFromOptions(Config::CHATBOT_NAME + ": Type of loan: ", loanCategories);

                if (!loan.validate()) {
                    cout << Config::CHATBOT_NAME << ": " << "Invalid loan information. Please re-enter this loan." << endl;
                    loanCount--;
                    continue;
                }

                application.addExistingLoan(loan);
                cout << Config::CHATBOT_NAME << ": " << " Loan #" << loanCount << " recorded successfully!" << endl;

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
bool ApplicationCollector::collectReferencesInfo(LoanApplication& application) {
    cout << endl << "=== REFERENCES INFORMATION ===" << endl;

    try {
        cout << "We need information for two references." << endl;

        bool referencesValid = false;
        int attemptCount = 0;
        const int MAX_ATTEMPTS = 3;

        while (!referencesValid && attemptCount < MAX_ATTEMPTS) {
            attemptCount++;

            if (attemptCount > 1) {
                cout << endl << "--- Please correct the reference information ---" << endl;
            }

            cout << endl << "--- Reference 1 ---" << endl;
            Reference ref1;
            ref1.name = getValidatedString("Reference 1 full name: ", "Reference name", 1, 100);
            ref1.cnic = getValidatedCNIC(Config::CHATBOT_NAME + ": Reference 1 CNIC (13 digits without dashes): ");
            ref1.cnicIssueDate = getValidatedCNICIssueDate(Config::CHATBOT_NAME + ": Reference 1 CNIC issue date (DD-MM-YYYY): ");
            ref1.phoneNumber = getValidatedPhone(Config::CHATBOT_NAME + ": Reference 1 phone number: ");
            ref1.email = getValidatedEmail(Config::CHATBOT_NAME + ": Reference 1 email address: ");

            cout << endl << "--- Reference 2 ---" << endl;
            Reference ref2;
            ref2.name = getValidatedString(Config::CHATBOT_NAME + ": Reference 2 full name: ", "Reference name", 1, 100);

            bool ref2CnicValid = false;
            while (!ref2CnicValid) {
                ref2.cnic = getValidatedCNIC(Config::CHATBOT_NAME + ": Reference 2 CNIC (13 digits without dashes): ");

                if (ref2.cnic == ref1.cnic) {
                    cout << endl << "VALIDATION FAILED:" << endl;
                    cout << "Reference 2 CNIC cannot be the same as Reference 1 CNIC!" << endl;
                    cout << "Reference 1 CNIC: " << ref1.cnic << endl;
                    cout << "Reference 2 CNIC: " << ref2.cnic << endl;
                    cout << "Please enter a different CNIC for Reference 2." << endl << endl;
                }
                else {
                    ref2CnicValid = true;
                }
            }

            ref2.cnicIssueDate = getValidatedCNICIssueDate(Config::CHATBOT_NAME + ": Reference 2 CNIC issue date (DD-MM-YYYY): ");
            ref2.phoneNumber = getValidatedPhone(Config::CHATBOT_NAME + ": Reference 2 phone number: ");
            ref2.email = getValidatedEmail(Config::CHATBOT_NAME + ": Reference 2 email address: ");

            bool ref1Valid = ref1.validate();
            bool ref2Valid = ref2.validate();

            if (!ref1Valid || !ref2Valid) {
                cout << endl << "VALIDATION FAILED:" << endl;
                if (!ref1Valid) {
                    cout << "Reference 1 has invalid data." << endl;
                }
                if (!ref2Valid) {
                    cout << "Reference 2 has invalid data." << endl;
                }

                if (attemptCount < MAX_ATTEMPTS) {
                    cout << "Please correct the reference information." << endl;
                    continue;
                }
                else {
                    cout << "Maximum attempts reached. Please try again later." << endl;
                    return false;
                }
            }
            application.setReference1(ref1);
            application.setReference2(ref2);
            referencesValid = true;
        }

        if (referencesValid) {
            cout << Config::CHATBOT_NAME << ": References information collected successfully!" << endl;
            return true;
        }
        else {
            return false;
        }
    }
    catch (const ValidationException& e) {
        cout << Config::CHATBOT_NAME << ": " << e.what() << endl;
        return false;
    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Error in references information: " << e.what() << endl;
        return false;
    }
}


bool ApplicationCollector::confirmApplication(const LoanApplication& application) {
    cout << endl << "=== APPLICATION SUMMARY ===" << endl;
    cout << "Please review your information:" << endl << endl;

    cout << "PERSONAL INFORMATION:" << endl;
    cout << "  Full Name: " << application.getFullName() << endl;
    cout << "  Father's Name: " << application.getFathersName() << endl;
    cout << "  CNIC: " << application.getCnicNumber() << endl;
    cout << "  Contact: " << application.getContactNumber() << endl;
    cout << "  Email: " << application.getEmailAddress() << endl;
    cout << "  Address: " << application.getPostalAddress() << endl;

    cout << endl << "EMPLOYMENT & FINANCIAL INFORMATION:" << endl;
    cout << "  Employment Status: " << application.getEmploymentStatus() << endl;
    cout << "  Marital Status: " << application.getMaritalStatus() << endl;
    cout << "  Gender: " << application.getGender() << endl;
    cout << "  Dependents: " << application.getNumberOfDependents() << endl;
    cout << "  Annual Income: PKR " << application.getAnnualIncome() << endl;
    cout << "  Avg Electricity Bill: PKR " << application.getAvgElectricityBill() << endl;
    cout << "  Current Electricity Bill: PKR " << application.getCurrentElectricityBill() << endl;

    cout << endl << "EXISTING LOANS: " << application.getExistingLoansCount() << " loan(s)" << endl;
    auto existingLoans = application.getExistingLoans();
    for (size_t i = 0; i < existingLoans.size(); i++) {
        cout << "  Loan " << (i + 1) << ": " << existingLoans[i].bankName
            << " - PKR " << existingLoans[i].totalAmount
            << " (" << (existingLoans[i].isActive ? "Active" : "Closed") << ")" << endl;
    }

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

        bool hasLetters = false;
        bool hasOnlyNumbers = true;

        for (char c : input) {
            if (isalpha(static_cast<unsigned char>(c))) {
                hasLetters = true;
                hasOnlyNumbers = false;
            }
            else if (!isspace(static_cast<unsigned char>(c)) && !ispunct(static_cast<unsigned char>(c))) {
                continue;
            }
        }

        if (!hasLetters) {
            if (hasOnlyNumbers && input.length() > 0) {
                cout << Config::CHATBOT_NAME << ": " << fieldName << " cannot contain only numbers. Please enter a valid name." << endl;
            }
            else {
                cout << Config::CHATBOT_NAME << ": " << fieldName << " must contain letters. Please enter a valid name." << endl;
            }
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

string ApplicationCollector::getValidatedCNICIssueDate(const string& prompt) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = trim(input);

        if (input.length() != 10 || input[2] != '-' || input[5] != '-') {
            cout << Config::CHATBOT_NAME << ": Date must be in DD-MM-YYYY format. Please try again." << endl;
            continue;
        }

        try {
            int day = stoi(input.substr(0, 2));
            int month = stoi(input.substr(3, 2));
            int year = stoi(input.substr(6, 4));

            if (day < 1 || day > 31) {
                cout << Config::CHATBOT_NAME << ": Day must be between 1-31. Please try again." << endl;
                continue;
            }

            if (month < 1 || month > 12) {
                cout << Config::CHATBOT_NAME << ": Month must be between 1-12. Please try again." << endl;
                continue;
            }

            if (year < 1900 || year > 2100) {
                cout << Config::CHATBOT_NAME << ": Year must be between 1900-2100. Please try again." << endl;
                continue;
            }

            string currentDate = getCurrentDate();
            int comparison = compareDates(input, currentDate);

            if (comparison > 0) {
                cout << Config::CHATBOT_NAME << ":  Issue date cannot be in the future. Please enter a valid issue date." << endl;
                cout << "Current date: " << currentDate << endl;
                cout << "Please try again." << endl;
                continue;
            }

            return input;
        }
        catch (const exception&) {
            cout << Config::CHATBOT_NAME << ": Invalid date format. Please use DD-MM-YYYY." << endl;
        }
    }
}
string ApplicationCollector::getValidatedCNICExpiryDate(const string& prompt) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = trim(input);

        if (input.length() != 10 || input[2] != '-' || input[5] != '-') {
            cout << Config::CHATBOT_NAME << ": Date must be in DD-MM-YYYY format. Please try again." << endl;
            continue;
        }

        try {
            int day = stoi(input.substr(0, 2));
            int month = stoi(input.substr(3, 2));
            int year = stoi(input.substr(6, 4));

            if (day < 1 || day > 31) {
                cout << Config::CHATBOT_NAME << ": Day must be between 1-31. Please try again." << endl;
                continue;
            }

            if (month < 1 || month > 12) {
                cout << Config::CHATBOT_NAME << ": Month must be between 1-12. Please try again." << endl;
                continue;
            }

            if (year < 1900 || year > 2100) {
                cout << Config::CHATBOT_NAME << ": Year must be between 1900-2100. Please try again." << endl;
                continue;
            }

            string currentDate = getCurrentDate();
            int comparison = compareDates(input, currentDate);

            if (comparison <= 0) {
                cout << endl << "========================================" << endl;
                cout << "   CNIC EXPIRED" << endl;
                cout << "========================================" << endl;
                cout << "Your CNIC has expired or is expired today." << endl;
                cout << "Current date: " << currentDate << endl;
                cout << "CNIC expiry date: " << input << endl;
                cout << endl;
                cout << "You must renew your CNIC before applying for a loan." << endl;
                cout << "========================================" << endl;
                cout << endl;

                cout << Config::CHATBOT_NAME << ": Would you like to:" << endl;
                cout << "  1. Enter a new expiry date (continue with corrected date)" << endl;
                cout << "  2. Return to main menu (exit application)" << endl;
                cout << Config::CHATBOT_NAME << ": Please choose (1 or 2): ";

                string choice;
                getline(cin, choice);
                choice = trim(choice);

                if (choice == "1") {
                    cout << endl << Config::CHATBOT_NAME << ": Please enter the correct CNIC expiry date:" << endl;
                    continue;
                }
                else if (choice == "2") {
                    cout << endl << Config::CHATBOT_NAME << ": Returning to main menu..." << endl;
                    throw ValidationException("User chose to return to main menu due to expired CNIC.");
                }
                else {
                    cout << Config::CHATBOT_NAME << ": Invalid choice. Please enter 1 or 2." << endl;
                    continue;
                }
            }

            return input;
        }
        catch (const ValidationException&) {
            throw;
        }
        catch (const exception&) {
            cout << Config::CHATBOT_NAME << ": Invalid date format. Please use DD-MM-YYYY." << endl;
        }
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

string ApplicationCollector::getValidatedDate(const string& prompt) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = trim(input);

        if (input.length() != 10 || input[2] != '-' || input[5] != '-') {
            cout << Config::CHATBOT_NAME << ": Date must be in DD-MM-YYYY format. Please try again." << endl;
            continue;
        }

        try {
            int day = stoi(input.substr(0, 2));
            int month = stoi(input.substr(3, 2));
            int year = stoi(input.substr(6, 4));

            if (day < 1 || day > 31) {
                cout << Config::CHATBOT_NAME << ": Day must be between 1-31. Please try again." << endl;
                continue;
            }

            if (month < 1 || month > 12) {
                cout << Config::CHATBOT_NAME << ": Month must be between 1-12. Please try again." << endl;
                continue;
            }

            if (year < 1900 || year > 2100) {
                cout << Config::CHATBOT_NAME << ": Year must be between 1900-2100. Please try again." << endl;
                continue;
            }

            return input;
        }
        catch (const exception&) {
            cout << Config::CHATBOT_NAME << ": Invalid date format. Please use DD-MM-YYYY." << endl;
        }
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

        if (cleaned.length() != 11) {
            cout << Config::CHATBOT_NAME << ": Phone number must be exactly 11 digits. Please try again." << endl;
            cout << "Example: 03001234567" << endl;
            continue;
        }

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
            cout << Config::CHATBOT_NAME << ": Phone number can only contain digits. Please try again." << endl;
            continue;
        }

        return cleaned;
    }
}

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

bool ApplicationCollector::collectImagePaths(LoanApplication& application) {
    cout << endl << "=== DOCUMENT UPLOAD ===" << endl;

    try {
        cout << Config::CHATBOT_NAME << ": " << "Now we need the file paths for your document images." << endl;
        cout << "Please make sure the image files exist at the paths you provide." << endl;
        cout << "The images will be copied to the project's 'images' folder." << endl << endl;

        bool documentsValid = false;
        int attemptCount = 0;
        const int MAX_ATTEMPTS = 3;

        while (!documentsValid && attemptCount < MAX_ATTEMPTS) {
            attemptCount++;

            if (attemptCount > 1) {
                cout << endl << "--- Attempt " << attemptCount << " - Please correct the document paths ---" << endl;
            }

            // CNIC Front Side
            cout << "--- CNIC Front Side ---" << endl;
            string cnicFrontPath = getImagePath("CNIC Front Side");
            application.setCnicFrontImagePath(cnicFrontPath);

            // CNIC Back Side
            cout << "--- CNIC Back Side ---" << endl;
            string cnicBackPath = getImagePath("CNIC Back Side");
            application.setCnicBackImagePath(cnicBackPath);

            // Electricity Bill
            cout << "--- Electricity Bill ---" << endl;
            string electricityBillPath = getImagePath("Recent Electricity Bill");
            application.setElectricityBillImagePath(electricityBillPath);

            // Salary Slip/Bank Statement
            cout << "--- Salary Slip/Bank Statement ---" << endl;
            string salarySlipPath = getImagePath("Salary Slip or Bank Statement");
            application.setSalarySlipImagePath(salarySlipPath);

            // Simple validation - check if any paths are empty
            if (application.getCnicFrontImagePath().empty() ||
                application.getCnicBackImagePath().empty() ||
                application.getElectricityBillImagePath().empty() ||
                application.getSalarySlipImagePath().empty()) {

                cout << endl << "  VALIDATION FAILED:" << endl;
                cout << "  • Some required documents are missing" << endl;

                if (application.getCnicFrontImagePath().empty())
                    cout << "  • CNIC Front Side is required" << endl;
                if (application.getCnicBackImagePath().empty())
                    cout << "  • CNIC Back Side is required" << endl;
                if (application.getElectricityBillImagePath().empty())
                    cout << "  • Electricity Bill is required" << endl;
                if (application.getSalarySlipImagePath().empty())
                    cout << "  • Salary Slip/Bank Statement is required" << endl;

                if (attemptCount < MAX_ATTEMPTS) {
                    cout << endl << Config::CHATBOT_NAME << ": Please provide all required documents." << endl;
                    // Continue the loop to try again
                    continue;
                }
                else {
                    cout << endl << Config::CHATBOT_NAME << ": Maximum attempts reached. Please complete documents later." << endl;
                    // Don't mark as completed if documents are missing
                    return false;
                }
            }

            documentsValid = true;
        }

        if (documentsValid) {
            cout << endl << Config::CHATBOT_NAME << ": " << "All documents uploaded successfully!" << endl;
            application.markSectionCompleted("documents");
            return true;
        }
        else {
            cout << endl << Config::CHATBOT_NAME << ": Document upload incomplete. You can complete it later." << endl;
            return false;
        }
    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error in document information: " << e.what() << endl;
        return false;
    }
}
string ApplicationCollector::getImagePath(const string& imageType) {
    int attemptCount = 0;
    const int MAX_ATTEMPTS = 3;

    while (attemptCount < MAX_ATTEMPTS) {
        attemptCount++;

        cout << endl;
        cout << "========================================" << endl;
        cout << "  " << imageType << " (Attempt " << attemptCount << " of " << MAX_ATTEMPTS << ")" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": Please enter the file path for " << imageType << endl;
        cout << endl;
        cout << "  • This must be from your ORIGINAL location on your computer" << endl;
        cout << "  • Example: C:\\Users\\YourName\\Pictures\\document.jpg" << endl;
        cout << "  • Or: /home/user/documents/file.png" << endl;
        cout << endl;
        cout << "File path: ";

        string path;
        getline(cin, path);
        path = trim(path);

        if (path.empty()) {
            cout << Config::CHATBOT_NAME << ": No path provided. Skipping this document." << endl;
            return "";
        }

        // Convert backslashes to forward slashes for consistency
        for (char& c : path) {
            if (c == '\\') c = '/';
        }

        // Check if file exists
        ifstream testFile(path, ios::binary | ios::ate);
        if (!testFile.is_open()) {
            cout << endl << "ERROR: Cannot find file at this location" << endl;
            cout << "Path tried: " << path << endl;

            if (attemptCount < MAX_ATTEMPTS) {
                cout << "Please check the path and try again." << endl;
                continue;
            }
            else {
                cout << Config::CHATBOT_NAME << ": Maximum attempts reached. Skipping this document." << endl;
                return "";
            }
        }

        streamsize fileSize = testFile.tellg();
        testFile.close();

        if (fileSize == 0) {
            cout << endl << "ERROR: File is empty (0 bytes)" << endl;
            if (attemptCount < MAX_ATTEMPTS) {
                cout << "Please select a file with content." << endl;
                continue;
            }
            else {
                cout << Config::CHATBOT_NAME << ": Maximum attempts reached. Skipping this document." << endl;
                return "";
            }
        }

        cout << endl << "File found and validated!" << endl;
        return path; // SUCCESS
    }

    return ""; // If all attempts failed
}

bool ApplicationCollector::collectReferenceDocuments(LoanApplication& application) {
    cout << endl << "=== REFERENCE DOCUMENTS ===" << endl;

    try {
        cout << Config::CHATBOT_NAME << ": " << "Now we need CNIC images for your references." << endl;

        cout << endl << "--- Reference 1 CNIC ---" << endl;
        string ref1CnicFront = getImagePath("Reference 1 CNIC Front Side");
        string ref1CnicBack = getImagePath("Reference 1 CNIC Back Side");

        cout << endl << "--- Reference 2 CNIC ---" << endl;
        string ref2CnicFront = getImagePath("Reference 2 CNIC Front Side");
        string ref2CnicBack = getImagePath("Reference 2 CNIC Back Side");

        cout << Config::CHATBOT_NAME << ": " << "Reference documents recorded successfully!" << endl;
        return true;
    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error in reference documents: " << e.what() << endl;
        return false;
    }
}

bool ApplicationCollector::collectFinancialDocuments(LoanApplication& application) {
    cout << endl << "=== ADDITIONAL FINANCIAL DOCUMENTS ===" << endl;

    try {
        cout << Config::CHATBOT_NAME << ": " << "We need some additional financial documents for verification." << endl;

        cout << endl << "--- Bank Statement ---" << endl;
        string bankStatementPath = getImagePath("Bank Statement (Last 3 months)");

        if (application.getEmploymentStatus() == "Salaried (Regular Job)" ||
            application.getEmploymentStatus() == "Salaried") {
            cout << endl << "--- Additional Salary Slips ---" << endl;
            string additionalSalarySlips = getImagePath("Last 3 Salary Slips");
        }

        if (application.getEmploymentStatus() == "Self-employed") {
            cout << endl << "--- Business Documents ---" << endl;
            string businessDocuments = getImagePath("Business Registration Documents");
        }

        cout << Config::CHATBOT_NAME << ": " << "Financial documents recorded successfully!" << endl;
        return true;
    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": " << "Error in financial documents: " << e.what() << endl;
        return false;
    }
}
/// <summary>
/// Collects installment start month and year from user
/// </summary>
void ApplicationCollector::collectInstallmentStartDate(LoanApplication& application) {
    cout << endl << "=== INSTALLMENT START DATE ===" << endl;
    cout << Config::CHATBOT_NAME << ": When would you like to start paying installments?" << endl;

    // Get starting month
    int startMonth = 0;
    while (startMonth < 1 || startMonth > 12) {
        cout << Config::CHATBOT_NAME << ": Enter starting month (1=January, 2=February, ..., 12=December): ";
        string monthInput;
        getline(cin, monthInput);

        try {
            startMonth = stoi(trim(monthInput));
            if (startMonth < 1 || startMonth > 12) {
                cout << " Month must be between 1 and 12." << endl;
            }
        }
        catch (...) {
            cout << " Invalid input. Please enter a number (1-12)." << endl;
            startMonth = 0;
        }
    }
    application.setInstallmentStartMonth(startMonth);

    // Get starting year
    int startYear = 0;
    while (startYear < 2024 || startYear > 2100) {
        cout << Config::CHATBOT_NAME << ": Enter starting year (e.g., 2024, 2025): ";
        string yearInput;
        getline(cin, yearInput);

        try {
            startYear = stoi(trim(yearInput));
            if (startYear < 2024 || startYear > 2100) {
                cout << " Year must be between 2024 and 2100." << endl;
            }
        }
        catch (...) {
            cout << " Invalid input. Please enter a valid year." << endl;
            startYear = 0;
        }
    }
    application.setInstallmentStartYear(startYear);

    cout << " Installment start date set to: " << getMonthName(startMonth) << " " << startYear << endl;
}
bool ApplicationCollector::getValidatedYesNo(const string& prompt) {
    while (true) {
        cout << Config::CHATBOT_NAME << ": " << prompt;
        string input;
        getline(cin, input);
        input = toLower(trim(input));

        if (input == Config::EXIT_COMMAND) {
            throw ValidationException("User cancelled operation");
        }

        if (input == "y" || input == "yes") {
            return true;
        }
        else if (input == "n" || input == "no") {
            return false;
        }
        else {
            // Check if input is a number
            bool isNumber = true;
            for (char c : input) {
                if (!isdigit(static_cast<unsigned char>(c))) {
                    isNumber = false;
                    break;
                }
            }

            if (isNumber) {
                cout << Config::CHATBOT_NAME << ":  Invalid input. Please enter 'Y' for Yes or 'N' for No, not numbers." << endl;
            }
            else {
                cout << Config::CHATBOT_NAME << ":  Invalid input. Please enter 'Y' for Yes or 'N' for No." << endl;
            }
        }
    }
}