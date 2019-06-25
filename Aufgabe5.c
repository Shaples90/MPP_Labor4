// Laborversuch 4 Aufgabe 5

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "tm4c1294ncpdt.h"

#define BUFFERLENGTH 13                // frei definiert!

void wait(unsigned long timevalue)
{
   unsigned int i = 0;
   for(i; i < timevalue; i++);         // wait loop
}

void configPorts(void)
{
   SYSCTL_RCGCGPIO_R |= (1 << 13);                 // Port-Takt P freischalten
   while(!(SYSCTL_PRGPIO_R & (1 << 13)));          // Warte auf Port-Takt-Aktivierung
   GPIO_PORTP_DEN_R |= ((1 << 1) | (1 << 0));      // GPIO-Funktion für P(1:0) enablen
   GPIO_PORTP_AFSEL_R |= ((1 << 1) | (1 << 0));    // Alternative-Funktion für P(1:0) enablen
   GPIO_PORTP_PCTL_R |= ((1 << 4) | (1 << 0));     // UART-Funktion für P(0) U6Rx und P(1) U6Tx aktivieren
}

void configUART(void)
{
   SYSCTL_RCGCUART_R |= (1 << 6);                     // Takt bei UART Modul 6 freischalten
   while(!(SYSCTL_PRUART_R & (1 << 6)));              // Warte auf Takt-Aktivierung bei UART6
   UART6_CTL_R &= ~0x01;                              // UART6 für Konfiguration disablen

   UART6_IBRD_R = 104;                                // BRDI = int(BRD) = int(fcpu / 16 * bitrate) = int(16Mhz / 16 * 9600 bit/s)
   UART6_FBRD_R = 11;                                 // BRDF = round((BRD - BRDI) * 2^6)
   UART6_LCRH_R = (0x3 << 5);                         // 8/N/1 Format
   UART6_CTL_R |= ((1 << 9) | (1 << 8) | (1 << 0));   // UART6 Transmit and Receive enable
}

void main(int argc, char const *argv[])
{
   char buffer[BUFFERLENGTH];
   unsigned char orig[7] = "Orig: ";

   configPorts();
   configUART();                                               // Bitrate: 9600 bit/s, Format: 8/N/1

   while(1)
   {
	  char szNumbers[8];
	  long int li;
	  char *pEnd;
	  unsigned int j = 0;
	  unsigned int k = 0;
	  unsigned int m = 0;
	  unsigned int i = 0;
	  unsigned int l = 0;

      while(i < BUFFERLENGTH)                                  // Loop wenn Buffer nicht voll ist
      {
         while(UART6_FR_R & (1 << 4));                         // warten bis Rx FIFO leer ist
         buffer[i] = UART6_DR_R;                               // byte vom UART6 Datenregister auslesen und zwischenspeichern
         if(buffer[i] == 0x03)                                 // break loop, wenn "End of Transmission"
         {
            buffer[i] = 0x00;
            break;                                             // letztes Element mit '\0' ersetzen um string zu terminieren
         }
         i++;                                                  // Buffer-Inkrement
      }

      for(k = 3; k < 11; k++)
      {
    	  szNumbers[m] = buffer[k];
    	  m++;
      }

      li = strtol(szNumbers, &pEnd, 16);           				// konvertiere hex-string zu dec-integer

      sprintf(szNumbers, "ID Number: %ld\n", li);				// konvertiere dec-integer zu dec-string

      while((UART6_FR_R & 0x0020) != 0);                       // Warte bis UART Transmit FIFO voll ist
      UART6_DR_R = 0x7C;                                       // LC-Display löschen
      while((UART6_FR_R & 0x0020) != 0);                       // Warte bis UART Transmit FIFO voll ist
      UART6_DR_R = 0x2D;                                       // Cursor 1.Zeile

      i = 0;
      while(szNumbers[i] != '\0')                                // Loop solange bis zum Ende des Strings
      {
         while((UART6_FR_R & 0x0020) != 0);                    // Warte bis UART Transmit FIFO voll ist
         UART6_DR_R = (szNumbers[i]);                            // Schreibe Zeichen ins UART Datenregister
         i++;                                                  // Gehe zum nächsten Zeichen des Strings
      }
      i = 0;

      while((UART6_FR_R & 0x0020) != 0);                       // Warte bis UART Transmit FIFO voll ist
      UART6_DR_R = 0xFE;                                       // Cursor 2.Zeile Anfang
      while((UART6_FR_R & 0x0020) != 0);                       // Warte bis UART Transmit FIFO voll ist
      UART6_DR_R = 0xC0;                                       // Position 0

      while(orig[i] != '\0')
      {
    	  while((UART6_FR_R & 0x0020) != 0);
    	  UART6_DR_R = orig[i];
    	  i++;
      }
      i = 0;

      l = 1;
      for(l; l < 13; l++)                                // Loop solange bis zum Ende des Strings
      {
    	 while((UART6_FR_R & 0x0020) != 0);                    // Warte bis UART Transmit FIFO voll ist
	     UART6_DR_R = buffer[l];                            // Schreibe Zeichen ins UART Datenregister
	  }
      l = 1;
   }
}
