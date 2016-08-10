/*******************************************************************************
********************************************************************************
* Programm:   Verschluesselung Bild
*
* Filename:   Verschluesselung_Bild.c	   			   
*									   									   
* Autor:      Jonas Arnold					  						   
*									   									   
* Version:    1.0				   			   
*									   									   
* Datum:      07.04.16
*
* Entwicklungsablauf(Version, Datum, Autor, Entwicklungsschritt):
* - 1.0   07.04.16  ARJO   ganzes Programm erstellt
*									   									   
********************************************************************************
*
* Verwendungszweck: 
* 
*
* Beschreibung:
* 
*
* Benötigte Libraries:
* - stdlib.h
* - stdio.h
* - string.h
*
*
*
* Copyright (c) 2016 by J. Arnold, CH-6410 Goldau
********************************************************************************
*******************************************************************************/


/*******************************************************************************
*  Include Files
*******************************************************************************/
#include <stdlib.h> /* Funktionsbibliothek: Hilfsfunktionen */
#include <stdio.h>  /* Funktionsbibliothek: Standard Ein- Ausgabe */
#include <string.h> /* Funktionen zur Stringbearbeitung */

/*******************************************************************************
*  Globale Deklarationen und Definitionen
*******************************************************************************/
#define DECRYPTED_ENDING ".BMP"
#define ENCRYPTED_ENDING ".BMP"
#define VERSCHL_ENDING ".TXT"
#define ENTSCHL_ADD "_ENTSCHL"
#define ENCRYPTED_ADD "_VERSCHL"

/* ErrorCodes */
#define ERROR_OPEN_LOAD_FILE  11  /* Originalfile kann nicht gelesen werden */
#define ERROR_OPEN_SAVE_FILE  21  /* Ausgabefile kann nicht geschr. werden */
#define ERROR_OPEN_VERSCHL_FILE				31		/* Passwort-File kann nicht geschr. wertden */
#define NO_ERROR               0  /* Kein Fehler vorhanden */

#define NAMENSGROESSE  120         /* Namensgroesse für Fileangabe */

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1 << pos))
#define SET_BIT(var,pos) ((var) |= (1 << pos))

/* Parameteruebergabe Definition */
#define DATEINAME argv[1]
#define PFAD argv[2]
#define DATEIENDUNG argv[3]

typedef unsigned char byte;

/*******************************************************************************
*  Funktions-Deklarationen
*******************************************************************************/
/* Verschluesseslung */
int encrypt (char *LoadFName, char *SaveFName, char *VerschluesselungFName); 

/* Verschluesselungs File Erstellung */
int createVerschlFile (char *VerschluesselungFName, char Verschluesselung[1500]);

/* Function to read out the Config File */
int readoutConfigFile (char ConfigFilePath[80], char SettingName[50], char *ValueOutput);

/* PW - File */
//int createVerschlFile (char *VerschluesselungFileName, char Verschluesselung[255]);  


