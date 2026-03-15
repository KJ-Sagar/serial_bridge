#include <stdio.h>
#include "pico/stdlib.h"
#include "pico_espat_udp_utils.h"

#define WIFI_SSID "D108-4G"
#define WIFI_PASSWORD "9533112237"

#define TCP_SERVER_PORT 20030

int main() {
    stdio_init_all();
    
    sleep_ms(10000);

    if(espat_init()) {
        printf("WiFi module initialized\n");
    }
    else {
        printf("Failed to initalize WiFi Module\n");
    }

    if(connect_wifi(WIFI_SSID, WIFI_PASSWORD)) {
        printf("Connected to '%s'\n", WIFI_SSID);
    }
    else {
        printf("Unable to connect to WiFi\n");
    }

    if(start_tcp_server(20030)) {
        printf("Started TCP server at port '%d'\n", TCP_SERVER_PORT);
    }
    else {
        printf("Unable to start TCP server\n");
    }

    printf("Entering bridge mode\n");
    while (true) {
        // USB to UART bridging
        int ch = getchar_timeout_us(0);
        if (ch != PICO_ERROR_TIMEOUT) {
            uart_putc(UART_ID, (char)ch);
        }
    }

    if(stop_tcp_server()) {
        printf("Stopped the TCP server\n");
    }
    else {
        printf("Unable to stop the TCP server\n");
    }

    if(disconnect_wifi(WIFI_SSID, WIFI_PASSWORD)) {
        printf("Disconnected from '%s'\n", WIFI_SSID);
    }
    else {
        printf("Unable to disconnect from WiFi\n");
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