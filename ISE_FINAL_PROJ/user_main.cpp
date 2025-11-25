#include <iostream>
#include <string>
#include <iomanip>
#include "utilities.h"
#include "home.h"
#include "car.h"
#include "scooter.h"
#include "application.h"
#include "application_collector.h"
#include "file_manager.h"
#include "utterances.h"
#include "ui_handler.h"

using namespace std;

// Forward declarations from user_client.cpp
void handleUserMode(const HomeLoan[], int, const CarLoan[], int,
    const ScooterLoan[], int, const Utterance[], int,
    ApplicationCollector&, FileManager&);

int main() {
    // Initialize data
    Utterance utterances[Config::MAX_UTTERANCES];
    HomeLoan homeLoans[Config::MAX_LOANS];
    CarLoan carLoans[Config::MAX_LOANS];
    ScooterLoan scooterLoans[Config::MAX_LOANS];
    FileManager fileManager;
    ApplicationCollector collector;

    int utteranceCount = loadUtterances(utterances, Config::MAX_UTTERANCES, Config::UTTERANCES_FILE);
    int homeLoanCount = loadHomeLoans(homeLoans, Config::MAX_LOANS, Config::HOME_LOANS_FILE);
    int carLoanCount = loadCarLoans(carLoans, Config::MAX_LOANS, Config::CAR_LOANS_FILE);
    int scooterLoanCount = loadScooterLoans(scooterLoans, Config::MAX_LOANS, Config::SCOOTER_LOANS_FILE);

    cout << "╔════════════════════════════════════╗" << endl;
    cout << "║   LOAN PROCESSING SYSTEM           ║" << endl;
    cout << "║   USER CLIENT                      ║" << endl;
    cout << "╚════════════════════════════════════╝" << endl;

    handleUserMode(homeLoans, homeLoanCount, carLoans, carLoanCount,
        scooterLoans, scooterLoanCount, utterances, utteranceCount,
        collector, fileManager);

    return 0;
}