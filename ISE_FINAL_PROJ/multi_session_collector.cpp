#include "multi_session_collector.h"
#include <iomanip>

using namespace std;

void MultiSessionCollector::showApplicationMenu() {
    cout << endl << "========================================" << endl;
    cout << "        LOAN APPLICATION SYSTEM" << endl;
    cout << "========================================" << endl;
    cout << "1. Start New Application" << endl;
    cout << "2. Resume Incomplete Application" << endl;
    cout << "3. Check Application Status" << endl;
    cout << "4. Exit to Main Menu" << endl;
    cout << "========================================" << endl;
    cout << "Choose option: ";
}

void MultiSessionCollector::displayApplicationProgress(const LoanApplication& application) {
    cout << endl << "=== APPLICATION PROGRESS ===" << endl;
    cout << "Application ID: " << application.getApplicationId() << endl;
    cout << "Current Status: " << application.getStatus() << endl;

    vector<string> sections = { "personal", "financial", "references", "documents" };
    cout << "Completed Sections: ";

    bool first = true;
    for (const auto& section : sections) {
        if (application.isSectionCompleted(section)) {
            if (!first) cout << ", ";
            cout << section;
            first = false;
        }
    }
    if (first) cout << "None";
    cout << endl;

    cout << "Next Section: " << application.getNextIncompleteSection() << endl;
    cout << "========================================" << endl;
}

void MultiSessionCollector::startNewApplication() {
    cout << endl << "=== STARTING NEW APPLICATION ===" << endl;

    // Loan type selection
    cout << "Select Loan Type:" << endl;
    cout << "1. Home Loan" << endl;
    cout << "2. Car Loan" << endl;
    cout << "3. Scooter Loan" << endl;
    cout << "4. Personal Loan" << endl;
    cout << "Choose option: ";

    string loanChoice;
    getline(cin, loanChoice);
    loanChoice = trim(loanChoice);

    string loanType, loanDetails;
    if (loanChoice == "1") {
        loanType = "home";
        loanDetails = "Home Loan";
    }
    else if (loanChoice == "2") {
        loanType = "car";
        loanDetails = "Car Loan";
    }
    else if (loanChoice == "3") {
        loanType = "scooter";
        loanDetails = "Scooter Loan";
    }
    else if (loanChoice == "4") {
        loanType = "personal";
        loanDetails = "Personal Loan";
    }
    else {
        cout << "Invalid selection. Returning to menu." << endl;
        return;
    }

    LoanApplication application;
    application.setLoanType(loanType);
    application.setLoanCategory(loanDetails);
    application.setStatus("incomplete_personal");

    // Generate application ID
    string appId = fileManager.generateApplicationId();
    application.setApplicationId(appId);

    cout << endl << "🎉 NEW APPLICATION CREATED 🎉" << endl;
    cout << "Your Application ID: " << appId << endl;
    cout << "Loan Type: " << loanDetails << endl;
    cout << "IMPORTANT: Save your Application ID to resume later!" << endl;
    cout << "========================================" << endl;

    // Start with personal information
    collectPersonalInfo(application);
}

void MultiSessionCollector::resumeExistingApplication() {
    cout << endl << "=== RESUME INCOMPLETE APPLICATION ===" << endl;

    string applicationId, cnic;

    cout << "Enter your Application ID: ";
    getline(cin, applicationId);
    applicationId = trim(applicationId);

    if (applicationId.empty()) {
        cout << "Application ID cannot be empty." << endl;
        return;
    }

    cout << "Enter your CNIC (13 digits without dashes): ";
    getline(cin, cnic);
    cnic = trim(cnic);

    // Validate CNIC format
    if (cnic.length() != 13) {
        cout << "ERROR: CNIC must be exactly 13 digits." << endl;
        return;
    }
    for (char c : cnic) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            cout << "ERROR: CNIC must contain only digits." << endl;
            return;
        }
    }

    LoanApplication application = fileManager.findIncompleteApplication(applicationId, cnic);

    if (application.getApplicationId().empty()) {
        cout << endl << "❌ ERROR: No incomplete application found with provided ID and CNIC." << endl;
        cout << "Possible reasons:" << endl;
        cout << "- Application ID is incorrect" << endl;
        cout << "- CNIC does not match" << endl;
        cout << "- Application is already completed" << endl;
        cout << "- Application does not exist" << endl;
        cout << "Please check your information or start a new application." << endl;
        return;
    }

    cout << endl << "✅ APPLICATION FOUND!" << endl;
    displayApplicationProgress(application);

    string nextSection = application.getNextIncompleteSection();
    cout << "Resuming from: " << nextSection << " section" << endl;

    bool continueProcessing = true;

    while (continueProcessing && !application.isApplicationComplete()) {
        nextSection = application.getNextIncompleteSection();

        if (nextSection == "personal") {
            continueProcessing = collectPersonalInfo(application);
        }
        else if (nextSection == "financial") {
            continueProcessing = collectFinancialInfo(application);
        }
        else if (nextSection == "references") {
            continueProcessing = collectReferencesInfo(application);
        }
        else if (nextSection == "documents") {
            continueProcessing = collectDocumentsInfo(application);
        }
        else {
            break;
        }

        if (continueProcessing && !application.isApplicationComplete()) {
            cout << endl << "Continue to next section? (yes/no): ";
            string choice;
            getline(cin, choice);
            choice = toLower(trim(choice));
            continueProcessing = (choice == "yes" || choice == "y");
        }
    }

    if (application.isApplicationComplete()) {
        cout << endl << "🎉 APPLICATION COMPLETED SUCCESSFULLY! 🎉" << endl;
        cout << "Application ID: " << application.getApplicationId() << endl;
        cout << "Status: " << application.getStatus() << endl;
        cout << "Thank you for submitting your application!" << endl;
    }
}

