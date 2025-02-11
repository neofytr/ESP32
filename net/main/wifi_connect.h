#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <stdint.h>
#include "esp_wifi.h"
#include <string.h>
#include "nvs_flash.h"

int retry_num = 0;
int wifi_status = 1000;

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
	switch(event_id)
	{
	case WIFI_EVENT_STA_START:
		{
			wifi_status = 1001;
			break;
		}
	case WIFI_EVENT_STA_CONNECTED:
		{
			wifi_status = 1002;
			break;
		}
	case WIFI_EVENT_STA_DISCONNECTED:
		{
			if (retry_num < 5)
			{
				esp_wifi_connect();
				retry_num++;
				wifi_status = 1001;
			}
			break;
		}
	case IP_EVENT_STA_GOT_IP:
		{
			wifi_status = 1010;
			break;
		}
	}
}

void wifi_connect(char *country, char *ssid, char *password)
{
	nvs_flash_init();
	esp_netif_init();
	esp_event_loop_create_default();
	esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
	esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
	wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&wifi_config);
	esp_netif_create_default_wifi_sta();
	wifi_config_t staconf = {
		.sta = {
			.threshold.authmode = WIFI_AUTH_WPA_PSK
		}
	};

	strcpy((char *)staconf.sta.ssid, ssid);
	strcpy((char *)staconf.sta.password, password);
	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_set_config(WIFI_IF_STA, &staconf);
	esp_wifi_set_country_code(country, false);
	esp_wifi_start();
	esp_wifi_connect();
}

#endif