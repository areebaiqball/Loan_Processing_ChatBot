@echo off
echo Starting User Client...
cd /d "%~dp0"

echo Checking for user_client.exe...
if exist "x64\UserClient\user_client.exe" (
    echo Found: x64\UserClient\user_client.exe
    start "User Client" "x64\UserClient\user_client.exe"
) else (
    echo ERROR: user_client.exe not found in x64\UserClient\
    echo.
    echo Available files in x64\UserClient:
    if exist "x64\UserClient\" dir /b "x64\UserClient\"
    echo.
    echo Please build the UserClient configuration in Visual Studio.
    pause
)