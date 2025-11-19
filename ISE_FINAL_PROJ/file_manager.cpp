#include "file_manager.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

FileManager::FileManager(const string& appsFile, const string& imagesDir)
    : applicationsFile(appsFile), imagesDirectory(imagesDir) {
    // Simple constructor - no complex setup needed
}

string FileManager::generateApplicationId() const {
    // Simple counter-based ID
    static int counter = 1001; // Start from 1001

    // Read existing applications to find the highest ID
    ifstream file(applicationsFile);
    string line;
    int maxId = 1000;

    while (getline(file, line)) {
        if (!line.empty()) {
            vector<string> parts = splitString(line, Config::DELIMITER);
            if (!parts.empty()) {
                try {
                    int currentId = stoi(parts[0]);
                    if (currentId > maxId) {
                        maxId = currentId;
                    }
                }
                catch (...) {
                    // Ignore non-numeric IDs
                }
            }
        }
    }
    file.close();

    // Use the next available ID
    stringstream ss;
    ss << setw(4) << setfill('0') << (maxId + 1);
    return ss.str();
}

bool FileManager::fileExists(const string& filename) const {
    ifstream file(filename);
    return file.good();
}

// REPLACE the copyImageFile method in file_manager.cpp

bool FileManager::copyImageFile(const string& sourcePath, const string& destinationPath) const {
    // Verify source file exists and is readable
    ifstream sourceCheck(sourcePath, ios::binary | ios::ate);
    if (!sourceCheck.is_open()) {
        cerr << "❌ ERROR: Cannot open source file for reading" << endl;
        return false;
    }

    streamsize sourceSize = sourceCheck.tellg();
    sourceCheck.close();

    if (sourceSize == 0) {
        cerr << "❌ ERROR: Source file is empty" << endl;
        return false;
    }

    // Open source file
    ifstream source(sourcePath, ios::binary);
    if (!source.is_open()) {
        cerr << "❌ ERROR: Failed to open source file for copying" << endl;
        return false;
    }

    // Open destination file
    ofstream destination(destinationPath, ios::binary);
    if (!destination.is_open()) {
        cerr << "❌ ERROR: Failed to create destination file" << endl;
        source.close();
        return false;
    }

    // Copy with buffer (without verbose output)
    const size_t bufferSize = 8192;
    char buffer[bufferSize];
    bool success = true;

    while (source.read(buffer, bufferSize) || source.gcount() > 0) {
        streamsize bytesRead = source.gcount();
        if (!destination.write(buffer, bytesRead)) {
            cerr << "❌ ERROR: Failed to write to destination" << endl;
            success = false;
            break;
        }
    }

    // Check for errors
    if (source.bad()) {
        cerr << "❌ ERROR: Source stream error" << endl;
        success = false;
    }
    if (destination.bad()) {
        cerr << "❌ ERROR: Destination stream error" << endl;
        success = false;
    }

    source.close();
    destination.close();

    if (success) {
        // Verify the copy silently
        ifstream destCheck(destinationPath, ios::binary | ios::ate);
        if (destCheck.is_open()) {
            streamsize destSize = destCheck.tellg();
            destCheck.close();

            if (destSize != sourceSize) {
                cerr << "❌ ERROR: File copy incomplete (size mismatch)" << endl;
                success = false;
            }
        }
        else {
            cerr << "❌ ERROR: Cannot verify destination file" << endl;
            success = false;
        }
    }

    return success;
}
bool FileManager::saveApplication(LoanApplication& application) {
    ofstream file(applicationsFile, ios::app);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << applicationsFile << " for writing" << endl;
        return false;
    }

    try {
        // Generate application ID if not set
        if (application.getApplicationId().empty()) {
            string appId = generateApplicationId();
            application.setApplicationId(appId);
        }

        string appId = application.getApplicationId();

        // Check for circular copy protection
        vector<string> sourcePaths = {
            application.getCnicFrontImagePath(),
            application.getCnicBackImagePath(),
            application.getElectricityBillImagePath(),
            application.getSalarySlipImagePath()
        };

        for (const auto& sourcePath : sourcePaths) {
            if (sourcePath.find("images/") != string::npos ||
                sourcePath.find("images\\") != string::npos ||
                sourcePath.find("COPY_FAILED") != string::npos) {
                cerr << "Error: Cannot use files from images folder as source" << endl;
                file.close();
                return false;
            }
        }

        // Set default status and date if not set
        if (application.getStatus().empty()) {
            application.setStatus("submitted");
        }

        if (application.getSubmissionDate().empty()) {
            application.setSubmissionDate("01-01-2024");
        }

        // Copy images
        string sourceCnicFront = application.getCnicFrontImagePath();
        string newCnicFrontPath = imagesDirectory + appId + "_cnic_front.jpg";
        if (copyImageFile(sourceCnicFront, newCnicFrontPath)) {
            application.setCnicFrontImagePath(newCnicFrontPath);
        }
        else {
            application.setCnicFrontImagePath("COPY_FAILED: " + sourceCnicFront);
        }

        string sourceCnicBack = application.getCnicBackImagePath();
        string newCnicBackPath = imagesDirectory + appId + "_cnic_back.jpg";
        if (copyImageFile(sourceCnicBack, newCnicBackPath)) {
            application.setCnicBackImagePath(newCnicBackPath);
        }
        else {
            application.setCnicBackImagePath("COPY_FAILED: " + sourceCnicBack);
        }

        string sourceElectricityBill = application.getElectricityBillImagePath();
        string newElectricityBillPath = imagesDirectory + appId + "_electricity_bill.jpg";
        if (copyImageFile(sourceElectricityBill, newElectricityBillPath)) {
            application.setElectricityBillImagePath(newElectricityBillPath);
        }
        else {
            application.setElectricityBillImagePath("COPY_FAILED: " + sourceElectricityBill);
        }

        string sourceSalarySlip = application.getSalarySlipImagePath();
        string newSalarySlipPath = imagesDirectory + appId + "_salary_slip.jpg";
        if (copyImageFile(sourceSalarySlip, newSalarySlipPath)) {
            application.setSalarySlipImagePath(newSalarySlipPath);
        }
        else {
            application.setSalarySlipImagePath("COPY_FAILED: " + sourceSalarySlip);
        }

        // Write application data to file
        file << application.getApplicationId() << Config::DELIMITER
            << application.getStatus() << Config::DELIMITER
            << application.getSubmissionDate() << Config::DELIMITER
            << application.getFullName() << Config::DELIMITER
            << application.getFathersName() << Config::DELIMITER
            << application.getPostalAddress() << Config::DELIMITER
            << application.getContactNumber() << Config::DELIMITER
            << application.getEmailAddress() << Config::DELIMITER
            << application.getCnicNumber() << Config::DELIMITER
            << application.getCnicExpiryDate() << Config::DELIMITER
            << application.getEmploymentStatus() << Config::DELIMITER
            << application.getMaritalStatus() << Config::DELIMITER
            << application.getGender() << Config::DELIMITER
            << application.getNumberOfDependents() << Config::DELIMITER
            << application.getAnnualIncome() << Config::DELIMITER
            << application.getAvgElectricityBill() << Config::DELIMITER
            << application.getCurrentElectricityBill() << Config::DELIMITER
            << application.getLoanType() << Config::DELIMITER
            << application.getLoanCategory() << Config::DELIMITER
            << application.getLoanAmount() << Config::DELIMITER
            << application.getDownPayment() << Config::DELIMITER
            << application.getInstallmentMonths() << Config::DELIMITER
            << application.getMonthlyPayment() << Config::DELIMITER
            << application.getInstallmentStartMonth() << Config::DELIMITER
            << application.getInstallmentStartYear() << Config::DELIMITER;

        // Existing loans
        vector<ExistingLoan> existingLoans = application.getExistingLoans();
        file << existingLoans.size() << Config::DELIMITER;
        for (size_t i = 0; i < existingLoans.size(); i++) {
            const auto& loan = existingLoans[i];
            file << loan.isActive << Config::DELIMITER
                << loan.totalAmount << Config::DELIMITER
                << loan.amountReturned << Config::DELIMITER
                << loan.amountDue << Config::DELIMITER
                << loan.bankName << Config::DELIMITER
                << loan.loanCategory << Config::DELIMITER;
        }

        // References
        Reference ref1 = application.getReference1();
        Reference ref2 = application.getReference2();

        file << ref1.name << Config::DELIMITER
            << ref1.cnic << Config::DELIMITER
            << ref1.cnicIssueDate << Config::DELIMITER
            << ref1.phoneNumber << Config::DELIMITER
            << ref1.email << Config::DELIMITER
            << ref2.name << Config::DELIMITER
            << ref2.cnic << Config::DELIMITER
            << ref2.cnicIssueDate << Config::DELIMITER
            << ref2.phoneNumber << Config::DELIMITER
            << ref2.email << Config::DELIMITER;

        // Image paths
        file << application.getCnicFrontImagePath() << Config::DELIMITER
            << application.getCnicBackImagePath() << Config::DELIMITER
            << application.getElectricityBillImagePath() << Config::DELIMITER
            << application.getSalarySlipImagePath() << endl;

        file.close();
        cout << "Application saved successfully with ID: " << application.getApplicationId() << endl;
        return true;

    }
    catch (const exception& e) {
        cerr << "Error saving application: " << e.what() << endl;
        file.close();
        return false;
    }
}
vector<LoanApplication> FileManager::loadAllApplications() const {
    vector<LoanApplication> applications;
    ifstream file(applicationsFile);

    if (!file.is_open()) {
        cerr << "Warning: Could not open applications file" << endl;
        return applications;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;

        try {
            vector<string> parts = splitString(line, Config::DELIMITER);

            // 🔥 FIX: Handle both old format (17 fields) and new format (23+ fields)
            if (parts.size() < 17) {
                cerr << "Warning: Line " << lineNumber << " has insufficient fields (" << parts.size() << ")" << endl;
                continue;
            }

            LoanApplication app;

            // Basic application data with validation
            if (!parts[0].empty()) app.setApplicationId(parts[0]);
            if (!parts[1].empty()) app.setStatus(parts[1]);
            if (!parts[2].empty()) app.setSubmissionDate(parts[2]);

            // Personal information
            if (!parts[3].empty()) app.setFullName(parts[3]);
            if (!parts[4].empty()) app.setFathersName(parts[4]);
            if (!parts[5].empty()) app.setPostalAddress(parts[5]);
            if (!parts[6].empty()) app.setContactNumber(parts[6]);
            if (!parts[7].empty()) app.setEmailAddress(parts[7]);
            if (!parts[8].empty()) app.setCnicNumber(parts[8]);
            if (!parts[9].empty()) app.setCnicExpiryDate(parts[9]);

            // Employment & Financial - WITH ERROR HANDLING
            if (!parts[10].empty()) app.setEmploymentStatus(parts[10]);
            if (!parts[11].empty()) app.setMaritalStatus(parts[11]);
            if (!parts[12].empty()) app.setGender(parts[12]);

            // Safe numeric parsing
            try {
                if (!parts[13].empty()) app.setNumberOfDependents(stoi(parts[13]));
            }
            catch (...) {
                cerr << "Warning: Invalid dependents at line " << lineNumber << ": " << parts[13] << endl;
            }

            try {
                if (!parts[14].empty()) app.setAnnualIncome(stoll(parts[14]));
            }
            catch (...) {
                cerr << "Warning: Invalid income at line " << lineNumber << ": " << parts[14] << endl;
            }

            try {
                if (!parts[15].empty()) app.setAvgElectricityBill(stoll(parts[15]));
            }
            catch (...) {
                cerr << "Warning: Invalid avg bill at line " << lineNumber << ": " << parts[15] << endl;
            }

            try {
                if (!parts[16].empty()) app.setCurrentElectricityBill(stoll(parts[16]));
            }
            catch (...) {
                cerr << "Warning: Invalid current bill at line " << lineNumber << ": " << parts[16] << endl;
            }

            // 🔥 FIX: Load loan-specific details if available
            if (parts.size() > 17 && !parts[17].empty()) {
                app.setLoanType(parts[17]);
            }
            else {
                app.setLoanType("Personal Loan"); // Default
            }

            if (parts.size() > 18 && !parts[18].empty()) {
                app.setLoanCategory(parts[18]);
            }
            else {
                app.setLoanCategory("Standard");
            }

            // Loan amount
            if (parts.size() > 19 && !parts[19].empty()) {
                try {
                    app.setLoanAmount(stoll(parts[19]));
                }
                catch (...) {
                    app.setLoanAmount(0);
                }
            }

            // Down payment
            if (parts.size() > 20 && !parts[20].empty()) {
                try {
                    app.setDownPayment(stoll(parts[20]));
                }
                catch (...) {
                    app.setDownPayment(0);
                }
            }

            // Installment months
            if (parts.size() > 21 && !parts[21].empty()) {
                try {
                    app.setInstallmentMonths(stoi(parts[21]));
                }
                catch (...) {
                    app.setInstallmentMonths(0);
                }
            }

            // Monthly payment
            if (parts.size() > 22 && !parts[22].empty()) {
                try {
                    app.setMonthlyPayment(stoll(parts[22]));
                }
                catch (...) {
                    app.setMonthlyPayment(0);
                }
            }

            applications.push_back(app);

        }
        catch (const exception& e) {
            cerr << "Error parsing application at line " << lineNumber << ": " << e.what() << endl;
            cerr << "Line content: " << line.substr(0, 100) << "..." << endl;
        }
    }

    file.close();
    return applications;
}
vector<LoanApplication> FileManager::findApplicationsByCNIC(const string& cnic) const {
    vector<LoanApplication> results;
    ifstream file(applicationsFile);

    if (!file.is_open()) {
        return results;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        try {
            vector<string> parts = splitString(line, Config::DELIMITER);
            if (parts.size() >= 25) {
                LoanApplication app;

                // Basic application data
                app.setApplicationId(parts[0]);
                app.setStatus(parts[1]);
                app.setSubmissionDate(parts[2]);

                // Personal information
                app.setFullName(parts[3]);
                app.setFathersName(parts[4]);
                app.setPostalAddress(parts[5]);
                app.setContactNumber(parts[6]);
                app.setEmailAddress(parts[7]);
                app.setCnicNumber(parts[8]);
                app.setCnicExpiryDate(parts[9]);

                // Employment & Financial
                app.setEmploymentStatus(parts[10]);
                app.setMaritalStatus(parts[11]);
                app.setGender(parts[12]);

                // Safe numeric parsing
                try { if (!parts[13].empty()) app.setNumberOfDependents(stoi(parts[13])); }
                catch (...) {}
                try { if (!parts[14].empty()) app.setAnnualIncome(stoll(parts[14])); }
                catch (...) {}
                try { if (!parts[15].empty()) app.setAvgElectricityBill(stoll(parts[15])); }
                catch (...) {}
                try { if (!parts[16].empty()) app.setCurrentElectricityBill(stoll(parts[16])); }
                catch (...) {}

                // Loan details
                if (parts.size() > 17) app.setLoanType(parts[17]);
                if (parts.size() > 18) app.setLoanCategory(parts[18]);

                try {
                    if (parts.size() > 19 && !parts[19].empty())
                        app.setLoanAmount(stoll(parts[19]));
                }
                catch (...) {}

                try {
                    if (parts.size() > 20 && !parts[20].empty())
                        app.setDownPayment(stoll(parts[20]));
                }
                catch (...) {}

                try {
                    if (parts.size() > 21 && !parts[21].empty())
                        app.setInstallmentMonths(stoi(parts[21]));
                }
                catch (...) {}

                try {
                    if (parts.size() > 22 && !parts[22].empty())
                        app.setMonthlyPayment(stoll(parts[22]));
                }
                catch (...) {}

                try {
                    if (parts.size() > 23 && !parts[23].empty())
                        app.setInstallmentStartMonth(stoi(parts[23]));
                }
                catch (...) {}

                try {
                    if (parts.size() > 24 && !parts[24].empty())
                        app.setInstallmentStartYear(stoi(parts[24]));
                }
                catch (...) {}

                if (app.getCnicNumber() == cnic) {
                    results.push_back(app);
                }
            }
        }
        catch (const exception& e) {
            cerr << "Error parsing application: " << e.what() << endl;
        }
    }

    file.close();
    return results;
}
void FileManager::getApplicationStatsByCNIC(const string& cnic, int& submitted, int& approved, int& rejected) const {
    submitted = approved = rejected = 0;

    auto applications = loadAllApplications();
    for (size_t i = 0; i < applications.size(); i++) {
        // Skip applications with empty CNIC or parsing errors
        if (applications[i].getCnicNumber().empty()) continue;

        if (applications[i].getCnicNumber() == cnic) {
            string status = applications[i].getStatus();
            if (status == "submitted") submitted++;
            else if (status == "approved") approved++;
            else if (status == "rejected") rejected++;
        }
    }
}

