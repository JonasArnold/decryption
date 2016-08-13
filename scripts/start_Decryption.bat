REM Setting Installation Directory
set INSTALLDIR=%PROGRAMFILES%\Decryption

REM Detecting the Parameters
SET file_name=%~n1
SET path=%1
SET file_extension=%~x1

REM Starting the Decryption (Detecting the Filetype)
if %file_extension% == .BMP (goto GOOD_FILETYPE)
if %file_extension% == .bmp (goto GOOD_FILETYPE) Else goto BAD_FILETYPE

:GOOD_FILETYPE
"%INSTALLDIR%\intelligence\Decryption.exe" %file_name% %path%
goto END

:BAD_FILETYPE
echo msgbox"The file-type %file_extension% does not work with DECRYPTION. To decrypt you need to use a .BMP-file!",vbCritical , "Incorrect file type"> %temp%\msg.vbs 
start /B "C:\Windows\system32\cscript.exe" %temp%\msg.vbs 
goto END

:END
exit