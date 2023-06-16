#ifndef __INCLUDES__H__
#define __INCLUDES__H__

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

/* == Nome da tag para depuração == */
static const char *TAG = "@vyni1208"; 

/* == Criando a API JSON == */
static const char *ON_RESP = "{\"status\": \"on\"}";
static const char *OFF_RESP = "{\"status\": \"off\"}";

#endif  //!__INCLUDES__H__