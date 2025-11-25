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
}

string FileManager::generateApplicationId() const {
    static int counter = 1001;

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

    stringstream ss;
    ss << setw(4) << setfill('0') << (maxId + 1);
    return ss.str();
}

bool FileManager::fileExists(const string& filename) const {
    ifstream file(filename);
    return file.good();
}

bool FileManager::copyImageFile(const string& sourcePath, const string& destinationPath) const {
    // Validate source file exists and is not empty
    ifstream sourceCheck(sourcePath, ios::binary | ios::ate);
    if (!sourceCheck.is_open()) {
        cerr << "ERROR: Cannot open source file: " << sourcePath << endl;
        return false;
    }

    streamsize sourceSize = sourceCheck.tellg();
    sourceCheck.close();

    if (sourceSize == 0) {
        cerr << "ERROR: Source file is empty: " << sourcePath << endl;
        return false;
    }

    // Prevent circular copying (source == destination)
    if (sourcePath == destinationPath) {
        cerr << "ERROR: Source and destination are the same: " << sourcePath << endl;
        return false;
    }

    // Open source file for reading
    ifstream source(sourcePath, ios::binary);
    if (!source.is_open()) {
        cerr << "ERROR: Failed to open source for reading: " << sourcePath << endl;
        return false;
    }

    // Open destination file for writing
    ofstream destination(destinationPath, ios::binary | ios::trunc);
    if (!destination.is_open()) {
        cerr << "ERROR: Failed to create destination: " << destinationPath << endl;
        source.close();
        return false;
    }

    // Copy file in chunks
    const size_t bufferSize = 8192;
    char buffer[bufferSize];
    streamsize totalCopied = 0;

    while (source.read(buffer, bufferSize) || source.gcount() > 0) {
        streamsize bytesRead = source.gcount();
        destination.write(buffer, bytesRead);

        if (destination.fail()) {
            cerr << "ERROR: Write failed at byte " << totalCopied << endl;
            source.close();
            destination.close();
            return false;
        }

        totalCopied += bytesRead;
    }

    // Close files
    source.close();
    destination.close();

    // Verify the copy
    ifstream verifyDest(destinationPath, ios::binary | ios::ate);
    if (!verifyDest.is_open()) {
        cerr << "ERROR: Cannot verify destination file" << endl;
        return false;
    }

    streamsize destSize = verifyDest.tellg();
    verifyDest.close();

    if (destSize != sourceSize) {
        cerr << "ERROR: File size mismatch. Source: " << sourceSize
            << " bytes, Destination: " << destSize << " bytes" << endl;
        return false;
    }

    cout << "✓ Image copied successfully: " << destinationPath
        << " (" << destSize << " bytes)" << endl;
    return true;
}

bool FileManager::saveApplication(LoanApplication& application) {
    cout << "DEBUG: saveApplication called for: " << application.getFullName()
        << " | Loan Type: " << application.getLoanType() << endl;
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

        // Set default status and date if not already set
        if (application.getStatus().empty()) {
            application.setStatus("submitted");
        }

        if (application.getSubmissionDate().empty()) {
            string currentDate = getCurrentDate();
            application.setSubmissionDate(currentDate);
        }

        // Get source paths
        string sourceCnicFront = application.getCnicFrontImagePath();
        string sourceCnicBack = application.getCnicBackImagePath();
        string sourceElectricityBill = application.getElectricityBillImagePath();
        string sourceSalarySlip = application.getSalarySlipImagePath();

        // Check if images need copying (not already in images directory)
        bool cnicFrontNeedsCopy = (sourceCnicFront.find(imagesDirectory) == string::npos);
        bool cnicBackNeedsCopy = (sourceCnicBack.find(imagesDirectory) == string::npos);
        bool electricityBillNeedsCopy = (sourceElectricityBill.find(imagesDirectory) == string::npos);
        bool salarySlipNeedsCopy = (sourceSalarySlip.find(imagesDirectory) == string::npos);

        // Copy CNIC Front
        if (cnicFrontNeedsCopy) {
            string newCnicFrontPath = imagesDirectory + appId + "_cnic_front.jpg";
            if (copyImageFile(sourceCnicFront, newCnicFrontPath)) {
                application.setCnicFrontImagePath(newCnicFrontPath);
            }
            else {
                application.setCnicFrontImagePath("COPY_FAILED: " + sourceCnicFront);
            }
        }

        // Copy CNIC Back
        if (cnicBackNeedsCopy) {
            string newCnicBackPath = imagesDirectory + appId + "_cnic_back.jpg";
            if (copyImageFile(sourceCnicBack, newCnicBackPath)) {
                application.setCnicBackImagePath(newCnicBackPath);
            }
            else {
                application.setCnicBackImagePath("COPY_FAILED: " + sourceCnicBack);
            }
        }

        // Copy Electricity Bill
        if (electricityBillNeedsCopy) {
            string newElectricityBillPath = imagesDirectory + appId + "_electricity_bill.jpg";
            if (copyImageFile(sourceElectricityBill, newElectricityBillPath)) {
                application.setElectricityBillImagePath(newElectricityBillPath);
            }
            else {
                application.setElectricityBillImagePath("COPY_FAILED: " + sourceElectricityBill);
            }
        }

        // Copy Salary Slip
        if (salarySlipNeedsCopy) {
            string newSalarySlipPath = imagesDirectory + appId + "_salary_slip.jpg";
            if (copyImageFile(sourceSalarySlip, newSalarySlipPath)) {
                application.setSalarySlipImagePath(newSalarySlipPath);
            }
            else {
                application.setSalarySlipImagePath("COPY_FAILED: " + sourceSalarySlip);
            }
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
            << application.getInstallmentStartYear() << Config::DELIMITER
            << application.getRejectionReason() << Config::DELIMITER;

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
        cout << "✓ Application saved successfully with ID: " << application.getApplicationId() << endl;
        cout << "Submission Date: " << application.getSubmissionDate() << endl;
        return true;

    }
    catch (const exception& e) {
        cerr << "Error saving application: " << e.what() << endl;
        file.close();
        return false;
    }
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

    vector<LoanApplication> applications = loadAllApplications();
    for (size_t i = 0; i < applications.size(); i++) {
        if (applications[i].getCnicNumber().empty()) continue;

        if (applications[i].getCnicNumber() == cnic) {
            string status = applications[i].getStatus();
            if (status == "submitted") submitted++;
            else if (status == "approved") approved++;
            else if (status == "rejected") rejected++;
        }
    }
}

