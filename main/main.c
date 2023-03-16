/* == Incluindo todas as bibliotecas necessarias == */
#include <stdio.h>
#include "includes.h"
#include "wifi/wifi.h"
#include "acionamento_rele/acio_rele.h"


void app_main(void)
{
    /* == Inicializando o NVS que armazena configurações do ESP32 == */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    connect_wifi();

    /* == Inicializando as GPIOs do ESP32 == */
    esp_rom_gpio_pad_select_gpio(CLK);
    gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(NIVEL);
    gpio_set_direction(NIVEL, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(RESGIS);
    gpio_set_direction(RESGIS, GPIO_MODE_OUTPUT);

    /* == Exibindo que o servidor foi inicializado com sucesso. == */
    ESP_LOGI(TAG, "Servidor iniciado com sucesso!!!\n");
    start_webserver();
}

/*
/*
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LCD_RS 18 // pino de controle RS do LCD
#define LCD_EN 5 // pino de controle EN do LCD
#define LCD_D4 23 // pino de dados D4 do LCD
#define LCD_D5 22 // pino de dados D5 do LCD
#define LCD_D6 21 // pino de dados D6 do LCD
#define LCD_D7 19 // pino de dados D7 do LCD

#define LCD_ROWS 4 // número de linhas do LCD
#define LCD_COLS 16 // número de colunas do LCD

static const char *TAG = "LCD";

static void lcd_write_nibble(uint8_t nibble) {
    gpio_set_level(LCD_D4, (nibble >> 0) & 0x01); // 0b0000 0001
    gpio_set_level(LCD_D5, (nibble >> 1) & 0x01);
    gpio_set_level(LCD_D6, (nibble >> 2) & 0x01);
    gpio_set_level(LCD_D7, (nibble >> 3) & 0x01);
    gpio_set_level(LCD_EN, 1);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(LCD_EN, 0);
    vTaskDelay(1 / portTICK_PERIOD_MS);
}

static void lcd_write_byte(uint8_t byte, uint8_t mode) {
    gpio_set_level(LCD_RS, mode);
    lcd_write_nibble(byte >> 4);
    lcd_write_nibble(byte);
}

static void lcd_clear() {
    lcd_write_byte(0x01, 0); // envia comando para limpar display
    vTaskDelay(2 / portTICK_PERIOD_MS); // espera 2ms para o comando ser executado
}

static void lcd_init() {
    gpio_pad_select_gpio(LCD_RS);
    gpio_pad_select_gpio(LCD_EN);
    gpio_pad_select_gpio(LCD_D4);
    gpio_pad_select_gpio(LCD_D5);
    gpio_pad_select_gpio(LCD_D6);
    gpio_pad_select_gpio(LCD_D7);

    gpio_set_direction(LCD_RS, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_EN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D4, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D5, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D6, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D7, GPIO_MODE_OUTPUT);

    vTaskDelay(50 / portTICK_PERIOD_MS);
    gpio_set_level(LCD_RS, 0);
    gpio_set_level(LCD_EN, 0);

    lcd_write_nibble(0x03);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    lcd_write_nibble(0x03);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    lcd_write_nibble(0x03);
    vTaskDelay(5 / portTICK_PERIOD_MS);

    lcd_write_nibble(0x02);
    vTaskDelay(1 / portTICK_PERIOD_MS);

    lcd_write_byte(0x28, 0); // função de 4 bits, 2 linhas, matriz 5x8
    lcd_write_byte(0x0c, 0); // display ligado, cursor desligado, piscar desligado
    lcd_write_byte(0x06, 0); // modo de entrada: incrementa endereço, sem deslocamento
    lcd_clear(); // limpa o display
    lcd_write_byte(0x02, 0); // retorna o cursor para a posição inicial
    vTaskDelay(2 / portTICK_PERIOD_MS);
}

static void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address;

    switch (row) {
        case 0:
            address = 0x00;
            break;
        case 1:
            address = 0x40;
            break;
        case 2:
            address = 0x10;
            break;
        case 3:
            address = 0x50;
            break;
        default:
            address = 0x00;
    }

    address += col;
    lcd_write_byte(0x80 | address, 0); // envia comando para posicionar cursor
}

static void lcd_write_string(const char *str) {
    while (*str) {
        lcd_write_byte(*str++, 1); // envia o caractere atual para o LCD
    }
}

void app_main() {
    ESP_LOGI(TAG, "Iniciando...");
    lcd_init();

    while (1) {
        // Escreve "Hello, world!" na primeira linha do LCD
        lcd_set_cursor(0, 0);
        lcd_write_string(TAG);

        // Espera 2 segundos
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        // Limpa o display
        lcd_clear();
    }
}

*/
*/
