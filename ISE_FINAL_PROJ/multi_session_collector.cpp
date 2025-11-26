#include "multi_session_collector.h"
#include "personal.h"
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
    cout << "4. Start general conversation mode" << endl;
    cout << "5. Return to main menu" << endl;
    cout << "========================================" << endl;
    cout << Config::CHATBOT_NAME << ": Please choose an option (1-5): ";
}

void MultiSessionCollector::displayApplicationProgress(const LoanApplication& application) {
    cout << endl << "========================================" << endl;
    cout << "   APPLICATION PROGRESS REPORT" << endl;
    cout << "========================================" << endl;
    cout << Config::CHATBOT_NAME << ": Let me show you where we are with your application!" << endl << endl;
    cout << "Application ID: " << application.getApplicationId() << endl;
    cout << "Current Status: " << getStatusDescription(application.getStatus()) << endl << endl;

    vector<string> sections = { "personal", "financial", "references", "documents" };
    vector<string> sectionNames = { "Personal Information", "Financial Information", "References", "Documents" };

    cout << "Progress Checklist:" << endl;
    for (size_t i = 0; i < sections.size(); i++) {
        bool completed = application.isSectionCompleted(sections[i]);
        cout << "  " << (completed ? "[V]" : "[ ]") << " " << sectionNames[i] << endl;
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

void MultiSessionCollector::selectPersonalLoanType(LoanApplication& application) {
    cout << endl << "========================================" << endl;
    cout << "   PERSONAL LOAN TYPE SELECTION" << endl;
    cout << "========================================" << endl;
    cout << Config::CHATBOT_NAME << ": What type of personal loan do you need?" << endl << endl;

    // You'll need to load personal loans here or pass them as parameter
    // For now, let's create a simple selection
    cout << "1. Standard Personal Loan - For any personal financial needs" << endl;
    cout << "2. Education Loan - For tuition, books, and educational expenses" << endl;
    cout << "3. Medical Loan - For medical treatments and healthcare expenses" << endl;
    cout << "4. Wedding Loan - For marriage and wedding expenses" << endl;
    cout << "5. Emergency Loan - For urgent and unexpected expenses" << endl;
    cout << "6. Home Renovation - For renovating or improving your home" << endl;

    cout << endl << Config::CHATBOT_NAME << ": Please choose an option (1-6): ";

    string loanChoice;
    if (!getline(cin, loanChoice)) {
        cout << Config::CHATBOT_NAME << ": Input error. Using standard personal loan." << endl;
        application.setLoanCategory("Standard Personal Loan");
        return;
    }

    loanChoice = trim(loanChoice);

    if (loanChoice == "1") {
        application.setLoanCategory("Standard Personal Loan");
        cout << Config::CHATBOT_NAME << ": Standard Personal Loan selected." << endl;
    }
    else if (loanChoice == "2") {
        application.setLoanCategory("Education Loan");
        cout << Config::CHATBOT_NAME << ": Education Loan selected." << endl;
    }
    else if (loanChoice == "3") {
        application.setLoanCategory("Medical Loan");
        cout << Config::CHATBOT_NAME << ": Medical Loan selected." << endl;
    }
    else if (loanChoice == "4") {
        application.setLoanCategory("Wedding Loan");
        cout << Config::CHATBOT_NAME << ": Wedding Loan selected." << endl;
    }
    else if (loanChoice == "5") {
        application.setLoanCategory("Emergency Loan");
        cout << Config::CHATBOT_NAME << ": Emergency Loan selected." << endl;
    }
    else if (loanChoice == "6") {
        application.setLoanCategory("Home Renovation");
        cout << Config::CHATBOT_NAME << ": Home Renovation Loan selected." << endl;
    }
    else {
        cout << Config::CHATBOT_NAME << ": Invalid choice. Using Standard Personal Loan." << endl;
        application.setLoanCategory("Standard Personal Loan");
    }

    // Set default values for personal loan
    application.setLoanAmount(500000);
    application.setDownPayment(50000);
    application.setInstallmentMonths(36);
    application.setMonthlyPayment(12500);
}
void MultiSessionCollector::startNewApplication() {
    try {
        cout << endl << "========================================" << endl;
        cout << "   STARTING NEW LOAN APPLICATION" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": Excellent! Let's start your loan application journey." << endl;
        cout << Config::CHATBOT_NAME << ": I'll guide you through each step carefully." << endl << endl;

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

            cout << endl << Config::CHATBOT_NAME << ": What type of personal loan do you need?" << endl;
            cout << "1. Standard Personal Loan - For any personal needs" << endl;
            cout << "2. Education Loan - For tuition and educational expenses" << endl;
            cout << "3. Medical Loan - For medical treatments" << endl;
            cout << "4. Wedding Loan - For marriage expenses" << endl;
            cout << "5. Emergency Loan - For urgent needs" << endl;
            cout << "6. Home Renovation - For home improvements" << endl;
            cout << Config::CHATBOT_NAME << ": Please choose (1-6): ";

            string personalType;
            getline(cin, personalType);
            personalType = trim(personalType);

            if (personalType == "1") loanDetails = "Standard Personal Loan";
            else if (personalType == "2") loanDetails = "Education Loan";
            else if (personalType == "3") loanDetails = "Medical Loan";
            else if (personalType == "4") loanDetails = "Wedding Loan";
            else if (personalType == "5") loanDetails = "Emergency Loan";
            else if (personalType == "6") loanDetails = "Home Renovation";
            else {
                cout << Config::CHATBOT_NAME << ": Invalid choice. Using Standard Personal Loan." << endl;
                loanDetails = "Standard Personal Loan";
            }
        }
        else {
            cout << Config::CHATBOT_NAME << ": Hmm, that's not a valid option. Let's try again." << endl;
            return;
        }

        LoanApplication application;
        application.setLoanType(loanType);
        application.setLoanCategory(loanDetails);
        application.setStatus("C1");

        string appId = fileManager.generateApplicationId();
        application.setApplicationId(appId);
        application.setSubmissionDate(getCurrentDate());

        cout << endl << "========================================" << endl;
        cout << "   APPLICATION CREATED" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": Your application has been created!" << endl;
        cout << endl << "Your Application ID: " << appId << endl;
        cout << "Loan Type: " << loanType << endl;
        cout << "Loan Category: " << loanDetails << endl;
        cout << "Date: " << getCurrentDate() << endl << endl;
        cout << "IMPORTANT: Please save this Application ID!" << endl;
        cout << "You'll need it if you want to continue later." << endl;
        cout << "========================================" << endl << endl;

        cout << Config::CHATBOT_NAME << ": Ready to start with your personal information? (yes/no): ";
        string ready;
        if (!getline(cin, ready)) {
            cout << Config::CHATBOT_NAME << ": Input error. Application saved with ID: " << appId << endl;
            fileManager.updateApplicationSection(application, "");
            return;
        }

        if (toLower(trim(ready)) != "yes" && toLower(trim(ready)) != "y") {
            fileManager.updateApplicationSection(application, "");
            cout << Config::CHATBOT_NAME << ": No problem! Your application has been saved." << endl;
            cout << Config::CHATBOT_NAME << ": Come back anytime with your Application ID: " << appId << endl;
            return;
        }

        // **FIX STARTS HERE** - Continue through all sections like resumeExistingApplication does
        bool continueProcessing = true;
        bool applicationWasSubmitted = false;

        while (continueProcessing && !application.isApplicationComplete() && !applicationWasSubmitted) {
            string nextSection = application.getNextIncompleteSection();

            if (nextSection == "complete") {
                break;
            }

            cout << endl << "========================================" << endl;
            cout << "   SECTION: " << getSectionDisplayName(nextSection) << endl;
            cout << "========================================" << endl;

            bool sectionCompleted = false;

            if (nextSection == "personal") {
                sectionCompleted = collectPersonalInfo(application);
            }
            else if (nextSection == "financial") {
                sectionCompleted = collectFinancialInfo(application);
            }
            else if (nextSection == "references") {
                sectionCompleted = collectReferencesInfo(application);
            }
            else if (nextSection == "documents") {
                sectionCompleted = collectDocumentsInfo(application);
                if (sectionCompleted && application.getStatus() == "submitted") {
                    applicationWasSubmitted = true;
                    break;
                }
            }
            else {
                break;
            }

            if (sectionCompleted && !applicationWasSubmitted) {
                if (application.getNextIncompleteSection() != "complete") {
                    cout << endl << Config::CHATBOT_NAME << ": Continue to next section? (yes/no): ";
                    string nextChoice;
                    getline(cin, nextChoice);
                    continueProcessing = (toLower(trim(nextChoice)) == "yes" || toLower(trim(nextChoice)) == "y");

                    if (!continueProcessing) {
                        cout << endl << Config::CHATBOT_NAME << ": Your progress has been saved!" << endl;
                        cout << "Application ID: " << appId << endl;
                        cout << "You can continue anytime by selecting 'Continue my incomplete application'" << endl;
                        return;
                    }
                }
                else {
                    continueProcessing = false;
                }
            }
            else {
                continueProcessing = sectionCompleted;
            }
        }

        if (applicationWasSubmitted || application.isApplicationComplete()) {
            cout << endl << "========================================" << endl;
            cout << "   CONGRATULATIONS!" << endl;
            cout << "========================================" << endl;
            cout << Config::CHATBOT_NAME << ": You've successfully completed your application!" << endl;
            cout << endl << "Application Details:" << endl;
            cout << "  Application ID: " << application.getApplicationId() << endl;
            cout << "  Status: Submitted for Review" << endl;
            cout << "  Submission Date: " << application.getSubmissionDate() << endl << endl;
            cout << Config::CHATBOT_NAME << ": We'll review your application and get back to you soon!" << endl;
            cout << "========================================" << endl;
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
        cout << "   APPLICATION NOT FOUND" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": I couldn't find an incomplete application with:" << endl;
        cout << "  Application ID: " << applicationId << endl;
        cout << "  CNIC: " << cnic << endl << endl;
        cout << Config::CHATBOT_NAME << ": Would you like to start a new application instead? (yes/no): ";
        string choice;
        getline(cin, choice);
        if (toLower(trim(choice)) == "yes" || toLower(trim(choice)) == "y") {
            startNewApplication();
        }
        return;
    }

    if (application.isApplicationComplete() || application.getStatus() == "submitted") {
        cout << endl << "========================================" << endl;
        cout << "   APPLICATION ALREADY SUBMITTED" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": Your application is already submitted!" << endl;
        cout << "Application ID: " << application.getApplicationId() << endl;
        cout << "Status: " << application.getStatus() << endl;
        cout << "Submission Date: " << application.getSubmissionDate() << endl;
        cout << Config::CHATBOT_NAME << ": No further action is needed." << endl;
        return;
    }

    cout << Config::CHATBOT_NAME << ": Found it!" << endl;
    displayApplicationProgress(application);

    while (true) {
        cout << endl << Config::CHATBOT_NAME << ": Which section would you like to update?" << endl;
        cout << "1. Personal Information" << endl;
        cout << "2. Financial Information" << endl;
        cout << "3. References" << endl;
        cout << "4. Documents" << endl;
        cout << "5. Continue from next incomplete section" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice (1-6): ";
        string choice;
        getline(cin, choice);
        choice = trim(choice);

        bool updated = false;
        if (choice == "1") {
            updated = collectPersonalInfo(application);
        }
        else if (choice == "2") {
            updated = collectFinancialInfo(application);
        }
        else if (choice == "3") {
            updated = collectReferencesInfo(application);
        }
        else if (choice == "4") {
            updated = collectDocumentsInfo(application);
        }
        else if (choice == "5") {
            break; 
        }
        else if (choice == "6") {
            cout << Config::CHATBOT_NAME << ": No problem! Your progress is saved. Come back anytime!" << endl;
            return;
        }
        else {
            cout << Config::CHATBOT_NAME << ": Invalid choice. Please select 1-6." << endl;
            continue;
        }

        if (application.isApplicationComplete() || application.getStatus() == "submitted") {
            cout << endl << "========================================" << endl;
            cout << "   CONGRATULATIONS!" << endl;
            cout << "========================================" << endl;
            cout << Config::CHATBOT_NAME << ": You've successfully completed your application!" << endl;
            cout << endl << "Application Details:" << endl;
            cout << "  Application ID: " << application.getApplicationId() << endl;
            cout << "  Status: Submitted for Review" << endl;
            cout << "  Submission Date: " << application.getSubmissionDate() << endl << endl;
            cout << Config::CHATBOT_NAME << ": We'll review your application and get back to you soon!" << endl;
            cout << "========================================" << endl;
            return;
        }
    }

    // Continue original resume loop for incomplete sections
    bool continueProcessing = true;
    bool applicationWasSubmitted = false;

    while (continueProcessing && !application.isApplicationComplete() && !applicationWasSubmitted) {
        string nextSection = application.getNextIncompleteSection();
        cout << endl << "========================================" << endl;
        cout << "   SECTION: " << getSectionDisplayName(nextSection) << endl;
        cout << "========================================" << endl;

        bool sectionCompleted = false;

        if (nextSection == "personal") {
            sectionCompleted = collectPersonalInfo(application);
        }
        else if (nextSection == "financial") {
            sectionCompleted = collectFinancialInfo(application);
        }
        else if (nextSection == "references") {
            sectionCompleted = collectReferencesInfo(application);
        }
        else if (nextSection == "documents") {
            sectionCompleted = collectDocumentsInfo(application);
            if (sectionCompleted && application.getStatus() == "submitted") {
                applicationWasSubmitted = true;
                break;
            }
        }
        else {
            break;
        }

        if (sectionCompleted && !applicationWasSubmitted) {
            LoanApplication updatedApp = fileManager.findIncompleteApplication(applicationId, cnic);
            if (!updatedApp.getApplicationId().empty()) {
                application = updatedApp;
            }

            if (application.getNextIncompleteSection() != "complete") {
                cout << endl << Config::CHATBOT_NAME << ": Continue to next section? (yes/no): ";
                string nextChoice;
                getline(cin, nextChoice);
                continueProcessing = (toLower(trim(nextChoice)) == "yes" || toLower(trim(nextChoice)) == "y");
            }
            else {
                continueProcessing = false;
            }
        }
        else {
            continueProcessing = sectionCompleted;
        }
    }

    if (applicationWasSubmitted || application.isApplicationComplete()) {
        cout << endl << "========================================" << endl;
        cout << "   CONGRATULATIONS!" << endl;
        cout << "========================================" << endl;
        cout << Config::CHATBOT_NAME << ": You've successfully completed your application!" << endl;
        cout << endl << "Application Details:" << endl;
        cout << "  Application ID: " << application.getApplicationId() << endl;
        cout << "  Status: Submitted for Review" << endl;
        cout << "  Submission Date: " << application.getSubmissionDate() << endl << endl;
        cout << Config::CHATBOT_NAME << ": We'll review your application and get back to you soon!" << endl;
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

        ValidationResult validation = application.validatePersonalInfo();
        if (!validation.isValid) {
            cout << endl << Config::CHATBOT_NAME << ": Oops! There are some issues:" << endl;
            cout << validation.getReport();
            return false;
        }

        // Save the section but DON'T show "Continue to next section?" here
        // Let startNewApplication() handle that
        saveSectionAndContinue(application, "personal");
        return true;  // Return true to indicate success

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
                    cout << Config::CHATBOT_NAME << ": Loan #" << loanCount << " recorded!" << endl;
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

        // Save the section but let startNewApplication() ask about continuing
        saveSectionAndContinue(application, "financial");
        return true;

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

        // Save and return success
        saveSectionAndContinue(application, "references");
        return true;

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Error: " << e.what() << endl;
        return false;
    }
}
bool MultiSessionCollector::collectDocumentsInfo(LoanApplication& application) {
    // Check if application is already submitted
    if (application.isApplicationComplete() || application.getStatus() == "submitted") {
        cout << Config::CHATBOT_NAME << ": Your application is already submitted!" << endl;
        return true;
    }

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

            // Don't submit if documents are invalid
            application.setStatus("incomplete_documents");
            fileManager.updateApplicationSection(application, "documents");
            return false;
        }

        // All documents are valid - submit the application
        application.setStatus("submitted");
        if (application.getSubmissionDate().empty()) {
            application.setSubmissionDate(getCurrentDate());
        }

        // Mark documents as completed first
        application.markSectionCompleted("documents");

        // Update the existing record instead of creating new one
        if (fileManager.updateApplicationSection(application, "documents")) {
            cout << endl << "========================================" << endl;
            cout << "   APPLICATION SUBMITTED!" << endl;
            cout << "========================================" << endl;
            cout << Config::CHATBOT_NAME << ": Congratulations! Your application is complete!" << endl;
            cout << endl << "Details:" << endl;
            cout << "  Application ID: " << application.getApplicationId() << endl;
            cout << "  Date: " << application.getSubmissionDate() << endl;
            cout << "  Status: Submitted for Review" << endl << endl;
            cout << Config::CHATBOT_NAME << ": We'll review your application and contact you soon!" << endl;
            cout << "========================================" << endl;
            return true;
        }
        else {
            cout << Config::CHATBOT_NAME << ": Failed to save application." << endl;
            return false;
        }

    }
    catch (const exception& e) {
        cout << Config::CHATBOT_NAME << ": Error: " << e.what() << endl;
        return false;
    }
}

bool MultiSessionCollector::saveSectionAndContinue(LoanApplication& application, const string& section) {
    // Move section progression logic here if needed
    if (section == "personal") {
        application.setStatus("C2");
    }
    else if (section == "financial") {
        application.setStatus("C3");
    }
    else if (section == "references") {
        application.setStatus("incomplete_documents");
    }

    // Mark section as completed if not already
    if (!application.isSectionCompleted(section)) {
        string currentSections = application.getCompletedSections();
        if (!currentSections.empty()) {
            currentSections += ",";
        }
        currentSections += section;
        application.setCompletedSections(currentSections);
    }

    // Save progress
    if (fileManager.updateApplicationSection(application, section)) {
        cout << endl << Config::CHATBOT_NAME << ": " << getSectionDisplayName(section)
            << " saved successfully!" << endl;
        return true;
    }
    else {
        cout << Config::CHATBOT_NAME << ": Failed to save. Please try again." << endl;
        return false;
    }
}