/*******************************************************************************
******************************* HAUPTPROGRAMM **********************************
*******************************************************************************/
main(argc, argv) /* Argumente an main-Funktion */
int argc;
char *argv[];
{
	int Error = NO_ERROR;                   		 /* Errorcode der Filezugriffe */
	char LoadFileName [NAMENSGROESSE + 1];   		/* Variable fuer Originalfile */  
	char SaveFileName [NAMENSGROESSE + 1];   		/* Variable fuer Sicherungsfile */
	char VerschluesselungFileName [NAMENSGROESSE + 1]; /* Variable fuer Ver-File */
	char Dateiname [NAMENSGROESSE + 1] = "";  /* Variable fuer Dateinamen */
	char Verschluesselung[NAMENSGROESSE + 1] = ""; 							/* Variable fuer Verschluesselung */
	char Verschluesselungstext[1500] = {0};    /* Variable fuer Verschluesselungstext */
	char Taste;
	byte File_creation = 0;
	char ConfigFilePath[] = "C:/Decryption/config/config.txt"; 	// path to the config file
	char SettingValue[20];										// value of the setting in config file
  
  /* Verschluesselung eines Bildes angefordert */
  puts("Encryption with Picture");
  puts("-----------------------\n");

	 puts("Please drag and drop your TXT-file with the text into this command");
	 puts("window. If you want to type some text you can just type in the");
		puts("keyword 'type'. Then press ENTER.\n\n");
		  
		scanf("%s", &Verschluesselung);
 
		if (strcmp(Verschluesselung, "type") == 0 ||
						strcmp(Verschluesselung, "TYPE") == 0 ||
						strcmp(Verschluesselung, "Type") == 0)
		{
			/* Verschluesslungstext file erstellen */
			File_creation = 1;
			system("cls");
			puts("Please type your text here: (ENTER key will start the Encryption!)");
			
			fflush(stdin);	//Flush
			fgets(Verschluesselungstext, 1500, stdin); //Text einlesen (max. 1500 Zeichen)
			
			/* Pfad fuer Verschluesselungstext file zusammensetzen */
			strcpy(VerschluesselungFileName, PFAD);
			strcat(VerschluesselungFileName, "Encryption_text");
			strcat(VerschluesselungFileName, VERSCHL_ENDING);
			Error = createVerschlFile(VerschluesselungFileName, Verschluesselungstext);
		}
		else
		{
			/* Verschl File nicht erstellen */
			strcpy(VerschluesselungFileName, Verschluesselung);
		}
		
		/* Original file zusammensetzen */
		strcpy(LoadFileName, PFAD);
		strcat(LoadFileName, DATEINAME);
		strcat(LoadFileName, DATEIENDUNG);
		 	 
		 	 
		/* Output file zusammensetzen */
		strcpy(SaveFileName, PFAD);
		strcat(SaveFileName, DATEINAME);
		strcat(SaveFileName, ENCRYPTED_ADD);
		strcat(SaveFileName, DATEIENDUNG);
		 	 
		printf("Encryption processing...");
		Error = encrypt(LoadFileName, SaveFileName, VerschluesselungFileName);
			 
	
		/* read the File Deletion Setting from config.txt */
		Error = readoutConfigFile(ConfigFilePath, "Enable_Encryption_Text_File_Deletion", SettingValue);

		/* Encryption_text.TXT File deletion if enabled in config */
		if ((strcmp(SettingValue, "true") == 0) && File_creation == 1)
		{
 			remove(VerschluesselungFileName);
		}

		system("cls");
		
		/* Errors auswerten */
		if (Error == NO_ERROR) 
  { printf ("\n-> Ausgabefile konnte erstellt werden.\n");	} 
  else {
    printf ("ACHTUNG: Der Sicherungsvorgang konnte nicht umgesetzt werden!\n");
    printf ("-> Errorcode: %d\n", Error);
    switch (Error) {
      case ERROR_OPEN_LOAD_FILE: { /* Problem mit Originalfile */
             printf ("-> Originalfile konnte nicht geoeffnet werden!\n");
             printf ("   Lesezugriff fuer Originalfile nicht moeglich!\n");              
             break;
      }
      case ERROR_OPEN_SAVE_FILE: { /* Problem mit Sicherungsfile */
             printf ("-> Das Ausgabefile konnte nicht erstellt werden!\n");
             printf ("   Schreibzugriff fuer Ausgabefile wurde verweigert!\n");
             break;
						}
      case ERROR_OPEN_VERSCHL_FILE: {  /* Problem mit PW-File */
      							printf("-> Das Passwort-File konnte nicht erstellt werden!\n");
      							printf("   Schreibzugriff fuer Passwort-File wurde verweigert!\n");
													break;
      }
    }
 	}

  return(0);
}             


/*******************************************************************************
*************************** FUNKTIONS-DEFINITIONEN *****************************
*******************************************************************************/

