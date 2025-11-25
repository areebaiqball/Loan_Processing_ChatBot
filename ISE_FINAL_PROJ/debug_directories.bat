@echo off
echo ================================================
echo   Debug Build Directories
echo ================================================
echo.

echo Checking x64\LenderServer:
if exist "x64\LenderServer\" (
    dir "x64\LenderServer\"
) else (
    echo x64\LenderServer does not exist
)
echo.

echo Checking x64\UserClient:
if exist "x64\UserClient\" (
    dir "x64\UserClient\"
) else (
    echo x64\UserClient does not exist
)
echo.

echo Checking x64\Debug:
if exist "x64\Debug\" (
    dir "x64\Debug\"
) else (
    echo x64\Debug does not exist
)
echo.

echo Any intermediate files?
dir /s *.obj 2>nul
if errorlevel 1 echo No .obj files found
echo.

pause