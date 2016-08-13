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


REM //////////////
REM UNINSTALLATION
REM //////////////

:START_SCRIPT
cls
echo DECRYPTION UNINSTALL
echo --------------------

set /p  ANSWER="You are about to uninstall Decryption with all it's components. Type "Y" if you want to proceed:"
IF /I "%ANSWER%" == "Y" goto START_UNINSTALLATION
IF /I "%ANSWER%" == "y" goto START_UNINSTALLATION
IF /I "%ANSWER%" == "Yes" goto START_UNINSTALLATION
IF /I "%ANSWER%" == "yes" goto START_UNINSTALLATION

exit

:START_UNINSTALLATION

REM Get Paths
set INSTALLDIR=%PROGRAMFILES%\Decryption
set CONFIGPATH=%APPDATA%\Decryption
echo Path to installed software: %INSTALLDIR%
echo Path of the config file: %CONFIGPATH%

echo.
echo Removing Files
rd /q /s "%INSTALLDIR%\intelligence"
rd /q /s "%INSTALLDIR%\scripts"
rd /q /s "%INSTALLDIR%\thumbs"
rd /q /s "%CONFIGPATH%\config"
del /q "%INSTALLDIR%\README.txt"
echo Finished Removing Files

echo.
echo Removing RMB Tools
SET KEY_DECR="HKCR\*\shell\DECRYPTION (Decrypt)"
SET KEY_ENCR="HKCR\*\shell\DECRYPTION (Encrypt)"
reg delete %KEY_DECR% /f
reg delete %KEY_ENCR% /f
echo Finished Removing RMB Tools

echo.
If Not Exist "%INSTALLDIR%\intelligence\Decryption.exe" goto Successful Else goto Not_Successful
:Successful
echo Uninstallation was successful.
echo.
pause
goto DEL_UNINSTALLFILE

:Not_Successful
echo Uninstallation was not successful. Please delete folder maually: "C:\Decryption"
echo.
pause


:DEL_UNINSTALLFILE
rd /s /q "%INSTALLDIR%"

:END
exit
