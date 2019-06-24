// Laborversuch 4 Aufgabe 5

#include <stdio.h>
#include <stdint.h>
#include "tm4c1294ncpdt.h"

#define BUFFERLENGTH 10                // frei definiert!

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
   UART6_CTL_R |= ((1 << 8) | (1 << 0) | (1 << 9));   // UART6 Transmit and Receive enable
}

void main(int argc, char const *argv[])
{
   unsigned char buffer[BUFFERLENGTH]; 
   unsigned int i = 0;

   configPorts();
   configUART();

   while (1)
   {
      while(i < BUFFERLENGTH)                // Loop wenn Buffer nicht voll ist
      {     
         while(UART6_FR_R & (1 << 4));       // warten bis Rx FIFO leer ist
         buffer[i] = UART6_DR_R;             // byte vom UART6 Datenregister auslesen
         if(buffer[i] == 0x04)               // break loop, wenn "EOT"
         {
            buffer[i] = 0x00;
            break;                           // letztes Element mit '\0' ersetzen um string zu terminieren
         }
         i++;                                // Buffer-Inkrement
      }
      printf("Content of Data Buffer(string): %s\n", buffer);
      printf("Content of Data Buffer(integer): %d\n", buffer);
   }
}
