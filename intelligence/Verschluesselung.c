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

#define NAMENSGROESSE  80         /* Namensgroesse für Fileangabe */

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1 << pos));
#define SET_BIT(var,pos) ((var) |= (1 << pos));

typedef unsigned char byte;

/*******************************************************************************
*  Funktions-Deklarationen
*******************************************************************************/
/* Verschluesseslung */
int encrypt (char *LoadFName, char *SaveFName, char *VerschluesselungFName); 

/* Entschluesselung */
int decrypt (char *LoadFName, char *VerschluesselungFName);

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
  char Dateiname [NAMENSGROESSE + 1] = {0};  /* Variable fuer Dateinamen */
  char Verschluesselung[NAMENSGROESSE + 1] = {0}; 							/* Variable fuer Verschluesselung */
  char Verschluesselungstext[1500] = {0};    /* Variable fuer Verschluesselungstext */
  char Taste;
    
        while (--argc)
        if (**++argv != '-')
            printf("Dateipfad: %s\n", *argv);
    
  /* Intro */ 
  puts("Verschluesselung mit Bild");
  puts("-------------------------\n");
  

	 puts("Bitte lege den Verschluesselungstext (.TXT-Datei) im Ordner des");
	 puts("Bildes (BMP) ab und gib hier den Dateinamen ohne Endung ein:")
	 puts("(Oder druecke Enter und gib  manuell einen Verschluesselungstext ein.)");
		  
		scanf("%s", &Verschluesselung);
 
		if (Verschluesselung == "")
		{
			/* Verschluesslungstext file erstellen */
			system("cls");
			puts("Bitte gib hier deinen Verschluesselungstext ein:");
			scanf("%s", &Verschluesselungstext);
			
			FILE * Verschluesselungstextfile;
			strcpy(VerschluesselungFileName, *argv);
			strcat(VerschluesselungFileName, "Verschluesselungstext");
			strcat(VerschluesselungFileName, VERSCHL_ENDING);
			Verschluesselungstextfile = fopen(VerschluesselungFileName,"wb");
			fprintf(Verschluesselungstextfile, Verschluesselungstext);
			fclose(Verschluesselungstextfile);
			
		}
		else
		{
			/* Verschl File nicht erstellen */
			strcpy(VerschluesselungFileName, *argv);
			strcat(VerschluesselungFileName, Verschluesselung);
			strcat(VerschluesselungFileName, VERSCHL_ENDING);
		}
		
		strcpy(LoadFileName, *argv);									//Originalfile Dateiname
		strcat(LoadFileName, DECRYPTED_ENDING);		//zusammensetzen
		 	 
		strcpy(SaveFileName, *argv);									//Ausgabefile Dateiname
		strcat(VerschluesselungFileName, VERSCHL_ENDING);
		 	 
		encrypt(LoadFileName, SaveFileName, VerschluesselungFileName);
			 
		
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
  

  /* Loeschen des PW-Files */
 // remove (VerschluesselungFileName);

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