bool MultiSessionCollector::collectPersonalInfo(LoanApplication& application) {
    cout << endl << "========================================" << endl;
    cout << "        PERSONAL INFORMATION" << endl;
    cout << "========================================" << endl;

    try {
        cout << "Please enter your personal details:" << endl << endl;

        string fullName = baseCollector.getValidatedString("Full Name: ", "Full name", 1, 100);
        application.setFullName(fullName);

        string fathersName = baseCollector.getValidatedString("Father's Name: ", "Father's name", 1, 100);
        application.setFathersName(fathersName);

        string postalAddress = baseCollector.getValidatedString("Postal Address: ", "Postal address", 1, 255);
        application.setPostalAddress(postalAddress);

        string contactNumber = baseCollector.getValidatedPhone("Contact Number: ");
        application.setContactNumber(contactNumber);

        string email = baseCollector.getValidatedEmail("Email Address: ");
        application.setEmailAddress(email);

        string cnic = baseCollector.getValidatedCNIC("CNIC Number (13 digits without dashes): ");
        application.setCnicNumber(cnic);

        string cnicExpiry = baseCollector.getValidatedCNICExpiryDate("CNIC Expiry Date (DD-MM-YYYY): ");
        application.setCnicExpiryDate(cnicExpiry);

        vector<string> employmentOptions = { "Self-employed", "Salaried", "Retired", "Unemployed" };
        string employmentStatus = baseCollector.getSelectionFromOptions("Employment Status:", employmentOptions);
        application.setEmploymentStatus(employmentStatus);

        vector<string> maritalOptions = { "Single", "Married", "Divorced", "Widowed" };
        string maritalStatus = baseCollector.getSelectionFromOptions("Marital Status:", maritalOptions);
        application.setMaritalStatus(maritalStatus);

        vector<string> genderOptions = { "Male", "Female", "Other" };
        string gender = baseCollector.getSelectionFromOptions("Gender:", genderOptions);
        application.setGender(gender);

        int dependents = baseCollector.getValidatedNumeric("Number of Dependents: ", "Dependents", 0, 20);
        application.setNumberOfDependents(dependents);

        // Validate the section
        ValidationResult validation = application.validatePersonalInfo();
        if (!validation.isValid) {
            cout << endl << "❌ VALIDATION ERRORS:" << endl;
            cout << validation.getReport();
            cout << "Please correct the errors above." << endl;
            return false;
        }

        // Save this section
        return saveSectionAndContinue(application, "personal");

    }
    catch (const exception& e) {
        cout << "Error collecting personal information: " << e.what() << endl;
        return false;
    }
}

