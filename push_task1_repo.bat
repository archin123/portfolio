@echo off
echo =====================================================================
echo  Push ESP32-Web-Server-LED-Control to https://github.com/archin123/ESP32-Web-Server-LED-Control
echo =====================================================================
echo.
echo Please make sure you have created an empty repository on GitHub:
echo   Repository Name: ESP32-Web-Server-LED-Control
echo   Owner: archin123
echo   (Do NOT check "Add a README file" or "Add .gitignore")
echo.
pause

set TARGET_DIR=%TEMP%\ESP32-Web-Server-LED-Control_deploy
if exist "%TARGET_DIR%" rmdir /s /q "%TARGET_DIR%"
mkdir "%TARGET_DIR%"

xcopy /E /I /Y "%~dp0ESP32-Web-Server-LED-Control" "%TARGET_DIR%"

cd /d "%TARGET_DIR%"
git init
git branch -M main
git add .
git commit -m "Initial commit: ESP32 Web Server & HTML LED Control (Week 7 Task 1)"
git remote add origin https://github.com/archin123/ESP32-Web-Server-LED-Control.git
echo.
echo Pushing to GitHub...
git push -u origin main

echo.
if %ERRORLEVEL% EQU 0 (
  echo [SUCCESS] Your standalone repository has been published to:
  echo   https://github.com/archin123/ESP32-Web-Server-LED-Control
) else (
  echo [NOTE] If git push asked for credentials or failed, check that the repository
  echo        'ESP32-Web-Server-LED-Control' has been created under your GitHub account.
)
echo.
pause
