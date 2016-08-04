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
  char Taste;
  
    while (--argc)
        if (**++argv != '-')
            printf("Dateipfad: %s\n", *argv);
    
  /* Intro */ 
  puts("Verschluesselung mit Bild");
  puts("-------------------------\n");  
  

 
	 strcpy(LoadFileName, *argv);									//Originalfile Dateiname
	 
	 /* Verschl File erstellen */
		strcpy(VerschluesselungFileName, *argv);
		strcat(VerschluesselungFileName, ENTSCHL_ADD);
		strcat(VerschluesselungFileName, VERSCHL_ENDING);
	 
	 puts("Enschluesseln startet....");
	 decrypt(LoadFileName, VerschluesselungFileName);
			
		
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
* Funktion:  Entschluesselung
*
* Source: 	  Verschluesselung_Satz.c
*
* Call:      decrypt (char * LoadFName, char * SaveFName, char * Passwort);	   			   
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
int decrypt (char *LoadFName, char *VerschluesselungFName) {

  int  ErrorCode = NO_ERROR;   /* Error-Code-Variable */
  char  Zeichen;               /* Das eingelesene Zeichen aus dem Vers. File*/
  FILE * LoadFile;             /* File welches geladen wird */
  FILE * VerschluesselungFile; /* File welches den Verschl. Text beinahltet */
  int i = 0;                   /* Zaehlervariabel */
  byte BildByte;               /* Byte welches aus dem Bild ausgelesen wird */
  char EndZeichen = 0x03;						/* Zeigt an ob das Enzeichen gesetzt (1) wurde */
 

  /* Kontrolliertes oeffnen der Files */
  if ((LoadFile = fopen (LoadFName, "rb")) == NULL)
    ErrorCode = ERROR_OPEN_LOAD_FILE;
  if ((VerschluesselungFile = fopen (VerschluesselungFName, "wb")) == NULL)
    ErrorCode = ERROR_OPEN_VERSCHL_FILE;
   
  /* Alle Filezugriffe moeglich ? */
  if (ErrorCode == NO_ERROR)
  {  
    //Header
  	 for (i=0; i<100; i++)
  	 {
  	   BildByte = fgetc(LoadFile);
  	 }
    
    while (Zeichen != EndZeichen) 
    {
      Zeichen = 0;
      for (i=0; i<=7; i++)
      {
        BildByte = fgetc (LoadFile);
        if(CHECK_BIT(BildByte, 0))
        {
          SET_BIT(Zeichen, (i));
        }
      }
      
      if(Zeichen != EndZeichen)
      {
        fputc(Zeichen, VerschluesselungFile);
						}
    } 
  }
  

  /* Wenn Orignalfile geoffnet werden konnte */
  if (ErrorCode != ERROR_OPEN_LOAD_FILE)  
    fclose (LoadFile);
    /* Wenn Veschl-File geschrieben werden konnte  */
  if (ErrorCode != ERROR_OPEN_VERSCHL_FILE) 
    fclose (VerschluesselungFile); 
 
  return (ErrorCode);  /* Rueckgabe des Fehlercodes */
}  


