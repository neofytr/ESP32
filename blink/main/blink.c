#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "driver/gpio.h"

#define BLINK_LED (2)

void app_main(void)
{
    char *our_task_name = pcTaskGetTaskName(NULL);
    ESP_LOGI(our_task_name, "hello, starting up!\n");

    gpio_reset_pin(BLINK_LED);
    gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(BLINK_LED, 1);
        // divide by portTICK_PERIOD_MS to convert to milliseconds
        // vTaskDelay delays for a number of system ticks (systick interrupts), not milliseconds
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_LED, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
