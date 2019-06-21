// Laborversuch 4 Aufgabe 4

#include <stdio.h>
#include <stdint.h>
#include "tm4c1294ncpdt.h"

void wait(unsigned long timevalue)
{
   unsigned int i = 0;
   for(i; i < timevalue; i++);
}
void configPorts(void)
{
   SYSCTL_RCGCGPIO_R |= (1 << 14);        // Port-Takt P freischalten
   while(!(SYSCTL_PRGPIO_R & (1 << 14))); // Warte auf Port-Takt-Aktivierung 
   GPIO_PORTP_DEN_R |= (1 << 1);          // GPIO-Funktion für P(1) enablen
   GPIO_PORTP_DIR_R = (1 << 1);           // P(1) als Output-Signal definieren
   GPIO_PORTP_AFSEL_R |= (1 << 1);        // Alternative-Funktion für P(1) enablen
   GPIO_PORTP_PCTL_R |= 0x1 << (4 * 1);   // UART-Funktion für P(1) aktivieren
}

void configUART(void)
{
   SYSCTL_RCGCUART_R |= (1 << 6);         // Takt bei UART Modul 6 freischalten
   while(!(SYSCTL_PRUART_R & (1 << 6)));  // Warte auf Takt-Aktivierung bei UART6
   UART6_CTL_R &= ~0x01;                  // UART6 für Konfiguration disablen

   UART6_IBRD_R = 781;                    // BRDI = int(BRD) = int(fcpu / 16 * bitrate) = int(120Mhz / 16 * 9600 bit/s)
   UART6_FBRD_R = 16;                     // BRDF = round((BRD - BRDI) * 2^6)
   UART6_LCRH_R = (0x3 << 5);             // 8/N/1 Format
   UART6_CTL_R |= ((1 << 8) | (1 << 0));  // UART6 und Transmit enable
}

void lcDisplayFirstLine(void)
{
   unsigned char firstString[20] = "Versuchsteilnehmer:";
   unsigned int i = 0;

   while((UART6_FR_R & 0x0020) != 0);     
   UART6_DR_R = 0x7C;                     // LC-Display löschen                  
   while((UART6_FR_R & 0x0020) != 0);     
   UART6_DR_R = 0x2D;                     // Cursor 1.Zeile

   while(firstString[i] != '\0')
   {
      while((UART6_FR_R & 0x0020) != 0);
      UART6_DR_R = firstString[i];
      i++;
   } 
}
void lcDisplaySecondLine(void)
{
   unsigned char secondString[20] = "Mateo Narvaez";
   unsigned int i = 0;

   while((UART6_FR_R & 0x0020) != 0);
   UART6_DR_R = 0xFE;                     // Cursor 2.Zeile Anfang
   while((UART6_FR_R & 0x0020) != 0);
   UART6_DR_R = 0xC0;                     // Position 0

   while(secondString[i] != '\0')
   {
      while((UART6_FR_R & 0x0020) != 0);
      UART6_DR_R = secondString[i];
      i++;
   }
}

void lcDisplayThirdLine(void)
{
   unsigned char thirdString[20] = "Nikolas Rieder";
   unsigned int i = 0;

   while((UART6_FR_R & 0x0020) != 0);
   UART6_DR_R = 0xFE;                     // Cursor 3.Zeile Anfang
   while((UART6_FR_R & 0x0020) != 0);  
   UART6_DR_R = 0x94;                     // Position 0

   while(thirdString[i] != '\0')
   {
      while((UART6_FR_R & 0x0020) != 0);
      UART6_DR_R = thirdString[i];
      i++;
   }
}

void lcDisplayForthLine(void)
{
   unsigned char forthString[20] = "Dae-Jin Seon";
   unsigned int i = 0;

   while((UART6_FR_R & 0x0020) != 0);
   UART6_DR_R = 0xFE;                     // Cursor 4.Zeile Anfang
   while((UART6_FR_R & 0x0020) != 0);
   UART6_DR_R = 0xD4;                     // Position 0

   while(forthString[i] != '\0')
   {
      while((UART6_FR_R & 0x0020) != 0);
      UART6_DR_R = forthString[i];
      i++;
   } 
}

void main(int argc, char const *argv[])
{
   configPorts();                         // P(1) für UART konfiguieren
   configUART();                          // Bitrate: 9600 bit/s, Format: 8/N/1

   lcDisplayFirstLine();                  // 'Versuchsteilnehmer:' anzeigen
   lcDisplaySecondLine();                 // 'Mateo Narvaez' anzeigen
   lcDisplayThirdLine();                  // 'Nikolas Rieder' anzeigen
   lcDisplayForthLine();                  // 'Dae-Jin Seon' anzeigen
}