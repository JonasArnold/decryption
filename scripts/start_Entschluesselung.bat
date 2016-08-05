REM Detecting the Parameters

SET Dateiname=%~n1
SET Pfad=%1
SET Dateiendung=%~x1

REM Starting the Decryption (Detecting the Filetype)
if %Dateiendung% == .BMP (goto GOOD_FILETYPE)
if %Dateiendung% == .bmp (goto GOOD_FILETYPE) Else goto BAD_FILETYPE

:BAD_FILETYPE
MSG * The file-type "%Dateiendung%" does not work with DECRYPTION. To decrypt you need to use a ".BMP"-file!
goto END

:GOOD_FILETYPE
start /min C:\Decryption\intelligence\Entschluesselung.exe %Dateiname% %Pfad%
goto END

:END
exit