bool FileManager::updateApplicationStatus(const string& applicationId, const string& newStatus) {
    // Read all applications
    vector<string> lines;
    ifstream file(applicationsFile);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << applicationsFile << " for reading" << endl;
        return false;
    }

    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> parts = splitString(line, Config::DELIMITER);
        if (!parts.empty() && parts[0] == applicationId) {
            // Update the status (second field)
            parts[1] = newStatus;

            // Reconstruct the line
            string updatedLine;
            for (size_t i = 0; i < parts.size(); i++) {
                if (i > 0) updatedLine += Config::DELIMITER;
                updatedLine += parts[i];
            }
            lines.push_back(updatedLine);
            found = true;
        }
        else {
            lines.push_back(line);
        }
    }
    file.close();

    if (!found) {
        cerr << "Error: Application ID " << applicationId << " not found" << endl;
        return false;
    }

    // Write all lines back to file
    ofstream outFile(applicationsFile);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open " << applicationsFile << " for writing" << endl;
        return false;
    }

    for (const auto& l : lines) {
        outFile << l << endl;
    }
    outFile.close();

    cout << "Application " << applicationId << " status updated to: " << newStatus << endl;
    return true;
}

LoanApplication FileManager::findApplicationById(const string& applicationId) const {
    ifstream file(applicationsFile);

    if (!file.is_open()) {
        cerr << "Error: Could not open applications file for reading" << endl;
        return LoanApplication();
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> parts = splitString(line, Config::DELIMITER);
        if (!parts.empty() && parts[0] == applicationId) {
            LoanApplication app;
            try {
                // Basic application data
                app.setApplicationId(parts[0]);
                app.setStatus(parts[1]);
                app.setSubmissionDate(parts[2]);

                // Personal information
                app.setFullName(parts[3]);
                app.setFathersName(parts[4]);
                app.setPostalAddress(parts[5]);
                app.setContactNumber(parts[6]);
                app.setEmailAddress(parts[7]);
                app.setCnicNumber(parts[8]);
                app.setCnicExpiryDate(parts[9]);

                // Employment & Financial
                app.setEmploymentStatus(parts[10]);
                app.setMaritalStatus(parts[11]);
                app.setGender(parts[12]);

                // Safe numeric parsing for basic fields
                try {
                    if (!parts[13].empty()) app.setNumberOfDependents(stoi(parts[13]));
                }
                catch (...) {
                    cerr << "Warning: Invalid dependents for application " << applicationId << endl;
                }

                try {
                    if (!parts[14].empty()) app.setAnnualIncome(stoll(parts[14]));
                }
                catch (...) {
                    cerr << "Warning: Invalid annual income for application " << applicationId << endl;
                }

                try {
                    if (!parts[15].empty()) app.setAvgElectricityBill(stoll(parts[15]));
                }
                catch (...) {
                    cerr << "Warning: Invalid avg electricity bill for application " << applicationId << endl;
                }

                try {
                    if (!parts[16].empty()) app.setCurrentElectricityBill(stoll(parts[16]));
                }
                catch (...) {
                    cerr << "Warning: Invalid current electricity bill for application " << applicationId << endl;
                }

                // 🔥 CRITICAL FIX: Load loan-specific details
                if (parts.size() > 17 && !parts[17].empty()) {
                    app.setLoanType(parts[17]);
                }
                else {
                    app.setLoanType("Unknown");
                }

                if (parts.size() > 18 && !parts[18].empty()) {
                    app.setLoanCategory(parts[18]);
                }
                else {
                    app.setLoanCategory("Unknown");
                }

                // Loan amount
                if (parts.size() > 19 && !parts[19].empty()) {
                    try {
                        app.setLoanAmount(stoll(parts[19]));
                    }
                    catch (...) {
                        cerr << "Warning: Invalid loan amount for application " << applicationId << endl;
                        app.setLoanAmount(0);
                    }
                }
                else {
                    app.setLoanAmount(0);
                }

                // Down payment
                if (parts.size() > 20 && !parts[20].empty()) {
                    try {
                        app.setDownPayment(stoll(parts[20]));
                    }
                    catch (...) {
                        cerr << "Warning: Invalid down payment for application " << applicationId << endl;
                        app.setDownPayment(0);
                    }
                }
                else {
                    app.setDownPayment(0);
                }

                // Installment months
                if (parts.size() > 21 && !parts[21].empty()) {
                    try {
                        app.setInstallmentMonths(stoi(parts[21]));
                    }
                    catch (...) {
                        cerr << "Warning: Invalid installment months for application " << applicationId << endl;
                        app.setInstallmentMonths(0);
                    }
                }
                else {
                    app.setInstallmentMonths(0);
                }

                // Monthly payment
                if (parts.size() > 22 && !parts[22].empty()) {
                    try {
                        app.setMonthlyPayment(stoll(parts[22]));
                    }
                    catch (...) {
                        cerr << "Warning: Invalid monthly payment for application " << applicationId << endl;
                        app.setMonthlyPayment(0);
                    }
                }
                else {
                    // Calculate monthly payment if not stored
                    if (app.getInstallmentMonths() > 0 && app.getLoanAmount() > 0 && app.getDownPayment() > 0) {
                        long long monthly = (app.getLoanAmount() - app.getDownPayment()) / app.getInstallmentMonths();
                        app.setMonthlyPayment(monthly);
                    }
                    else {
                        app.setMonthlyPayment(0);
                    }
                }

                file.close();
                return app;

            }
            catch (const exception& e) {
                cerr << "Error parsing application " << applicationId << ": " << e.what() << endl;
                file.close();
                return LoanApplication();
            }
        }
    }

    file.close();
    cerr << "Application " << applicationId << " not found in file" << endl;
    return LoanApplication();
}

