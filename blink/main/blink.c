#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "gpio_driver.h"

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
        vtaskDelay(1000);
        gpio_set_level(BLINK_LED, 0);
        vtaskDelay(1000);
    }
}
