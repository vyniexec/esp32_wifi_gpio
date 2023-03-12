/* == Incluindo todas as bibliotecas necessarias == */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"
#include <esp_http_server.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "driver/gpio.h"
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>

/* == Definindo os pinos do registrador de dados para acionar o Relé == */
#define  CLK 12
#define NIVEL 27
#define RESGIS 14

/* == Criando a API JSON == */
static const char *ON_RESP = "{\"status\": \"on\"}";
static const char *OFF_RESP = "{\"status\": \"off\"}";

static const char *TAG = "espressif"; /* == Nome da tag para depuração == */

/* == Conectando a REDE == */
#define NAME_WIFI CONFIG_ESP_WIFI_SSID
#define PASSWORD_WIFI CONFIG_ESP_WIFI_PASSWORD
#define MAXIMUM_RETRY CONFIG_ESP_MAXIMUM_RETRY

/* == Sinalizar se a conexão foi estabelecida ou não == */
static EventGroupHandle_t s_wifi_event_group;

/* == Definindo WIFI CONNECTED como BIT0 == */
#define WIFI_CONNECTED_BIT BIT0 

/* == Definindo WIFI FAIL como BIT1 == */
#define WIFI_FAIL_BIT BIT1

static int s_retry_num = 0;

/* == Iiniciando a função para o ESP se conectar com o WIFI definido em Kconfig.projbuild == */
static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "Tente se conectar novamente!");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "A conexão ao WIFI falhou!");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}
void connect_wifi(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = NAME_WIFI,
            .password = PASSWORD_WIFI,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finalizado!!!");

    /* == Aguardando até que conexão seja estabelecida (WIFI_CONNECTED_BIT) ou
    falhe pelo numero de tentativas(WIFI_FAIL_BIT). O numero de tentativas são definidas acima por event_handler() == */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    /* == Testa qual evento realmente ocorreu pelo "bits" == */
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "Conectado ao SSID:%s password:%s",
                 NAME_WIFI, PASSWORD_WIFI);
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "A conexão ao SSID:%s, password:%s falhou!!!",
                 NAME_WIFI, PASSWORD_WIFI);
    }
    else
    {
        ESP_LOGE(TAG, "Evento inesperado!!!");
    }
    vEventGroupDelete(s_wifi_event_group);
}

/* == Manipulador HTTP GET led2on == */
esp_err_t led2on_get_handler(httpd_req_t *req)
{
    gpio_set_level(NIVEL, 1);
    vTaskDelay(100);
    gpio_set_level(CLK, 1);
    vTaskDelay(100);
    gpio_set_level(NIVEL, 0);
    gpio_set_level(CLK, 0);
    gpio_set_level(RESGIS, 1);
    vTaskDelay(1000);
    gpio_set_level(RESGIS, 0);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, ON_RESP, strlen(ON_RESP));
    return ESP_OK;
}
/* == Manipulador HTTP GET led2off == */
esp_err_t led2off_get_handler(httpd_req_t *req)
{
    gpio_set_level(NIVEL, 0);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, OFF_RESP, strlen(OFF_RESP));
    return ESP_OK;
}

/* == Configuração do evento LED ON no HTTP GET == */
httpd_uri_t led2on = {
    .uri = "/led2on",
    .method = HTTP_GET,
    .handler = led2on_get_handler,
    .user_ctx = NULL};

/* == Configuração do evento LED OFF no HTTP GET == */
httpd_uri_t led2off = {
    .uri = "/led2off",
    .method = HTTP_GET,
    .handler = led2off_get_handler,
    .user_ctx = NULL};

/* == Função para inicar o WEB Server HTTP == */
httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* == Inicia o servidor HTTP == */
    if (httpd_start(&server, &config) == ESP_OK)
    {
        /* == Registra as URI responsaveis pelo estado da função led2on ou led2off == */
        httpd_register_uri_handler(server, &led2on);
        httpd_register_uri_handler(server, &led2off);
    }

    return server;
}

void app_main()
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