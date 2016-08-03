REM Detecting the Parameters

SET Dateiname=%~n1
SET Pfad=%1
SET Dateiendung=%~x1

REM Starting the Encryption (Detecting the Filetype)
if %Dateiendung% == .BMP (goto GOOD_FILETYPE) Else goto REQ2
:REQ2
if %Dateiendung% == .TXT (goto GOOD_FILETYPE) Else goto BAD_FILETYPE

:GOOD_FILETYPE
start /min C:\Decryption\intelligence\Verschluesselung.exe %Dateiname% %Pfad% %Dateiendung%
goto END

:BAD_FILETYPE
MSG * The file-type "%Dateiendung%" does not work with DECRYPTION. To encrypt you need to use a ".BMP" or ".TXT" file!
goto END

:END
exit