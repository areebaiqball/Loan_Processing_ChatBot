@echo off
echo Starting Lender Server...
cd /d "%~dp0"

echo Checking for lender_server.exe...
if exist "x64\LenderServer\lender_server.exe" (
    echo Found: x64\LenderServer\lender_server.exe
    start "Lender Server" "x64\LenderServer\lender_server.exe"
) else (
    echo ERROR: lender_server.exe not found in x64\LenderServer\
    echo.
    echo Available files in x64\LenderServer:
    if exist "x64\LenderServer\" dir /b "x64\LenderServer\"
    echo.
    echo Please build the LenderServer configuration in Visual Studio.
    pause
)