#ifndef __WIFI__H__
#define __WIFI__H__
#include "../includes.h"

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void connect_wifi(void);

#endif  //!__WIFI__H__