bool MultiSessionCollector::collectFinancialInfo(LoanApplication& application) {
    cout << endl << "========================================" << endl;
    cout << "        FINANCIAL INFORMATION" << endl;
    cout << "========================================" << endl;

    try {
        cout << "Please enter your financial details:" << endl << endl;

        long long annualIncome = baseCollector.getValidatedNumeric(
            "Annual Income in PKR (without commas): ", "Annual income", 0, 1000000000);
        application.setAnnualIncome(annualIncome);

        long long avgElectricityBill = baseCollector.getValidatedNumeric(
            "Monthly Average Electricity Bill (PKR, without commas): ", "Average electricity bill", 0, 100000);
        application.setAvgElectricityBill(avgElectricityBill);

        long long currentElectricityBill = baseCollector.getValidatedNumeric(
            "Current Electricity Bill Amount (PKR, without commas): ", "Current electricity bill", 0, 100000);
        application.setCurrentElectricityBill(currentElectricityBill);

        // Existing loans collection
        cout << endl << "--- EXISTING LOANS ---" << endl;
        cout << "Do you have any existing loans? (yes/no): ";
        string hasLoans;
        getline(cin, hasLoans);
        hasLoans = toLower(trim(hasLoans));

        if (hasLoans == "yes" || hasLoans == "y") {
            application.clearExistingLoans();
            bool addMoreLoans = true;
            int loanCount = 0;

            while (addMoreLoans && loanCount < 10) {
                loanCount++;
                ExistingLoan loan;

                cout << endl << "--- Existing Loan #" << loanCount << " ---" << endl;

                vector<string> activeOptions = { "Yes - Loan is currently active", "No - Loan is closed" };
                string activeChoice = baseCollector.getSelectionFromOptions("Is this loan active?", activeOptions);
                loan.isActive = (activeChoice.find("Yes") != string::npos);

                loan.totalAmount = baseCollector.getValidatedNumeric("Total Loan Amount (PKR): ", "Total amount", 1000, 1000000000);

                if (loan.isActive) {
                    loan.amountReturned = baseCollector.getValidatedNumeric("Amount Returned (PKR): ", "Amount returned", 0, loan.totalAmount);
                    loan.amountDue = loan.totalAmount - loan.amountReturned;
                    cout << "Calculated remaining amount: PKR " << loan.amountDue << endl;
                }
                else {
                    loan.amountReturned = loan.totalAmount;
                    loan.amountDue = 0;
                    cout << "Loan marked as fully paid (closed)." << endl;
                }

                loan.bankName = baseCollector.getValidatedString("Bank Name: ", "Bank name", 2, 100);

                vector<string> loanCategories = { "Car", "Home", "Bike", "Personal", "Business", "Other" };
                loan.loanCategory = baseCollector.getSelectionFromOptions("Loan Category:", loanCategories);

                if (loan.validate()) {
                    application.addExistingLoan(loan);
                    cout << "✅ Loan #" << loanCount << " recorded successfully!" << endl;
                }
                else {
                    cout << "❌ Invalid loan data. Please re-enter this loan." << endl;
                    loanCount--;
                    continue;
                }

                if (loanCount < 10) {
                    cout << "Add another loan? (yes/no): ";
                    string another;
                    getline(cin, another);
                    addMoreLoans = (toLower(trim(another)) == "yes" || toLower(trim(another)) == "y");
                }
                else {
                    cout << "Maximum of 10 loans reached." << endl;
                    addMoreLoans = false;
                }
            }
            cout << "Total " << loanCount << " loan(s) recorded." << endl;
        }
        else {
            cout << "No existing loans recorded." << endl;
        }

        // Validate the section
        ValidationResult validation = application.validateFinancialInfo();
        if (!validation.isValid) {
            cout << endl << "❌ VALIDATION ERRORS:" << endl;
            cout << validation.getReport();
            cout << "Please correct the errors above." << endl;
            return false;
        }

        if (!validation.warnings.empty()) {
            cout << endl << "⚠️  VALIDATION WARNINGS:" << endl;
            cout << validation.getReport();
        }

        return saveSectionAndContinue(application, "financial");

    }
    catch (const exception& e) {
        cout << "Error collecting financial information: " << e.what() << endl;
        return false;
    }
}

