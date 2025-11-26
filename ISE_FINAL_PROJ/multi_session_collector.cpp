#include "multi_session_collector.h"
#include <iomanip>

using namespace std;

void MultiSessionCollector::showApplicationMenu() {
    cout << endl << "========================================" << endl;
    cout << "   " << Config::CHATBOT_NAME << "'S LOAN APPLICATION CENTER" << endl;
    cout << "========================================" << endl;
    cout << Config::CHATBOT_NAME << ": Hello! How can I help you today?" << endl << endl;
    cout << "1. Start a brand new loan application" << endl;
    cout << "2. Continue my incomplete application" << endl;
    cout << "3. Check my application status" << endl;
    cout << "4. Return to main menu" << endl;
    cout << "========================================" << endl;
    cout << Config::CHATBOT_NAME << ": Please choose an option (1-4): ";
}

void MultiSessionCollector::displayApplicationProgress(const LoanApplication& application) {
    cout << endl << "========================================" << endl;
    cout << "   APPLICATION PROGRESS REPORT" << endl;
    cout << "========================================" << endl;
    cout << Config::CHATBOT_NAME << ": Let me show you where we are with your application!" << endl << endl;
    cout << "Application ID: " << application.getApplicationId() << endl;
    cout << "Current Status: " << getStatusDescription(application.getStatus()) << endl << endl;

    // Show progress with checkmarks
    vector<string> sections = { "personal", "financial", "references", "documents" };
    vector<string> sectionNames = { "Personal Information", "Financial Information", "References", "Documents" };

    cout << "Progress Checklist:" << endl;
    for (size_t i = 0; i < sections.size(); i++) {
        bool completed = application.isSectionCompleted(sections[i]);
        cout << "  " << (completed ? "[✓]" : "[ ]") << " " << sectionNames[i] << endl;
    }

    string nextSection = application.getNextIncompleteSection();
    if (nextSection != "complete") {
        cout << endl << Config::CHATBOT_NAME << ": Next step: " << getSectionDisplayName(nextSection) << endl;
    }
    else {
        cout << endl << Config::CHATBOT_NAME << ": Great news! Your application is complete!" << endl;
    }
    cout << "========================================" << endl;
}

string MultiSessionCollector::getStatusDescription(const string& status) const {
    if (status == "incomplete_personal" || status == "C1") return "Personal Info Needed";
    if (status == "incomplete_financial" || status == "C2") return "Financial Info Needed";
    if (status == "incomplete_references") return "References Needed";
    if (status == "incomplete_documents" || status == "C3") return "Documents Needed";
    if (status == "submitted") return "Submitted for Review";
    if (status == "approved") return "Approved";
    if (status == "rejected") return "Rejected";
    return status;
}

string MultiSessionCollector::getSectionDisplayName(const string& section) const {
    if (section == "personal") return "Personal Information";
    if (section == "financial") return "Financial Information";
    if (section == "references") return "References";
    if (section == "documents") return "Documents";
    return section;
}

