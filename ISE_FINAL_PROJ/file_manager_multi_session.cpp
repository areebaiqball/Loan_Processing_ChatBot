//#include "file_manager.h"
//#include "utilities.h"
//#include <fstream>
//#include <iostream>
//#include <sstream>
//#include <iomanip>
//#include <vector>
//
//using namespace std;
//
//bool FileManager::updateApplicationSection(const LoanApplication& application, const string& section) {
//    vector<string> lines;
//    ifstream file(applicationsFile);
//
//    if (!file.is_open()) {
//        cerr << "Error: Could not open applications file" << endl;
//        return false;
//    }
//
//    string line;
//    bool found = false;
//
//    while (getline(file, line)) {
//        if (line.empty()) continue;
//
//        vector<string> parts = splitString(line, Config::DELIMITER);
//        if (!parts.empty() && parts[0] == application.getApplicationId()) {
//            LoanApplication updatedApp = application;
//            updatedApp.markSectionCompleted(section);
//            string updatedLine = applicationToFileFormat(updatedApp);
//            lines.push_back(updatedLine);
//            found = true;
//        }
//        else {
//            lines.push_back(line);
//        }
//    }
//    file.close();
//
//    if (!found) {
//        LoanApplication newApp = application;
//        newApp.markSectionCompleted(section);
//        lines.push_back(applicationToFileFormat(newApp));
//        found = true;
//    }
//
//    ofstream outFile(applicationsFile);
//    if (!outFile.is_open()) {
//        cerr << "Error: Could not open applications file for writing" << endl;
//        return false;
//    }
//
//    for (size_t i = 0; i < lines.size(); i++) {
//        outFile << lines[i] << endl;
//    }
//    outFile.close();
//
//    cout << "✓ Section '" << section << "' saved successfully for application "
//        << application.getApplicationId() << endl;
//    return true;
//}
//
//LoanApplication FileManager::findIncompleteApplication(const string& applicationId, const string& cnic) const {
//    vector<LoanApplication> allApplications = loadAllApplications();
//
//    for (size_t i = 0; i < allApplications.size(); i++) {
//        const LoanApplication& app = allApplications[i];
//        if (app.getApplicationId() == applicationId &&
//            app.getCnicNumber() == cnic &&
//            !app.isApplicationComplete()) {
//            return app;
//        }
//    }
//
//    return LoanApplication();
//}
//
//vector<LoanApplication> FileManager::findUserIncompleteApplications(const string& cnic) const {
//    vector<LoanApplication> allApplications = loadAllApplications();
//    vector<LoanApplication> incompleteApps;
//
//    for (size_t i = 0; i < allApplications.size(); i++) {
//        const LoanApplication& app = allApplications[i];
//        if (app.getCnicNumber() == cnic && !app.isApplicationComplete()) {
//            incompleteApps.push_back(app);
//        }
//    }
//
//    return incompleteApps;
//}
//
//bool FileManager::canUpdateApplication(const string& applicationId, const string& cnic) const {
//    LoanApplication app = findIncompleteApplication(applicationId, cnic);
//    return !app.getApplicationId().empty() && !app.isApplicationComplete();
//}