bool MultiSessionCollector::collectReferencesInfo(LoanApplication& application) {
    cout << endl << "========================================" << endl;
    cout << "        REFERENCES INFORMATION" << endl;
    cout << "========================================" << endl;

    try {
        cout << "Please provide details for two references:" << endl << endl;

        cout << "--- REFERENCE 1 ---" << endl;
        Reference ref1;
        ref1.name = baseCollector.getValidatedString("Reference 1 Full Name: ", "Reference name", 1, 100);
        ref1.cnic = baseCollector.getValidatedCNIC("Reference 1 CNIC (13 digits): ");
        ref1.cnicIssueDate = baseCollector.getValidatedCNICIssueDate("Reference 1 CNIC Issue Date (DD-MM-YYYY): ");
        ref1.phoneNumber = baseCollector.getValidatedPhone("Reference 1 Phone Number: ");
        ref1.email = baseCollector.getValidatedEmail("Reference 1 Email Address: ");
        application.setReference1(ref1);

        cout << endl << "--- REFERENCE 2 ---" << endl;
        Reference ref2;
        ref2.name = baseCollector.getValidatedString("Reference 2 Full Name: ", "Reference name", 1, 100);
        ref2.cnic = baseCollector.getValidatedCNIC("Reference 2 CNIC (13 digits): ");
        ref2.cnicIssueDate = baseCollector.getValidatedCNICIssueDate("Reference 2 CNIC Issue Date (DD-MM-YYYY): ");
        ref2.phoneNumber = baseCollector.getValidatedPhone("Reference 2 Phone Number: ");
        ref2.email = baseCollector.getValidatedEmail("Reference 2 Email Address: ");
        application.setReference2(ref2);

        // Validate the section
        ValidationResult validation = application.validateReferences();
        if (!validation.isValid) {
            cout << endl << "❌ VALIDATION ERRORS:" << endl;
            cout << validation.getReport();
            cout << "Please correct the errors above." << endl;
            return false;
        }

        cout << endl << "✅ References information collected successfully!" << endl;
        return saveSectionAndContinue(application, "references");

    }
    catch (const exception& e) {
        cout << "Error collecting references: " << e.what() << endl;
        return false;
    }
}

bool MultiSessionCollector::collectDocumentsInfo(LoanApplication& application) {
    cout << endl << "========================================" << endl;
    cout << "        DOCUMENTS UPLOAD" << endl;
    cout << "========================================" << endl;

    try {
        cout << "Please provide file paths for required documents:" << endl;
        cout << "Note: Files will be copied to the application folder." << endl << endl;

        string cnicFront = baseCollector.getImagePath("CNIC Front Side");
        application.setCnicFrontImagePath(cnicFront);

        string cnicBack = baseCollector.getImagePath("CNIC Back Side");
        application.setCnicBackImagePath(cnicBack);

        string electricityBill = baseCollector.getImagePath("Recent Electricity Bill");
        application.setElectricityBillImagePath(electricityBill);

        string salarySlip = baseCollector.getImagePath("Salary Slip or Bank Statement (Page 1)");
        application.setSalarySlipImagePath(salarySlip);

        // Validate the section
        ValidationResult validation = application.validateDocuments();
        if (!validation.isValid) {
            cout << endl << "❌ VALIDATION ERRORS:" << endl;
            cout << validation.getReport();
            cout << "Please provide all required documents." << endl;
            return false;
        }

        // Mark as fully submitted
        application.setStatus("submitted");
        if (application.getSubmissionDate().empty()) {
            application.setSubmissionDate(getCurrentDate());
        }

        // Final save
        if (fileManager.saveApplication(application)) {
            cout << endl << "🎉 APPLICATION SUBMITTED SUCCESSFULLY! 🎉" << endl;
            cout << "========================================" << endl;
            cout << "Application ID: " << application.getApplicationId() << endl;
            cout << "Submission Date: " << application.getSubmissionDate() << endl;
            cout << "Status: " << application.getStatus() << endl;
            cout << "========================================" << endl;
            cout << "You can check your application status using your CNIC." << endl;
            return true;
        }
        else {
            cout << "❌ ERROR: Failed to save application." << endl;
            return false;
        }

    }
    catch (const exception& e) {
        cout << "Error collecting documents: " << e.what() << endl;
        return false;
    }
}

bool MultiSessionCollector::saveSectionAndContinue(LoanApplication& application, const string& section) {
    if (fileManager.updateApplicationSection(application, section)) {
        cout << endl << "✅ " << section << " information saved successfully!" << endl;
        displayApplicationProgress(application);

        if (section != "documents") {
            cout << endl << "Options:" << endl;
            cout << "1. Continue to next section" << endl;
            cout << "2. Save and exit (resume later)" << endl;
            cout << "Choose option: ";

            string choice;
            getline(cin, choice);
            choice = trim(choice);

            if (choice == "2") {
                cout << endl << "💾 Application saved successfully!" << endl;
                cout << "You can resume later with:" << endl;
                cout << "Application ID: " << application.getApplicationId() << endl;
                cout << "CNIC: " << application.getCnicNumber() << endl;
                cout << "Next section: " << application.getNextIncompleteSection() << endl;
                return false; // Stop collection
            }
        }
        return true; // Continue to next section
    }
    else {
        cout << "❌ ERROR: Failed to save " << section << " information." << endl;
        return false;
    }
}