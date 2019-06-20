// Laborversuch 4 Aufgabe 3

#include <stdio.h>
#include <stdint.h>
#include "tm4c1294ncpdt.h"

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

   UART6_IBRD_R = 130;                    // BRDI = int(120Mhz / 16 * 57600 bit/s)
   UART6_FBRD_R = 13;                     // BRDF = round((BRD - BRDI) * 2^6)
   UART6_LCRH_R = 0x3 << 5;               // 8/N/1 Format
   UART6_CTL_R |= (1 << 8) | (1 << 0);    // UART6 und Transmit enable
}

void main(int argc, char const *argv[])
{
   configPorts();
   configUART();

   while (1)
   {
      while((UART6_FR_R & (1 << 5)) != 0);
      UART6_DR_R = 'E';
   }
}