/*******************************************************************************
********************************************************************************
* Funktion:  Verschluesselung
*
* Source: 	  Verschluesselung_Satz.c
*
* Call:      encrypt (char * LoadFName, char * SaveFName, char * Passwort);	   			   
*									   									   
* Autor:     Jonas Arnold			  						   
*									   									   
* Version:   1.0				   			   
*									   									   
* Datum: 	   07.04.16
*
* Entwicklungsablauf(Version, Datum, Autor, Entwicklungsschritt):
* -  1.0   07.04.16   ARJO   ganze Funktion erstellt
*									   									   
********************************************************************************
*
* Verwendungszweck: 
* 
*
* Beschreibung:
* 
* 
* 
* Precondition:
* Fuer erfolgreichen Kopiervorgang gilt:  
* - Originalfile ist vorhanden und befindet sich im entsprechenden Ordner
* - Ausgabefile kann im entsprechenden Ordner erstellt werden (schreibrecht)
*
* Postcondition:
* Wenn Originalfile nicht vorhanden oder nicht lesbar -> Fehlermeldung wird
*  ausgegeben.
* Wenn Ausgabefile nicht geschrieben werden kann -> Fehlermeldung wird
*  ausgegeben.
*
* Benötigte Unterprogramme:
* 
*
* Parameter: (I: Input, O: Output, U: Update)
* -
*
* Copyright (c) 2016 by J.Arnold, CH-6410 Goldau
********************************************************************************
*******************************************************************************/ 
int encrypt (char *LoadFName, char *SaveFName, char *VerschluesselungFName) {

  int  ErrorCode = NO_ERROR;   /* Error-Code-Variable */
  byte Zeichen;                /* Das eingelesene Zeichen aus dem Vers. File*/
  FILE * LoadFile;             /* File welches geladen wird */
  FILE * SaveFile;             /* File welches gespeichert wird */  
  FILE * VerschluesselungFile; /* File welches den Verschl. Text beinahltet */
  int i = 0;                   /* Zaehlervariabel */
  byte BildByte;               /* Byte welches aus dem Bild ausgelesen wird */
  char EndZeichen = 0x03;      /* Zeigt an ob das Enzeichen gesetzt (1) wurde */
 

  /* Kontrolliertes oeffnen der Files */
  if ((LoadFile = fopen (LoadFName, "rb")) == NULL)
    ErrorCode = ERROR_OPEN_LOAD_FILE;
  if ((SaveFile = fopen (SaveFName, "wb")) == NULL)
    ErrorCode = ERROR_OPEN_SAVE_FILE;
  if ((VerschluesselungFile = fopen (VerschluesselungFName, "rb")) == NULL)
    ErrorCode = ERROR_OPEN_VERSCHL_FILE;
   
  /* Alle Filezugriffe moeglich ? */
  if (ErrorCode == NO_ERROR) {  
			
			 //Header
			 for (i=0; i<100; i++)
			 {
			   Zeichen = fgetc(LoadFile);
			   fputc(Zeichen, SaveFile);
			 }
			 

		  //Verschluesselungstext hineinkopieren
			 while (!feof(LoadFile))
			 {
      do 
      {
        Zeichen = fgetc (VerschluesselungFile);
        
        if(Zeichen != 0xFF)
        {
									 // Byte des Zeichens auseinandernehmen und 
		  			   for (i=0; i<=7; i++)
		  			   {
		  			     BildByte = fgetc(LoadFile);
		  			     CLEAR_BIT(BildByte, 0);
		  			     if(CHECK_BIT(Zeichen,i)) // Wenn das aktuelle Bit des Buchstabens 1 ist
		  			     {
		  			       SET_BIT(BildByte, 0);
		  			     }
		          fputc(BildByte, SaveFile);
		  			   }
								}
      } while (!feof(VerschluesselungFile));
      
      // Endzeichen setzen (Textende)
      if (EndZeichen != 0)
      {
      	 Zeichen = EndZeichen;
  			   for (i=0; i<=7; i++)
  			   {
  			     BildByte = fgetc(LoadFile);
  			     CLEAR_BIT(BildByte, 0);
  			     if(CHECK_BIT(Zeichen,i)) // Wenn das aktuelle Bit des Buchstabens 1 ist
  			     {
  			       SET_BIT(BildByte, 0);
  			     }
          fputc(BildByte, SaveFile);
  			   }
  			   EndZeichen = 0;
      }
      
      Zeichen = fgetc (LoadFile);
			   fputc(Zeichen, SaveFile);
			 }
  }

  /* Wenn Orignalfile geoffnet werden konnte */
  if (ErrorCode != ERROR_OPEN_LOAD_FILE)  
    fclose (LoadFile);
  /* Wenn Ausgabefile geschrieben werden konnte  */
  if (ErrorCode != ERROR_OPEN_SAVE_FILE) 
    fclose (SaveFile);  
    /* Wenn PW-File geschrieben werden konnte  */
  if (ErrorCode != ERROR_OPEN_VERSCHL_FILE) 
    fclose (VerschluesselungFile); 
 
  return (ErrorCode);  /* Rueckgabe des Fehlercodes */
}   

