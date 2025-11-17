#include "file_manager.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

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

bool FileManager::copyImageFile(const string& sourcePath, const string& destinationPath) const {
    // Basic check if source file exists
    ifstream source(sourcePath, ios::binary);
    if (!source.is_open()) {
        cerr << "Error: Source file not found: " << sourcePath << endl;
        return false;
    }

    // Open destination file
    ofstream destination(destinationPath, ios::binary);
    if (!destination.is_open()) {
        cerr << "Error: Cannot create destination file: " << destinationPath << endl;
        source.close();
        return false;
    }

    // Copy file content
    destination << source.rdbuf();

    // Check if copy was successful
    bool success = !source.fail() && !destination.fail();

    source.close();
    destination.close();

    if (!success) {
        cerr << "Error: File copy failed from " << sourcePath << " to " << destinationPath << endl;
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

        // Set default status and date if not set
        if (application.getStatus().empty()) {
            application.setStatus("submitted");
        }

        if (application.getSubmissionDate().empty()) {
            application.setSubmissionDate("01-01-2024"); // Default date
        }

        // Copy images and get new paths
        string appId = application.getApplicationId();

        cout << endl << "Copying document images..." << endl;

        // Copy CNIC Front
        string sourceCnicFront = application.getCnicFrontImagePath();
        string newCnicFrontPath = imagesDirectory + appId + "_cnic_front.jpg";
        if (copyImageFile(sourceCnicFront, newCnicFrontPath)) {
            application.setCnicFrontImagePath(newCnicFrontPath);
            cout << "CNIC front image copied to: " << newCnicFrontPath << endl;
        }
        else {
            cerr << "Failed to copy CNIC front image" << endl;
            application.setCnicFrontImagePath("COPY_FAILED: " + sourceCnicFront);
        }

        // Copy CNIC Back
        string sourceCnicBack = application.getCnicBackImagePath();
        string newCnicBackPath = imagesDirectory + appId + "_cnic_back.jpg";
        if (copyImageFile(sourceCnicBack, newCnicBackPath)) {
            application.setCnicBackImagePath(newCnicBackPath);
            cout << "CNIC back image copied to: " << newCnicBackPath << endl;
        }
        else {
            cerr << "Failed to copy CNIC back image" << endl;
            application.setCnicBackImagePath("COPY_FAILED: " + sourceCnicBack);
        }

        // Copy Electricity Bill
        string sourceElectricityBill = application.getElectricityBillImagePath();
        string newElectricityBillPath = imagesDirectory + appId + "_electricity_bill.jpg";
        if (copyImageFile(sourceElectricityBill, newElectricityBillPath)) {
            application.setElectricityBillImagePath(newElectricityBillPath);
            cout << "Electricity bill image copied to: " << newElectricityBillPath << endl;
        }
        else {
            cerr << "Failed to copy electricity bill image" << endl;
            application.setElectricityBillImagePath("COPY_FAILED: " + sourceElectricityBill);
        }

        // Copy Salary Slip
        string sourceSalarySlip = application.getSalarySlipImagePath();
        string newSalarySlipPath = imagesDirectory + appId + "_salary_slip.jpg";
        if (copyImageFile(sourceSalarySlip, newSalarySlipPath)) {
            application.setSalarySlipImagePath(newSalarySlipPath);
            cout << "Salary slip image copied to: " << newSalarySlipPath << endl;
        }
        else {
            cerr << "Failed to copy salary slip image" << endl;
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
            << application.getCurrentElectricityBill() << Config::DELIMITER;

        // Existing loans
        auto existingLoans = application.getExistingLoans();
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

        // Image paths (the new copied paths)
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

// Rest of the methods remain the same...
vector<LoanApplication> FileManager::loadAllApplications() const {
    vector<LoanApplication> applications;

    ifstream file(applicationsFile);
    if (!file.is_open()) {
        return applications;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        try {
            vector<string> parts = splitString(line, Config::DELIMITER);
            if (parts.size() >= 25) {
                LoanApplication app;

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
        }
        catch (const exception& e) {
            cerr << "Error parsing application: " << e.what() << endl;
        }
    }

    file.close();
    return applications;
}

vector<LoanApplication> FileManager::findApplicationsByCNIC(const string& cnic) const {
    vector<LoanApplication> results;
    auto allApplications = loadAllApplications();

    for (size_t i = 0; i < allApplications.size(); i++) {
        if (allApplications[i].getCnicNumber() == cnic) {
            results.push_back(allApplications[i]);
        }
    }

    return results;
}

void FileManager::getApplicationStatsByCNIC(const string& cnic, int& submitted, int& approved, int& rejected) const {
    submitted = approved = rejected = 0;
 
    auto applications = findApplicationsByCNIC(cnic);
    for (size_t i = 0; i < applications.size(); i++) {
        string status = applications[i].getStatus();
        if (status == "submitted") submitted++;
        else if (status == "approved") approved++;
        else if (status == "rejected") rejected++;
    }
}