void MultiSessionCollector::startNewApplication() {
    try {
        cout << endl << "========================================" << endl;
        cout << "   STARTING NEW LOAN APPLICATION" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": Excellent! Let's start your loan application journey." << endl;
        cout << Config::CHATBOT_NAME << ": I'll guide you through each step carefully." << endl << endl;

        // Loan type selection
        cout << Config::CHATBOT_NAME << ": First, what type of loan are you interested in?" << endl << endl;
        cout << "1. Home Loan - For purchasing or constructing your dream home" << endl;
        cout << "2. Car Loan - For buying a new or used vehicle" << endl;
        cout << "3. Scooter Loan - For eco-friendly transportation" << endl;
        cout << "4. Personal Loan - For any personal financial needs" << endl << endl;
        cout << Config::CHATBOT_NAME << ": Please enter your choice (1-4): ";

        string loanChoice;
        if (!getline(cin, loanChoice)) {
            cout << Config::CHATBOT_NAME << ": Input error. Returning to menu." << endl;
            return;
        }
        loanChoice = trim(loanChoice);

        string loanType, loanDetails;
        if (loanChoice == "1") {
            loanType = "home";
            loanDetails = "Home Loan";
            cout << Config::CHATBOT_NAME << ": Great choice! Home loans are perfect for building your future." << endl;
        }
        else if (loanChoice == "2") {
            loanType = "car";
            loanDetails = "Car Loan";
            cout << Config::CHATBOT_NAME << ": Wonderful! Let's get you on the road." << endl;
        }
        else if (loanChoice == "3") {
            loanType = "scooter";
            loanDetails = "Scooter Loan";
            cout << Config::CHATBOT_NAME << ": Smart choice for city commuting!" << endl;
        }
        else if (loanChoice == "4") {
            loanType = "personal";
            loanDetails = "Personal Loan";
            cout << Config::CHATBOT_NAME << ": Personal loans offer great flexibility!" << endl;
        }
        else {
            cout << Config::CHATBOT_NAME << ": Hmm, that's not a valid option. Let's try again." << endl;
            return;
        }

        LoanApplication application;
        application.setLoanType(loanType);
        application.setLoanCategory(loanDetails);
        application.setStatus("C1"); // Checkpoint 1 - Personal Info needed

        // Generate application ID
        string appId = fileManager.generateApplicationId();
        application.setApplicationId(appId);
        application.setSubmissionDate(getCurrentDate());

        cout << endl << "========================================" << endl;
        cout << "   🎉 APPLICATION CREATED 🎉" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": Your application has been created!" << endl;
        cout << endl << "Your Application ID: " << appId << endl;
        cout << "Loan Type: " << loanDetails << endl;
        cout << "Date: " << getCurrentDate() << endl << endl;
        cout << "⚠️  IMPORTANT: Please save this Application ID!" << endl;
        cout << "   You'll need it if you want to continue later." << endl;
        cout << "========================================" << endl << endl;

        cout << Config::CHATBOT_NAME << ": Ready to start with your personal information? (yes/no): ";
        string ready;
        if (!getline(cin, ready)) {
            cout << Config::CHATBOT_NAME << ": Input error. Application saved with ID: " << appId << endl;
            fileManager.updateApplicationSection(application, "");
            return;
        }

        if (toLower(trim(ready)) == "yes" || toLower(trim(ready)) == "y") {
            collectPersonalInfo(application);
        }
        else {
            // Save incomplete application
            fileManager.updateApplicationSection(application, "");
            cout << Config::CHATBOT_NAME << ": No problem! Your application has been saved." << endl;
            cout << Config::CHATBOT_NAME << ": Come back anytime with your Application ID: " << appId << endl;
        }
    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Error: " << e.what() << endl;
        cout << Config::CHATBOT_NAME << ": Returning to main menu." << endl;
    }
}

