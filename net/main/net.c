#include <stdio.h>
#include "wifi_connect.h"
#include "esp_log_level.h"
#include "esp_log.h"

void app_main(void)
{
	wifi_connect("CO", "raj", "12345678");
	while(wifi_status != 1010)
	{
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	ESP_LOGI("connected", "wifi_status: %d", wifi_status);
	while(true)
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