/*******************************************************************************
********************************************************************************
* Funktion:  Password File erstellen
*
* Source: 	  Verschluesselung_Satz.c
*
* Call:      createPasswordFile (char * LoadFName, char * SaveFName, char * PasswordFName);	   			   
*									   									   
* Autor:     Jonas Arnold			  						   
*									   									   
* Version:   1.0				   			   
*									   									   
* Datum: 	   07.04.16
*
* Entwicklungsablauf(Version, Datum, Autor, Entwicklungsschritt):
* -  1.0   07.04.16   ARJO   ganze Funktion erstellt
*									   									   
********************************************************************************
*
* Verwendungszweck: 
* 
*
* Beschreibung:
* 
* 
* 
* Precondition:
* Fuer erfolgreichen Kopiervorgang gilt:  
* - Originalfile ist vorhanden und befindet sich im entsprechenden Ordner
* - Ausgabefile kann im entsprechenden Ordner erstellt werden (schreibrecht)
*
* Postcondition:
* Wenn Originalfile nicht vorhanden oder nicht lesbar -> Fehlermeldung wird
*  ausgegeben.
* Wenn Ausgabefile nicht geschrieben werden kann -> Fehlermeldung wird
*  ausgegeben.
*
* Benötigte Unterprogramme:
* 
*
* Parameter: (I: Input, O: Output, U: Update)
* -
*
* Copyright (c) 2016 by J.Arnold, CH-6410 Goldau
********************************************************************************
*******************************************************************************/ 
int createVerschlFile (char *VerschluesselungFName, char Verschluesselung[1500])
{

  int  ErrorCode = NO_ERROR;   /* Error-Code-Variable */
  FILE * VerschluesselungFile;               /* File welches geladen wird */

  /* Kontrolliertes oeffnen der Files */
  if ((VerschluesselungFile = fopen (VerschluesselungFName, "wb")) == NULL)
    ErrorCode = ERROR_OPEN_VERSCHL_FILE;
   
  /* Alle Filezugriffe moeglich ? */
  if (ErrorCode == NO_ERROR) {  
     fputs (Verschluesselung, VerschluesselungFile);  
  }

  /* Wenn PW File geoffnet werden konnte */
  if (ErrorCode != ERROR_OPEN_VERSCHL_FILE)  
    fclose (VerschluesselungFile); 
 
  return (ErrorCode);  /* Rueckgabe des Fehlercodes */
}  



/***************************************************************************************
****************************************************************************************
* Function:	readoutConfigFile
*
* Call:		ErrorVariable = readoutConfigFile(ConfigFilePath, SettingName, ValueOutput);	   			   
*									   									   
* Author:	Jonas Arnold			  						   
*									   									   
* Version:	1.0				   			   
*									   									   
* Date:		10.08.2016
*									   									   
****************************************************************************************
*
* Usage: 
* Used to read out the value of a setting in a configuration file.
* Originally developped for DECRYPTION (github.com/jonasarnold/decryption)
*
*
* Description:
* ConfigFilePath: a string, containing the file path of the config file
* SettingName: a string containing the name of the setting
* ValueOutput: a pointer pointing at a string to write in the value of the setting
*
*
* Example definition and call:
*   
* 	char ConfigFilePath [80]; 			// path to the config file
*	char SettingName [50];				// name of the setting in config file
*	char SettingValue[20];				// value of the setting in config file
*
*	Error = readoutConfigFile(ConfigFilePath, SettingName, SettingValue);
*
* Note to the 'Error' Variable:
* 'Error' is a int variable to detect errors while opening/closing a file.
* Errors will be returned by the function. To use the error codes create the following
* constants and give them a number (= ErrorCode):
*   'NO_ERROR', 'ERROR_OPEN_LOAD_FILE', 'ERROR_OPEN_LOAD_FILE'
*
* Pre-condition:
* Config-File available und accessible.
* Settings liste in this style (without apostrophes):
* 'SettingNameWithNoWhitespaces=ValueOfTheSetting'
*
*
* Post-condition:
* The value of the requested setting in the config file will be stored
* in the given string parameter "ValueOutput".
*
* Copyright (c) 2016 by Jonas Arnold
********************************************************************************
*******************************************************************************/ 
int readoutConfigFile (char ConfigFilePath[80], char SettingName[50], char *ValueOutput){

 	int  ErrorCode = NO_ERROR;   	/* Error-Code variable */
 	FILE * ConfigFile;              /* File Stream to config file */
	int line_number = 1;			/* number of the line (file) */
	char temp[512];					/* temporary array */
	char delimiter[] = "=";			/* between value and name of the setting */
	char FileValue[50];				/* value read from the file */
	char *pointer_value;			/* points on the value of the setting */			
	char OutputValue[50];			/* final value to return */

  	/* controlled opening of the file */
  	if ((ConfigFile = fopen (ConfigFilePath, "r")) == NULL)
  	  ErrorCode = ERROR_OPEN_LOAD_FILE;
   
  	/* file access possible ? */
  	if (ErrorCode == NO_ERROR)
  	{  
  		/* search the file for strings */
  		while(fgets(temp, 512, ConfigFile) != NULL)
		{
			/* search the specific string */ 
			if((strstr(temp, SettingName)) != NULL) {
				strcpy(FileValue, temp);   //copy the text to the variable
				}	
			line_number++;  //increment the line number
		}
	}
	
	/* limiting the string to where the value ends */
	FileValue[sizeof(FileValue)-1] = '\0';
		
	/* initialize delimination */
	pointer_value = strtok(FileValue, delimiter);

	/* get value */
	pointer_value = strtok(NULL, delimiter);
	
	/* copy value to the return-pointer */
	strcpy(ValueOutput, pointer_value);
	
	/* close file */
	if (ErrorCode != ERROR_OPEN_LOAD_FILE)  
    fclose (ConfigFile);
  	
  	
	return(ErrorCode);
}   
