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