void MultiSessionCollector::resumeExistingApplication() {
    cout << endl << "========================================" << endl;
    cout << "   RESUME YOUR APPLICATION" << endl;
    cout << "========================================" << endl;
    cout << Config::CHATBOT_NAME << ": Welcome back! Let's continue where you left off." << endl << endl;

    string applicationId, cnic;

    cout << Config::CHATBOT_NAME << ": Please enter your Application ID: ";
    getline(cin, applicationId);
    applicationId = trim(applicationId);

    if (applicationId.empty()) {
        cout << Config::CHATBOT_NAME << ": Oops! Application ID cannot be empty." << endl;
        return;
    }

    cout << Config::CHATBOT_NAME << ": Now, please enter your CNIC (13 digits, no dashes): ";
    getline(cin, cnic);
    cnic = trim(cnic);

    // Validate CNIC
    if (cnic.length() != 13) {
        cout << Config::CHATBOT_NAME << ": Sorry, CNIC must be exactly 13 digits." << endl;
        cout << Config::CHATBOT_NAME << ": Example: 3520212345678" << endl;
        return;
    }

    for (char c : cnic) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            cout << Config::CHATBOT_NAME << ": Sorry, CNIC should contain only numbers." << endl;
            return;
        }
    }

    cout << endl << Config::CHATBOT_NAME << ": Let me search for your application..." << endl;

    LoanApplication application = fileManager.findIncompleteApplication(applicationId, cnic);

    if (application.getApplicationId().empty()) {
        cout << endl << "========================================" << endl;
        cout << "   ❌ APPLICATION NOT FOUND" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": I couldn't find an incomplete application with:" << endl;
        cout << "  • Application ID: " << applicationId << endl;
        cout << "  • CNIC: " << cnic << endl << endl;
        cout << Config::CHATBOT_NAME << ": This could mean:" << endl;
        cout << "  1. The Application ID might be incorrect" << endl;
        cout << "  2. The CNIC doesn't match our records" << endl;
        cout << "  3. The application might already be submitted" << endl;
        cout << "  4. The application doesn't exist" << endl << endl;
        cout << Config::CHATBOT_NAME << ": Would you like to start a new application instead? (yes/no): ";
        string choice;
        getline(cin, choice);
        if (toLower(trim(choice)) == "yes" || toLower(trim(choice)) == "y") {
            startNewApplication();
        }
        return;
    }

    cout << Config::CHATBOT_NAME << ": Found it! 🎉" << endl;
    displayApplicationProgress(application);

    cout << endl << Config::CHATBOT_NAME << ": Shall we continue? (yes/no): ";
    string continueChoice;
    getline(cin, continueChoice);

    if (toLower(trim(continueChoice)) != "yes" && toLower(trim(continueChoice)) != "y") {
        cout << Config::CHATBOT_NAME << ": No problem! Your progress is saved. Come back anytime!" << endl;
        return;
    }

    // Continue from where user left off
    bool continueProcessing = true;
    while (continueProcessing && !application.isApplicationComplete()) {
        string nextSection = application.getNextIncompleteSection();
        cout << endl << "========================================" << endl;
        cout << "   SECTION: " << getSectionDisplayName(nextSection) << endl;
        cout << "========================================" << endl;

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

        // Refresh the application from file to get updated status
        if (continueProcessing) {
            application = fileManager.findIncompleteApplication(applicationId, cnic);
        }
    }

    if (application.isApplicationComplete()) {
        cout << endl << "========================================" << endl;
        cout << "   🎉 CONGRATULATIONS! 🎉" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": You've successfully completed your application!" << endl;
        cout << endl << "Application Details:" << endl;
        cout << "  • Application ID: " << application.getApplicationId() << endl;
        cout << "  • Status: Submitted for Review" << endl;
        cout << "  • Submission Date: " << application.getSubmissionDate() << endl << endl;
        cout << Config::CHATBOT_NAME << ": We'll review your application and get back to you soon!" << endl;
        cout << Config::CHATBOT_NAME << ": You can check your status anytime using your CNIC." << endl;
        cout << "========================================" << endl;
    }
}
bool MultiSessionCollector::collectPersonalInfo(LoanApplication& application) {
    cout << endl << Config::CHATBOT_NAME << ": Let's start with your personal information." << endl;
    cout << Config::CHATBOT_NAME << ": This helps us understand you better!" << endl << endl;

    try {
        string fullName = baseCollector.getValidatedString(
            "What is your full name? ", "Full name", 1, 100);
        application.setFullName(fullName);
        cout << Config::CHATBOT_NAME << ": Nice to meet you, " << fullName << "!" << endl << endl;

        string fathersName = baseCollector.getValidatedString(
            "What is your father's name? ", "Father's name", 1, 100);
        application.setFathersName(fathersName);

        string postalAddress = baseCollector.getValidatedString(
            "What is your complete postal address? ", "Postal address", 1, 255);
        application.setPostalAddress(postalAddress);

        string contactNumber = baseCollector.getValidatedPhone(
            "What is your contact number? (11 digits): ");
        application.setContactNumber(contactNumber);

        string email = baseCollector.getValidatedEmail(
            "What is your email address? ");
        application.setEmailAddress(email);

        string cnic = baseCollector.getValidatedCNIC(
            "What is your CNIC? (13 digits, no dashes): ");
        application.setCnicNumber(cnic);

        string cnicExpiry = baseCollector.getValidatedCNICExpiryDate(
            "When does your CNIC expire? (DD-MM-YYYY): ");
        application.setCnicExpiryDate(cnicExpiry);

        cout << endl << Config::CHATBOT_NAME << ": Now, let me ask about your employment and family..." << endl << endl;

        vector<string> employmentOptions = { "Self-employed", "Salaried", "Retired", "Unemployed" };
        string employmentStatus = baseCollector.getSelectionFromOptions(
            "What is your employment status?", employmentOptions);
        application.setEmploymentStatus(employmentStatus);

        vector<string> maritalOptions = { "Single", "Married", "Divorced", "Widowed" };
        string maritalStatus = baseCollector.getSelectionFromOptions(
            "What is your marital status?", maritalOptions);
        application.setMaritalStatus(maritalStatus);

        vector<string> genderOptions = { "Male", "Female", "Other" };
        string gender = baseCollector.getSelectionFromOptions(
            "What is your gender?", genderOptions);
        application.setGender(gender);

        int dependents = baseCollector.getValidatedNumeric(
            "How many dependents do you have? ", "Dependents", 0, 20);
        application.setNumberOfDependents(dependents);

        // Validate
        ValidationResult validation = application.validatePersonalInfo();
        if (!validation.isValid) {
            cout << endl << Config::CHATBOT_NAME << ": Oops! There are some issues:" << endl;
            cout << validation.getReport();
            return false;
        }

        return saveSectionAndContinue(application, "personal");

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Sorry, something went wrong: " << e.what() << endl;
        return false;
    }
}

