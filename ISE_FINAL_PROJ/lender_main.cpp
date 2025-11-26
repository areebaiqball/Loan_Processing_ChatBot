#include <iostream>
#include <string>
#include "file_manager.h"
#include "application.h"
#include "utilities.h"
#include "ui_handler.h"

using namespace std;

// Forward declarations from lender_server.cpp
void handleLenderCommands(FileManager& fileManager);

int main() {
    FileManager fileManager;

    cout << endl<<"LOAN PROCESSING SYSTEM           " << endl;
    cout << endl<<"LENDER SERVER                    " << endl<<endl;

    handleLenderCommands(fileManager);

    return 0;
}