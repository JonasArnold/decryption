REM Setting Installation Directory
set INSTALLDIR=%PROGRAMFILES%\Decryption

REM Detecting the Parameters
SET file_name=%~n1
SET path=%~dp1
SET file_extension=%~x1

REM Starting the Encryption (Detecting the Filetype)
if %file_extension% == .BMP (goto GOOD_FILETYPE)
if %file_extension% == .bmp (goto GOOD_FILETYPE)

if %file_extension% == .TXT (goto GOOD_FILETYPE)
if %file_extension% == .txt (goto GOOD_FILETYPE) Else goto BAD_FILETYPE

:GOOD_FILETYPE
"%INSTALLDIR%\intelligence\Encryption.exe" %file_name% %path% %file_extension%
goto END

:BAD_FILETYPE
MSG * The file-type "%file_extension%" does not work with DECRYPTION. To encrypt you need to use a ".BMP" or ".TXT" file!
goto END

:END
exit