vector<LoanApplication> FileManager::loadAllApplicationsDetailed() const {
    vector<LoanApplication> applications;
    ifstream file(applicationsFile);

    if (!file.is_open()) {
        return applications;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> parts = splitString(line, Config::DELIMITER);
        if (parts.size() >= 17) { // At least basic fields
            LoanApplication app;
            try {
                app.setApplicationId(parts[0]);
                app.setStatus(parts[1]);
                app.setSubmissionDate(parts[2]);
                app.setFullName(parts[3]);
                app.setFathersName(parts[4]);
                app.setPostalAddress(parts[5]);
                app.setContactNumber(parts[6]);
                app.setEmailAddress(parts[7]);
                app.setCnicNumber(parts[8]);
                app.setCnicExpiryDate(parts[9]);
                app.setEmploymentStatus(parts[10]);
                app.setMaritalStatus(parts[11]);
                app.setGender(parts[12]);
                app.setNumberOfDependents(stoi(parts[13]));
                app.setAnnualIncome(stoll(parts[14]));
                app.setAvgElectricityBill(stoll(parts[15]));
                app.setCurrentElectricityBill(stoll(parts[16]));

                applications.push_back(app);
            }
            catch (const exception& e) {
                cerr << "Error parsing application: " << e.what() << endl;
            }
        }
    }

    file.close();
    return applications;
}