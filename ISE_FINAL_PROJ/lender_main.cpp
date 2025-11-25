#include <iostream>
#include <string>
#include <thread>
#include "network_utils.h"
#include "file_manager.h"
#include "application.h"
#include "utilities.h"
#include "ui_handler.h"

using namespace std;

// Forward declarations from lender_server.cpp
void handleLenderCommands(FileManager& fileManager);

int main() {
    FileManager fileManager;

    cout << "╔════════════════════════════════════╗" << endl;
    cout << "║   LOAN PROCESSING SYSTEM           ║" << endl;
    cout << "║   LENDER SERVER                    ║" << endl;
    cout << "╚════════════════════════════════════╝" << endl;

    handleLenderCommands(fileManager);

    return 0;
}