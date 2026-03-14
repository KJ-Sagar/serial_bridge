#include <stdio.h>
#include "pico/stdlib.h"
#include "pico_espat_udp_utils.h"

char WIFI_SSID[] = "D108-6G";
char WIFI_PASSWORD[] = "9533112237";

int main() {
    stdio_init_all();
    
    
    sleep_ms(10000);
    printf("Testing ESPAT\n");
    printf("%d\n", espat_init());

    printf("Testing WifiConn\n");
    printf("%d\n", connect_to_wifi(WIFI_SSID, WIFI_PASSWORD));


    
    while (true) {
        // PC → ESP8266
        int ch = getchar_timeout_us(0);
        if (ch != PICO_ERROR_TIMEOUT) {
            uart_putc(UART_ID, (char)ch);
        }

        // ESP8266 → PC
        if (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            putchar(c);
        }
    }

}
































/*     sleep_ms(5000);
    printf("UART Bridge Ready\n");
    
    char buffer[100];

    // WiFi Configuration
    printf("Setting WiFi mode...\n");
    sprintf(buffer, "AT+CWMODE=3\r\n");
    uart_puts(UART_ID, buffer);
    sleep_ms(1000);

    printf("Connecting to WiFi...\n");
    sprintf(buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);
    uart_puts(UART_ID, buffer);
    sleep_ms(1000);

    // Clear UART buffer
    while (uart_is_readable(UART_ID)) {
        uart_getc(UART_ID);
    }

    while (true) {
        // PC → ESP8266
        int ch = getchar_timeout_us(0);
        if (ch != PICO_ERROR_TIMEOUT) {
            uart_putc(UART_ID, (char)ch);
        }

        // ESP8266 → PC
        if (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            putchar(c);
        }
    }
} */