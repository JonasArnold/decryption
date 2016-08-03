@echo off

REM /////////////
REM ADMIN ABFRAGE
REM /////////////

echo Request Admin Status

:ADMIN_REQ
fsutil > NUL
if %errorlevel% == 1 (GOTO NO_ADMIN) else (GOTO ADMIN)

:NO_ADMIN
echo NO ADMIN ACCESS
REM MSG * PLEASE RUN THIS SCRIPT AS AN ADMINISTATOR (Rightclick on it -- Run as Administator)
set tempvbs=%temp%\%~n0.vbs
 
echo Dim oShell > "%tempvbs%"
echo Set oShell = CreateObject("Shell.Application") >> "%tempvbs%"
echo oShell.ShellExecute "%~f0", WScript.ScriptFullName, vbNullString, "runas" >> "%tempvbs%"
echo Set oShell = CreateObject("Shell.Application") >> "%tempvbs%"
cscript "%tempvbs%" //nologo
del "%tempvbs%"
goto ADMIN_REQUEST

:ADMIN
echo ADMIN ACCESS
goto START_SCRIPT

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
echo.
echo Removing Files
rd /q /s "C:\Decryption\intelligence"
rd /q /s "C:\Decryption\scripts"
rd /q /s "C:\Decryption\thumbs"
del /q "C:\Decryption\information.txt"
echo Finished Removing Files

echo.
echo Removing RMB Tools
SET KEYENTSCHL="HKCR\*\shell\DECRYPTION (Entschluesseln)"
SET KEYVERSCHL="HKCR\*\shell\DECRYPTION (Verschluesseln)"
reg delete %KEYENTSCHL% /f
reg delete %KEYVERSCHL% /f
echo Finished Removing RMB Tools

echo.
If Not Exist "C:\Decryption\intelligence\Entschluesselung.exe" goto Successful Else goto Not_Successful
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
rd /q /s "C:\Decryption"

:END
exit
