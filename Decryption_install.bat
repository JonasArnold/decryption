@echo off

REM /////////////
REM ADMIN ABFRAGE
REM /////////////

:ADMIN_REQ
REM  --> Check for permissions
    IF "%PROCESSOR_ARCHITECTURE%" EQU "amd64" (
>nul 2>&1 "%SYSTEMROOT%\SysWOW64\cacls.exe" "%SYSTEMROOT%\SysWOW64\config\system"
) ELSE (
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
)

REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    set params = %*:"=""
    echo UAC.ShellExecute "cmd.exe", "/c ""%~s0"" %params%", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    pushd "%CD%"
    CD /D "%~dp0"

REM //////////////////
REM ENDE ADMIN ABFRAGE
REM //////////////////


REM ////////////
REM INSTALLATION
REM ////////////

:START_INSTALLATION
cls
echo DECRYPTION
echo ----------
echo.
echo Installation started...

REM Get current Path
set CURRENTPATH=%~dp0
set INSTALLDIR=%PROGRAMFILES%\Decryption
echo Path of the Installer-Files: %CURRENTPATH%
echo Path to install the software: %INSTALLDIR%

echo.
echo Creating Directorys...
mkdir "%INSTALLDIR%"
mkdir "%INSTALLDIR%\scripts"
mkdir "%INSTALLDIR%\intelligence"
mkdir "%INSTALLDIR%\thumbs"
mkdir "%INSTALLDIR%\config"
echo Finished Directory Creation!

echo.
echo Copying Files...
copy "%CURRENTPATH%\scripts\start_Decryption.bat" "%INSTALLDIR%\scripts\"
copy "%CURRENTPATH%\intelligence\Decryption.exe" "%INSTALLDIR%\intelligence\"
copy "%CURRENTPATH%\scripts\start_Encryption.bat" "%INSTALLDIR%\scripts\"
copy "%CURRENTPATH%\intelligence\Encryption.exe" "%INSTALLDIR%\intelligence\"
copy "%CURRENTPATH%\thumbs\key.ico" "%INSTALLDIR%\thumbs"
copy "%CURRENTPATH%\thumbs\lock.ico" "%INSTALLDIR%\thumbs"
copy "%CURRENTPATH%\config\config.txt" "%INSTALLDIR%\config"
copy "%CURRENTPATH%\Decryption_uninstall.bat" "%INSTALLDIR%"

REM Looking for existing Readme file (rename can't be forced)
if exist "%INSTALLDIR%\README.txt" goto Delete_old_Readme else goto No_old_readme

:Delete_old_Readme
erase %INSTALLDIR%\README.txt
:No_old_readme
REM Renaming Readme-file
copy "%CURRENTPATH%\README.md" "%INSTALLDIR%"
ren "%INSTALLDIR%\README.md" "README.txt"


echo Finished Copy Process!

echo.
echo Adding RMB Tools
SET KEY_DECR=HKCR\*\shell\DECRYPTION (Decrypt)
SET KEY_ENCR=HKCR\*\shell\DECRYPTION (Encrypt)

SET PATH_DECR=%INSTALLDIR%\scripts\start_Decryption.bat
SET PATH_ENCR=%INSTALLDIR%\scripts\start_Encryption.bat

SET ICON_DECR=%INSTALLDIR%\thumbs\key.ico
SET ICON_ENCR=%INSTALLDIR%\thumbs\lock.ico

REG ADD "%KEY_DECR%" /v "Icon" /t REG_EXPAND_SZ /d "%ICON_DECR%" /f
REG ADD "%KEY_DECR%\command" /ve /d "%PATH_DECR% %%1" /f

REG ADD "%KEY_ENCR%" /v "Icon" /t REG_EXPAND_SZ /d "%ICON_ENCR%" /f
REG ADD "%KEY_ENCR%\command" /ve /d "%PATH_ENCR% %%1" /f
echo Finished RMB Tools Add

echo.
If Exist %INSTALLDIR% goto Successful Else goto Not_Successful
:Successful
echo Installation was successful.

REM MSGBOX - SUCCESSFUL INSTALLATION
echo msgbox"The Installation of DECRYPTION was successful!",vbInformation , "Installation successful"> %temp%\msg.vbs 
echo.
cscript %temp%\msg.vbs 
erase %temp%\msg.vbs

exit

:Not_Successful
echo Installation was not successful. Please contact content owner! Github: jonasarnold

:END
echo.
pause