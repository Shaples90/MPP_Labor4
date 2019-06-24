// Laborversuch 4 Aufgabe 3

#include <stdio.h>
#include <stdint.h>
#include "tm4c1294ncpdt.h"

void configPorts(void)
{
   SYSCTL_RCGCGPIO_R |= ((1 << 13) | (1 << 3));       // Port-Takt P und D freischalten
   while(!(SYSCTL_PRGPIO_R & ((1 << 13) |(1 << 3)))); // Warte auf Port-Takt-Aktivierung 
   GPIO_PORTP_DEN_R |= (1 << 1);                      // GPIO-Funktion für P(1) enablen
   GPIO_PORTP_AFSEL_R |= (1 << 1);                    // Alternative-Funktion für P(1) enablen
   GPIO_PORTP_PCTL_R |= 0x1 << (4 * 1);               // UART-Funktion für P(1) aktivieren
   
   GPIO_PORTD_AHB_DEN_R |= (1 << 0);                  // PD(0) enablen
   GPIO_PORTD_AHB_DIR_R |= (1 << 0);                  // PD(0) als Output-Signal setzen
}

void configUART1(void)
{
   SYSCTL_RCGCUART_R |= (1 << 6);         // Takt bei UART Modul 6 freischalten
   while(!(SYSCTL_PRUART_R & (1 << 6)));  // Warte auf Takt-Aktivierung bei UART6
   UART6_CTL_R &= ~0x01;                  // UART6 für Konfiguration disablen

   UART6_IBRD_R = 17;                     // BRDI = int(BRD) = int(fcpu / 16 * bitrate) = int(16Mhz / 16 * 57600 bit/s)
   UART6_FBRD_R = 23;                     // BRDF = round((BRD - BRDI) * 2^6)
   UART6_LCRH_R = (0x3 << 5);             // 8/N/1 Format
   UART6_CTL_R |= ((1 << 8) | (1 << 0));  // UART6 und Transmit enable
}

void configUART2(void)
{
   SYSCTL_RCGCUART_R |= (1 << 6);                     // Takt bei UART Modul 6 freischalten
   while(!(SYSCTL_PRUART_R & (1 << 6)));              // Warte auf Takt-Aktivierung bei UART6
   UART6_CTL_R &= ~0x01;                              // UART6 für Konfiguration disablen

   UART6_IBRD_R = 8;                                  // BRDI = int(BRD) = int(fcpu / 16 * bitrate) = int(16Mhz / 16 * 115200 bit/s)
   UART6_FBRD_R = 44;                                 // BRDF = round((BRD - BRDI) * 2^6)
   UART6_LCRH_R = ((0x2 << 5) | (1 << 1) | (1 << 3)); // 7/O/2 Format
   UART6_CTL_R |= ((1 << 8) | (1 << 0));              // UART6 und Transmit enable
}

void configUART3(void)
{
   SYSCTL_RCGCUART_R |= (1 << 6);                     // Takt bei UART Modul 6 freischalten
   while(!(SYSCTL_PRUART_R & (1 << 6)));              // Warte auf Takt-Aktivierung bei UART6
   UART6_CTL_R &= ~0x01;                              // UART6 für Konfiguration disablen

   UART6_IBRD_R = 26;                                 // BRDI = int(BRD) = int(fcpu / 16 * bitrate) = int(16Mhz / 16 * 38400 bit/s)
   UART6_FBRD_R = 3;                                  // BRDF = round((BRD - BRDI) * 2^6)
   UART6_LCRH_R = ((0x1 << 5) | (1 << 1) | (1 << 2)); // 6/E/1 Format
   UART6_CTL_R |= ((1 << 8) | (1 << 0));              // UART6 und Transmit enable
}

void configUART4(void)
{
   SYSCTL_RCGCUART_R |= (1 << 6);            // Takt bei UART Modul 6 freischalten
   while(!(SYSCTL_PRUART_R & (1 << 6)));     // Warte auf Takt-Aktivierung bei UART6
   UART6_CTL_R &= ~0x01;                     // UART6 für Konfiguration disablen

   UART6_IBRD_R = 52;                        // BRDI = int(BRD) = int(fcpu / 16 * bitrate) = int(16Mhz / 16 * 19200 bit/s)
   UART6_FBRD_R = 5;                         // BRDF = round((BRD - BRDI) * 2^6)
   UART6_LCRH_R = ((0x0 << 5) | (1 << 3));   // 5/N/2 Format
   UART6_CTL_R |= ((1 << 8) | (1 << 0));     // UART6 und Transmit enable
}

void main(int argc, char const *argv[])
{
   configPorts();
   configUART1();    // Bitrate: 57600 bit/s, Format: 8/N/1
   //configUART2();  // Bitrate: 115200 bit/s, Format: 7/O/2
   //configUART3();  // Bitrate: 38400 bit/s, Format: 6/E/1
   //configUART4();  // BItrate: 19200 bit/s, Format: 5/N/2

   while (1)
   {
      GPIO_PORTD_AHB_DATA_R &= ~(1 << 0); // Signalisierung der Datenübertragung zurücksetzen
      while((UART6_FR_R & 0x0020) != 0);  // Warte bis UART Transmit FIFO voll ist
      UART6_DR_R = 'E';                   // schreibe 0x45 (ASCII: E) ins UART6 Datenregister
      GPIO_PORTD_AHB_DATA_R |= (1 << 0);  // Start der Datenübertragung signalisieren
      wait(500);
   }
}
