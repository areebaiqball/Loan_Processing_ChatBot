@echo off
echo ================================================
echo   Loan Processing System Launcher
echo ================================================
echo.

cd /d "%~dp0"

echo Checking for executables...
echo.

set LENDER_EXE=
set USER_EXE=

if exist "x64\LenderServer\lender_server.exe" (
    echo Found Lender: x64\LenderServer\lender_server.exe
    set LENDER_EXE=x64\LenderServer\lender_server.exe
) else (
    echo ERROR: lender_server.exe not found in x64\LenderServer\
)

if exist "x64\UserClient\user_client.exe" (
    echo Found User: x64\UserClient\user_client.exe
    set USER_EXE=x64\UserClient\user_client.exe
) else (
    echo ERROR: user_client.exe not found in x64\UserClient\
)

echo.
if "%LENDER_EXE%"=="" (
    echo Cannot start Lender Server - executable not found.
) else (
    echo Starting Lender Server...
    start "Lender Server" "%LENDER_EXE%"
)

if "%USER_EXE%"=="" (
    echo Cannot start User Client - executable not found.
) else (
    timeout /t 2 /nobreak >nul
    echo Starting User Client...
    start "User Client" "%USER_EXE%"
)

echo.
echo ================================================
if not "%LENDER_EXE%"=="" if not "%USER_EXE%"=="" (
    echo Both applications launched successfully!
) else (
    echo Some applications could not be launched.
    echo Check the build output above.
)
echo ================================================
echo.
pause