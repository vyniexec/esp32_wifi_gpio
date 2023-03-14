#ifndef __ACIO_RELE__H__
#define __ACIO_RELE__H__
#include "../wifi/wifi.h"
#include "../includes.h"

esp_err_t led2on_get_handler(httpd_req_t *req);
esp_err_t led2off_get_handler(httpd_req_t *req);
httpd_handle_t start_webserver(void);

#endif  //!__ACIO_RELE__H__
