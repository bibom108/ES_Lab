#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define BTN_GPIO 13

void print_mssv_task (void *pvParameter) {
    while (1) {
        printf("1914691\n");
        vTaskDelay (1000 / portTICK_RATE_MS);
    }
}

void button_task(void *pvParameter)
{
    gpio_pad_select_gpio(BTN_GPIO);

    gpio_set_direction(BTN_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_GPIO, GPIO_PULLDOWN_ONLY);
    int cnt = 10;
    int btn = 2;
    while(1) {
        if (cnt == 0) {
            if (gpio_get_level(BTN_GPIO) == 0) {
                btn -= 1;
                if (btn == 0) {
                    printf("ESP32\n");
                    btn = 2;
                }
            }
            cnt = 10;
        }
        cnt--;
        vTaskDelay (10 / portTICK_RATE_MS);
    }
}

void app_main(void)
{
    xTaskCreate(&print_mssv_task, "print_mssv_task", 2048, NULL, 0, NULL);
    xTaskCreate(&button_task, "button_task", 2048, NULL, 1, NULL);
}