bool MultiSessionCollector::collectFinancialInfo(LoanApplication& application) {
    cout << endl << Config::CHATBOT_NAME << ": Now let's talk about your finances." << endl;
    cout << Config::CHATBOT_NAME << ": This information helps us determine your loan eligibility." << endl << endl;

    try {
        long long annualIncome = baseCollector.getValidatedNumeric(
            "What is your annual income in PKR? (numbers only, no commas): ",
            "Annual income", 0, 1000000000);
        application.setAnnualIncome(annualIncome);

        cout << endl << Config::CHATBOT_NAME << ": Great! Now about your electricity bills..." << endl << endl;

        long long avgElectricityBill = baseCollector.getValidatedNumeric(
            "What is your average monthly electricity bill? (PKR): ",
            "Average electricity bill", 0, 100000);
        application.setAvgElectricityBill(avgElectricityBill);

        long long currentElectricityBill = baseCollector.getValidatedNumeric(
            "What is your current electricity bill amount? (PKR): ",
            "Current electricity bill", 0, 100000);
        application.setCurrentElectricityBill(currentElectricityBill);

        // Existing loans
        cout << endl << Config::CHATBOT_NAME << ": Do you have any existing loans? (yes/no): ";
        string hasLoans;
        getline(cin, hasLoans);
        hasLoans = toLower(trim(hasLoans));

        if (hasLoans == "yes" || hasLoans == "y") {
            cout << Config::CHATBOT_NAME << ": Okay, let me collect details about your existing loans." << endl;
            application.clearExistingLoans();

            bool addMoreLoans = true;
            int loanCount = 0;

            while (addMoreLoans && loanCount < 10) {
                loanCount++;
                cout << endl << "--- Loan #" << loanCount << " ---" << endl;
                ExistingLoan loan;

                vector<string> activeOptions = { "Yes - Active", "No - Closed" };
                string activeChoice = baseCollector.getSelectionFromOptions(
                    "Is this loan currently active?", activeOptions);
                loan.isActive = (activeChoice.find("Yes") != string::npos);

                loan.totalAmount = baseCollector.getValidatedNumeric(
                    "Total loan amount (PKR): ", "Total amount", 1000, 1000000000);

                if (loan.isActive) {
                    loan.amountReturned = baseCollector.getValidatedNumeric(
                        "Amount already paid (PKR): ", "Amount returned", 0, loan.totalAmount);
                    loan.amountDue = loan.totalAmount - loan.amountReturned;
                    cout << Config::CHATBOT_NAME << ": Remaining amount: PKR " << loan.amountDue << endl;
                }
                else {
                    loan.amountReturned = loan.totalAmount;
                    loan.amountDue = 0;
                    cout << Config::CHATBOT_NAME << ": Marked as fully paid." << endl;
                }

                loan.bankName = baseCollector.getValidatedString(
                    "Bank name: ", "Bank name", 2, 100);

                vector<string> loanCategories = { "Car", "Home", "Bike", "Personal", "Business", "Other" };
                loan.loanCategory = baseCollector.getSelectionFromOptions(
                    "Loan type:", loanCategories);

                if (loan.validate()) {
                    application.addExistingLoan(loan);
                    cout << Config::CHATBOT_NAME << ": ✓ Loan #" << loanCount << " recorded!" << endl;
                }

                if (loanCount < 10) {
                    cout << endl << Config::CHATBOT_NAME << ": Do you have another loan to add? (yes/no): ";
                    string another;
                    getline(cin, another);
                    addMoreLoans = (toLower(trim(another)) == "yes" || toLower(trim(another)) == "y");
                }
            }
        }

        ValidationResult validation = application.validateFinancialInfo();
        if (!validation.isValid) {
            cout << endl << Config::CHATBOT_NAME << ": There are some issues:" << endl;
            cout << validation.getReport();
            return false;
        }

        return saveSectionAndContinue(application, "financial");

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Error: " << e.what() << endl;
        return false;
    }
}
bool MultiSessionCollector::collectReferencesInfo(LoanApplication& application) {
    cout << endl << Config::CHATBOT_NAME << ": We need two references who can vouch for you." << endl;
    cout << Config::CHATBOT_NAME << ": These should be people who know you well." << endl << endl;

    try {
        cout << "--- REFERENCE 1 ---" << endl;
        Reference ref1;
        ref1.name = baseCollector.getValidatedString(
            "Reference 1 - Full name: ", "Reference name", 1, 100);
        ref1.cnic = baseCollector.getValidatedCNIC(
            "Reference 1 - CNIC (13 digits): ");
        ref1.cnicIssueDate = baseCollector.getValidatedCNICIssueDate(
            "Reference 1 - CNIC issue date (DD-MM-YYYY): ");
        ref1.phoneNumber = baseCollector.getValidatedPhone(
            "Reference 1 - Phone number: ");
        ref1.email = baseCollector.getValidatedEmail(
            "Reference 1 - Email: ");
        application.setReference1(ref1);

        cout << endl << "--- REFERENCE 2 ---" << endl;
        Reference ref2;
        ref2.name = baseCollector.getValidatedString(
            "Reference 2 - Full name: ", "Reference name", 1, 100);
        ref2.cnic = baseCollector.getValidatedCNIC(
            "Reference 2 - CNIC (13 digits): ");
        ref2.cnicIssueDate = baseCollector.getValidatedCNICIssueDate(
            "Reference 2 - CNIC issue date (DD-MM-YYYY): ");
        ref2.phoneNumber = baseCollector.getValidatedPhone(
            "Reference 2 - Phone number: ");
        ref2.email = baseCollector.getValidatedEmail(
            "Reference 2 - Email: ");
        application.setReference2(ref2);

        ValidationResult validation = application.validateReferences();
        if (!validation.isValid) {
            cout << endl << Config::CHATBOT_NAME << ": Issues found:" << endl;
            cout << validation.getReport();
            return false;
        }

        cout << endl << Config::CHATBOT_NAME << ": ✓ References recorded successfully!" << endl;
        return saveSectionAndContinue(application, "references");

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Error: " << e.what() << endl;
        return false;
    }
}

