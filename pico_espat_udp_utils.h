#ifndef PICO_ESPAT_UDP_UTILS_H
#define PICO_ESPAT_UDP_UTILS_H

    #include <stdio.h>
    #include <string.h>
    #include "pico/stdlib.h"
    #include "hardware/uart.h"

    #define UART_ID uart0
    #define UART_TX_PIN 0
    #define UART_RX_PIN 1
    #define BAUD_RATE 115200

    #define BUFFER_LENGTH 100
    char tx_buffer[BUFFER_LENGTH];


    void read_at_response() {
        int index = 0;
        while (uart_is_readable(UART_ID) && index < BUFFER_LENGTH - 1) {
            char c = uart_getc(UART_ID);
            if (c == '\r' || c == '\n') {
                continue;
            }
            tx_buffer[index++] = c;
        }
        tx_buffer[index] = '\0';
    }

    void clear_uart_buffer() {
        while (uart_is_readable(UART_ID)) {
            uart_getc(UART_ID);
        }
    }

    bool espat_init() {
        uart_init(UART_ID, BAUD_RATE);
        gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
        gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

        // Disable echo
        uart_puts(UART_ID, "ATE0\r\n");
        sleep_ms(100);
        clear_uart_buffer();

        // Send AT command to check if ESP-AT is responding
        uart_puts(UART_ID, "AT\r\n");
        sleep_ms(100);
        read_at_response();
        return strcmp(tx_buffer, "OK") == 0;
    }

    bool connect_to_wifi(char* wifi_ssid, char* wifi_password) {
        char buffer[100];

        uart_puts(UART_ID, "AT+CWMODE=1\r\n"); // Setup as station
        sprintf(buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", wifi_ssid, wifi_password); // Connect to WiFi
        uart_puts(UART_ID, buffer);
        sleep_ms(10000);

        
        // TODO: Verify the response to ensure Wi-Fi connection is successful
        int n_retries = 10;
        bool connected = false;
        char expected_result[100];
        for (int i=0; i<n_retries; i++) {
            read_at_response();
            if (strcmp(tx_buffer, "OK")) {
                connected = true;
                break;
            }
            else if (strcmp(tx_buffer, "ERROR")) {
                break;
            }
            sleep_ms(1000);
        }
        return connected;
    }

    void disconnect_wifi() {
        uart_puts(UART_ID, "AT+CWQAP\r\n");
        sleep_ms(1000); // Wait for disconnection

        // TODO: Verify the response to ensure Wi-Fi disconnection is successful
    }

    void setup_tcp_server() {
        // Send AT commands to set up UDP server
        uart_puts(UART_ID, "AT+CIPMUX=1\r\n"); // Enable multiple connections
        sleep_ms(1000);
        uart_puts(UART_ID, "AT+CIPSERVER=1,12345\r\n"); // Start UDP server on port 12345
        sleep_ms(1000);
    }

    void close_tcp_server() {
        uart_puts(UART_ID, "AT+CIPSERVER=0\r\n"); // Stop TCP server
        sleep_ms(1000);
    }

    void parse_ipd_message() {
        
    }

#endif //PICO_ESPAT_UDP_UTILS_H