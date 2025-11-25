@echo off
echo Starting Loan Processing System...
start "Lender Server" lender_server.exe
timeout /t 2 /nobreak > nul
start "User Client" user_client.exe
echo Both interfaces started!