bool FileManager::updateApplicationStatus(const string& applicationId, const string& newStatus, const string& rejectionReason) {
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
            parts[1] = newStatus;

            if (!rejectionReason.empty() && parts.size() > 25) {
                parts[25] = rejectionReason;
            }

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
    if (!rejectionReason.empty()) {
        cout << "Rejection reason: " << rejectionReason << endl;
    }
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
        if (parts.size() >= 17) {
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

string FileManager::applicationToFileFormat(const LoanApplication& application) const {
    stringstream ss;

    // Basic application data
    ss << application.getApplicationId() << Config::DELIMITER
        << application.getStatus() << Config::DELIMITER
        << application.getSubmissionDate() << Config::DELIMITER
        << application.getCompletedSections() << Config::DELIMITER
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
        << application.getInstallmentStartYear() << Config::DELIMITER
        << application.getRejectionReason() << Config::DELIMITER;

    // Existing loans
    vector<ExistingLoan> existingLoans = application.getExistingLoans();
    ss << existingLoans.size() << Config::DELIMITER;
    for (size_t i = 0; i < existingLoans.size(); i++) {
        const auto& loan = existingLoans[i];
        ss << loan.isActive << Config::DELIMITER
            << loan.totalAmount << Config::DELIMITER
            << loan.amountReturned << Config::DELIMITER
            << loan.amountDue << Config::DELIMITER
            << loan.bankName << Config::DELIMITER
            << loan.loanCategory << Config::DELIMITER;
    }

    // References
    Reference ref1 = application.getReference1();
    Reference ref2 = application.getReference2();

    ss << ref1.name << Config::DELIMITER
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
    ss << application.getCnicFrontImagePath() << Config::DELIMITER
        << application.getCnicBackImagePath() << Config::DELIMITER
        << application.getElectricityBillImagePath() << Config::DELIMITER
        << application.getSalarySlipImagePath();

    return ss.str();
}

LoanApplication FileManager::applicationFromFileFormat(const vector<string>& parts) const {
    LoanApplication app;

    if (parts.size() < 28) return app; // Minimum required fields

    try {
        // Basic application data
        app.setApplicationId(parts[0]);
        app.setStatus(parts[1]);
        app.setSubmissionDate(parts[2]);
        app.setCompletedSections(parts[3]);

        // Personal information
        app.setFullName(parts[4]);
        app.setFathersName(parts[5]);
        app.setPostalAddress(parts[6]);
        app.setContactNumber(parts[7]);
        app.setEmailAddress(parts[8]);
        app.setCnicNumber(parts[9]);
        app.setCnicExpiryDate(parts[10]);
        app.setEmploymentStatus(parts[11]);
        app.setMaritalStatus(parts[12]);
        app.setGender(parts[13]);

        if (!parts[14].empty()) app.setNumberOfDependents(stoi(parts[14]));
        if (!parts[15].empty()) app.setAnnualIncome(stoll(parts[15]));
        if (!parts[16].empty()) app.setAvgElectricityBill(stoll(parts[16]));
        if (!parts[17].empty()) app.setCurrentElectricityBill(stoll(parts[17]));

        // Loan details
        if (parts.size() > 18) app.setLoanType(parts[18]);
        if (parts.size() > 19) app.setLoanCategory(parts[19]);
        if (parts.size() > 20 && !parts[20].empty()) app.setLoanAmount(stoll(parts[20]));
        if (parts.size() > 21 && !parts[21].empty()) app.setDownPayment(stoll(parts[21]));
        if (parts.size() > 22 && !parts[22].empty()) app.setInstallmentMonths(stoi(parts[22]));
        if (parts.size() > 23 && !parts[23].empty()) app.setMonthlyPayment(stoll(parts[23]));

        // Rejection reason
        if (parts.size() > 26 && !parts[26].empty()) app.setRejectionReason(parts[26]);

        // Existing loans
        if (parts.size() > 27) {
            int loanCount = 0;
            try { loanCount = stoi(parts[27]); }
            catch (...) {}

            int index = 28;
            for (int i = 0; i < loanCount && index + 5 < parts.size(); i++) {
                ExistingLoan loan;
                loan.isActive = (parts[index] == "1" || toLower(parts[index]) == "true");
                loan.totalAmount = stoll(parts[index + 1]);
                loan.amountReturned = stoll(parts[index + 2]);
                loan.amountDue = stoll(parts[index + 3]);
                loan.bankName = parts[index + 4];
                loan.loanCategory = parts[index + 5];
                app.addExistingLoan(loan);
                index += 6;
            }

            // References
            if (index + 9 < parts.size()) {
                Reference ref1;
                ref1.name = parts[index];
                ref1.cnic = parts[index + 1];
                ref1.cnicIssueDate = parts[index + 2];
                ref1.phoneNumber = parts[index + 3];
                ref1.email = parts[index + 4];
                app.setReference1(ref1);

                Reference ref2;
                ref2.name = parts[index + 5];
                ref2.cnic = parts[index + 6];
                ref2.cnicIssueDate = parts[index + 7];
                ref2.phoneNumber = parts[index + 8];
                ref2.email = parts[index + 9];
                app.setReference2(ref2);
                index += 10;
            }

            // Image paths
            if (index + 3 < parts.size()) {
                app.setCnicFrontImagePath(parts[index]);
                app.setCnicBackImagePath(parts[index + 1]);
                app.setElectricityBillImagePath(parts[index + 2]);
                app.setSalarySlipImagePath(parts[index + 3]);
            }
        }

    }
    catch (const exception& e) {
        cerr << "Error parsing application: " << e.what() << endl;
    }

    return app;
}

bool FileManager::updateApplicationSection(const LoanApplication& application, const string& section) {
    vector<string> lines;
    ifstream file(applicationsFile);

    if (!file.is_open()) {
        cerr << "Error: Could not open applications file" << endl;
        return false;
    }

    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> parts = splitString(line, Config::DELIMITER);
        if (!parts.empty() && parts[0] == application.getApplicationId()) {
            // Replace with updated application
            LoanApplication updatedApp = application;
            updatedApp.markSectionCompleted(section);
            string updatedLine = applicationToFileFormat(updatedApp);
            lines.push_back(updatedLine);
            found = true;
        }
        else {
            lines.push_back(line);
        }
    }
    file.close();

    if (!found) {
        // Application doesn't exist yet, add it
        LoanApplication newApp = application;
        newApp.markSectionCompleted(section);
        lines.push_back(applicationToFileFormat(newApp));
        found = true;
    }

    // Write back to file
    ofstream outFile(applicationsFile);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open applications file for writing" << endl;
        return false;
    }

    for (const auto& l : lines) {
        outFile << l << endl;
    }
    outFile.close();

    cout << "✓ Section '" << section << "' saved successfully for application "
        << application.getApplicationId() << endl;
    return true;
}