bool MultiSessionCollector::collectDocumentsInfo(LoanApplication& application) {
    cout << endl << Config::CHATBOT_NAME << ": Finally, we need some document scans/photos." << endl;
    cout << Config::CHATBOT_NAME << ": Please provide the file paths from your computer." << endl << endl;

    try {
        string cnicFront = baseCollector.getImagePath("CNIC Front Side");
        application.setCnicFrontImagePath(cnicFront);

        string cnicBack = baseCollector.getImagePath("CNIC Back Side");
        application.setCnicBackImagePath(cnicBack);

        string electricityBill = baseCollector.getImagePath("Recent Electricity Bill");
        application.setElectricityBillImagePath(electricityBill);

        string salarySlip = baseCollector.getImagePath("Salary Slip / Bank Statement");
        application.setSalarySlipImagePath(salarySlip);

        ValidationResult validation = application.validateDocuments();
        if (!validation.isValid) {
            cout << endl << Config::CHATBOT_NAME << ": Missing documents:" << endl;
            cout << validation.getReport();
            return false;
        }

        // Mark as submitted
        application.setStatus("submitted");
        if (application.getSubmissionDate().empty()) {
            application.setSubmissionDate(getCurrentDate());
        }

        if (fileManager.saveApplication(application)) {
            cout << endl << "========================================" << endl;
            cout << "   🎉 APPLICATION SUBMITTED! 🎉" << endl;
            cout << "========================================" << endl;
            cout << Config::CHATBOT_NAME << ": Congratulations! Your application is complete!" << endl;
            cout << endl << "Details:" << endl;
            cout << "  • Application ID: " << application.getApplicationId() << endl;
            cout << "  • Date: " << application.getSubmissionDate() << endl;
            cout << "  • Status: Submitted for Review" << endl << endl;
            cout << Config::CHATBOT_NAME << ": We'll review your application and contact you soon!" << endl;
            cout << "========================================" << endl;
            return true;
        }
        else {
            cout << Config::CHATBOT_NAME << ": ❌ Failed to save application." << endl;
            return false;
        }

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Error: " << e.what() << endl;
        return false;
    }
}

