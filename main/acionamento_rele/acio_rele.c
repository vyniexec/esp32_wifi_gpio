#include "acio_rele.h"
#include "../wifi/wifi.h"
#include "../includes.h"

/* == Manipulador HTTP GET led2on == */
esp_err_t led2on_get_handler(httpd_req_t *req)
{
    gpio_set_level(NIVEL, 1);
    vTaskDelay(10);
    gpio_set_level(CLK, 1);
    vTaskDelay(10);
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