// Multi-session support methods - SINGLE IMPLEMENTATION ONLY
LoanApplication FileManager::findIncompleteApplication(const string& applicationId, const string& cnic) const {
    vector<LoanApplication> allApplications = loadAllApplications();

    for (size_t i = 0; i < allApplications.size(); i++) {
        const LoanApplication& app = allApplications[i];
        if (app.getApplicationId() == applicationId &&
            app.getCnicNumber() == cnic &&
            !app.isApplicationComplete()) {
            return app;
        }
    }

    return LoanApplication(); // Return empty application if not found
}

vector<LoanApplication> FileManager::findUserIncompleteApplications(const string& cnic) const {
    vector<LoanApplication> allApplications = loadAllApplications();
    vector<LoanApplication> incompleteApps;

    for (size_t i = 0; i < allApplications.size(); i++) {
        const LoanApplication& app = allApplications[i];
        if (app.getCnicNumber() == cnic && !app.isApplicationComplete()) {
            incompleteApps.push_back(app);
        }
    }

    return incompleteApps;
}

bool FileManager::canUpdateApplication(const string& applicationId, const string& cnic) const {
    LoanApplication app = findIncompleteApplication(applicationId, cnic);
    return !app.getApplicationId().empty() && !app.isApplicationComplete();
}

// Update loadAllApplications to use new format
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

            if (parts.size() < 4) { // At least ID, status, date, completed sections
                cerr << "Warning: Line " << lineNumber << " has insufficient fields (" << parts.size() << ")" << endl;
                continue;
            }

            LoanApplication app = applicationFromFileFormat(parts);
            if (!app.getApplicationId().empty()) {
                applications.push_back(app);
            }

        }
        catch (const exception& e) {
            cerr << "Error parsing application at line " << lineNumber << ": " << e.what() << endl;
        }
    }

    file.close();
    return applications;
}