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
copy "%INSTALLDIR%\scripts\start_Entschluesselung.bat" "c:\Decryption\scripts\"
copy "%INSTALLDIR%\intelligence\Entschluesselung.exe" "c:\Decryption\intelligence\"
copy "%INSTALLDIR%\scripts\start_Verschluesselung.bat" "c:\Decryption\scripts\"
copy "%INSTALLDIR%\intelligence\Verschluesselung.exe" "c:\Decryption\intelligence\"
copy "%INSTALLDIR%\thumbs\key.ico" "c:\Decryption\thumbs"
copy "%INSTALLDIR%\thumbs\lock.ico" "c:\Decryption\thumbs"
copy "%INSTALLDIR%\information\information.txt" "c:\Decryption\"
copy "%INSTALLDIR%\Decryption_uninstall.bat" "c:\Decryption\"
echo Finished Copy Process!

echo.
echo Adding RMB Tools
SET KEYENTSCHL=HKCR\*\shell\DECRYPTION (Decrypt)
SET KEYVERSCHL=HKCR\*\shell\DECRYPTION (Encrypt)

SET PATHENTSCHL=C:\Decryption\scripts\start_Entschluesselung.bat
SET PATHVERSCHL=C:\Decryption\scripts\start_Verschluesselung.bat

SET ICONENTSCHL=C:\Decryption\thumbs\key.ico
SET ICONVERSCHL=C:\Decryption\thumbs\lock.ico

REG ADD "%KEYENTSCHL%" /v "Icon" /t REG_EXPAND_SZ /d "%ICONENTSCHL%" /f
REG ADD "%KEYENTSCHL%\command" /ve /d "%PATHENTSCHL% %%1" /f

REG ADD "%KEYVERSCHL%" /v "Icon" /t REG_EXPAND_SZ /d "%ICONVERSCHL%" /f
REG ADD "%KEYVERSCHL%\command" /ve /d "%PATHVERSCHL% %%1" /f
echo Finished RMB Tools Add

echo.
If Exist "C:\Decryption" goto Successful Else goto Not_Successful
:Successful
echo Installation was successful.
goto END
:Not_Successful
echo Installation was not successful. Please contact content owner! (see information.txt)

:END
echo.
pause