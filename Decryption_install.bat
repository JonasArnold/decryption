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

echo.
echo Creating Directorys...
mkdir "c:\Decryption"
mkdir "c:\Decryption\scripts"
mkdir "c:\Decryption\intelligence"
mkdir "c:\Decryption\thumbs"
echo Finished Directory Creation!

echo.
echo Copying Files...
REM Get current Path
set INSTALLDIR=%~dp0
echo Path: %INSTALLDIR%
copy "%INSTALLDIR%\scripts\start_Decryption.bat" "c:\Decryption\scripts\"
copy "%INSTALLDIR%\intelligence\Decryption.exe" "c:\Decryption\intelligence\"
copy "%INSTALLDIR%\scripts\start_Encryption.bat" "c:\Decryption\scripts\"
copy "%INSTALLDIR%\intelligence\Encryption.exe" "c:\Decryption\intelligence\"
copy "%INSTALLDIR%\thumbs\key.ico" "c:\Decryption\thumbs"
copy "%INSTALLDIR%\thumbs\lock.ico" "c:\Decryption\thumbs"
copy "%INSTALLDIR%\Decryption_uninstall.bat" "c:\Decryption\"
copy "%INSTALLDIR%\README.md" "c:\Decryption\"
echo Finished Copy Process!

echo.
echo Adding RMB Tools
SET KEY_DECR=HKCR\*\shell\DECRYPTION (Decrypt)
SET KEY_ENCR=HKCR\*\shell\DECRYPTION (Encrypt)

SET PATH_DECR=C:\Decryption\scripts\start_Decryption.bat
SET PATH_ENCR=C:\Decryption\scripts\start_Encryption.bat

SET ICON_DECR=C:\Decryption\thumbs\key.ico
SET ICON_ENCR=C:\Decryption\thumbs\lock.ico

REG ADD "%KEY_DECR%" /v "Icon" /t REG_EXPAND_SZ /d "%ICON_DECR%" /f
REG ADD "%KEY_DECR%\command" /ve /d "%PATH_DECR% %%1" /f

REG ADD "%KEY_ENCR%" /v "Icon" /t REG_EXPAND_SZ /d "%ICON_ENCR%" /f
REG ADD "%KEY_ENCR%\command" /ve /d "%PATH_ENCR% %%1" /f
echo Finished RMB Tools Add

echo.
If Exist "C:\Decryption" goto Successful Else goto Not_Successful
:Successful
echo Installation was successful.

REM MSGBOX - SUCCESSFUL INSTALLATION
echo msgbox"The Installation of DECRYPTION was successful!",vbInformation , "Installation successful"> %temp%\msg.vbs 
%temp%\msg.vbs 
erase %temp%\msg.vbs

exit

:Not_Successful
echo Installation was not successful. Please contact content owner! Github: jonasarnold

:END
echo.
pause