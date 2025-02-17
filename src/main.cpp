#include "FreeRTOS.h"
#include "task.h"
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <cstdio>
#include <pico/cyw43_arch.h>


#define LED_PIN 15
#define DELAY 1000
#define WIFI_RETRY 3

[[noreturn]] void v_wifi(void*) {
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_UK)) {
        printf("failed to initialise\n");
    }
    cyw43_arch_enable_sta_mode();

    int retry_count = WIFI_RETRY;
    printf("Connecting to WiFi : %s...\n", WIFI_SSID);
    while (
            cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)
            && retry_count > 0
            ) {
        printf("failed to  connect retrying, %i of %i attempts left\n", retry_count, WIFI_RETRY);
        retry_count--;
    }
    if (retry_count > 0) {
        printf("Connected.\n");
    } else {
        printf("Connect failed after %i retry attempts\n", WIFI_RETRY);
        exit(-1);
    }
    while(true){
        vTaskDelay(DELAY);
        printf("Wifi Task\n");
    }

}

[[noreturn]] void v_blink(void*){
    printf("LED Task\n");
    gpio_init(LED_PIN);
    while (true) {
        printf("LED Task loop\n");
        gpio_set_dir(LED_PIN,GPIO_OUT);
        gpio_put(LED_PIN, true);
        vTaskDelay(DELAY);
        gpio_put(LED_PIN, false);
        vTaskDelay(DELAY);
    }
}

int main(){
    printf("booting\n");
    sleep_ms(5000);
    stdio_init_all();
    printf("create task\n");
    xTaskCreate(v_wifi,"WIFI",256, nullptr,5, nullptr);
    xTaskCreate(v_blink,"LED",128, nullptr,1, nullptr);
    printf("created task\n");
    vTaskStartScheduler();
    printf("scheduler started\n");
    return 0;
}

void vApplicationTickHook (void)
{
};

void vApplicationStackOverflowHook(TaskHandle_t Task, char *pcTaskName)
{
    panic("stack overflow (not the helpful kind) for %s\n", *pcTaskName);
}

void vApplicationMallocFailedHook(void)
{
    panic("Malloc Failed\n");
};