bool MultiSessionCollector::saveSectionAndContinue(LoanApplication& application, const string& section) {
    // Update status based on checkpoint
    if (section == "personal") {
        application.setStatus("C2"); // Checkpoint 2 - Financial needed
    }
    else if (section == "financial") {
        application.setStatus("C3"); // Checkpoint 3 - References needed
    }
    else if (section == "references") {
        application.setStatus("incomplete_documents"); // Documents needed
    }

    // Mark section as completed
    if (!application.isSectionCompleted(section)) {
        string currentSections = application.getCompletedSections();
        if (!currentSections.empty()) {
            currentSections += ",";
        }
        currentSections += section;
        application.setCompletedSections(currentSections);
    }

    if (fileManager.updateApplicationSection(application, section)) {
        cout << endl << Config::CHATBOT_NAME << ": ✓ " << getSectionDisplayName(section)
            << " saved successfully!" << endl;

        displayApplicationProgress(application);

        if (section != "documents") {
            cout << endl << Config::CHATBOT_NAME << ": What would you like to do?" << endl;
            cout << "1. Continue to next section" << endl;
            cout << "2. Save and exit (I'll come back later)" << endl;
            cout << Config::CHATBOT_NAME << ": Choose option (1 or 2): ";

            string choice;
            getline(cin, choice);
            choice = trim(choice);

            if (choice == "2") {
                cout << endl << Config::CHATBOT_NAME << ": 💾 Your progress has been saved!" << endl;
                cout << Config::CHATBOT_NAME << ": To continue later, you'll need:" << endl;
                cout << "  • Application ID: " << application.getApplicationId() << endl;
                cout << "  • Your CNIC: " << application.getCnicNumber() << endl;
                cout << Config::CHATBOT_NAME << ": See you soon!" << endl;
                return false;
            }
            return true; // Continue to next section
        }
        return true; // Documents completed
    }
    else {
        cout << Config::CHATBOT_NAME << ": ❌ Failed to save. Please try again." << endl;
        return false;
    }
}