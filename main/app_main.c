#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "mqtt_connect.h"
//#include "wifi_connect.h"
#include "wifi.h"
#include "twai_connect.h"
#include "ws_server.h"
#include "runStats.h"
#include "socket_tasks.h"
#include "sys_config.h"

#include "ina_219_handler.h"

static const char *TAG = "main";

esp_mqtt_client_config_t mqtt_cfg_t = {
    .broker.address.hostname = MQTT_ADDRESS,
    .broker.address.port = MQTT_PORT,
    .session.last_will.topic = DISCONNECT_TOPIC_PUB,
    .session.last_will.msg = "Esp32",
    .session.last_will.msg_len = 0,
    .session.last_will.qos = 1,
    .session.last_will.retain = 0};
// .host = MQTT_ADDRESS,
// .port = MQTT_PORT,
// .lwt_topic = DISCONNECT_TOPIC_PUB,
// .lwt_msg = "Esp32",
// .lwt_msg_len = 0,
// .lwt_qos = 1,
// .lwt_retain = 0};
MQTT_Handler_Struct mqtt_h =
    {
        .mqtt_cfg = &mqtt_cfg_t,
};
Twai_Handler_Struct twai_h =
    {
        .f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL(),
        .t_config = TWAI_TIMING_CONFIG_500KBITS(),
        .g_config = {.mode = TWAI_MODE_NORMAL,
                     .tx_io = TX_GPIO_NUM,
                     .rx_io = RX_GPIO_NUM,
                     .clkout_io = TWAI_IO_UNUSED,
                     .bus_off_io = TWAI_IO_UNUSED,
                     .tx_queue_len = 30, // old: 10
                     .rx_queue_len = 30, // old: 10
                     .alerts_enabled = TWAI_ALERT_ALL,
                     .clkout_divider = 0,
                     .intr_flags = ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LEVEL3}};

#ifdef INA_219
INA_Handler_Struct ina_handler;
#endif
typedef enum
{
    START = 0,
    WIFI_CONNECTED,
    TCP_SERVER_CREATED,
    WEB_SERVER_CREATED
} running_state_t;
void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    // gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    // gpio_pad_select_gpio(BUTTON_PIN_1);
    gpio_set_direction(BUTTON_PIN_1, GPIO_MODE_INPUT);
    // gpio_pad_select_gpio(BUTTON_PIN_2);
    gpio_set_direction(BUTTON_PIN_2, GPIO_MODE_INPUT);
    // Wifi start
   // wifi_init();
    static httpd_handle_t server = NULL;
    // Mqtt start
    // mqtt_init_start(&mqtt_h);
    // Twai start
    twai_install_start(&twai_h);
    //task
   // xTaskCreate(twai_receive_task, "TWAI_rx", 1024*5, &mqtt_h, RX_TASK_PRIO, NULL );
    //xTaskCreate(twai_transmit_msg_Steering, "TWAI_tx", 1024*5, &twai_h, TX_TASK_PRIO, NULL);
    // error = socket_tasks_init();
    // if (error == ESP_OK)
    // {
    //     ESP_LOGI("Main", "TCP server started!");
    // }
    // else
    // {
    //     ESP_LOGE("Main", "TCP server is fail!");
    // }

    // while (running_state == TCP_SERVER_CREATED)
    // {
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    // xTaskCreate(mqtt_receive_task, "MQTT_tx", 4096, NULL, TX_TASK_PRIO, NULL);
 static running_state_t running_state = START; // Set the desired running status here
    // Initialize NVS (Non-Volatile Storage)
    esp_err_t error = nvs_flash_init();
    if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        error = nvs_flash_init();
    }
    ESP_ERROR_CHECK(error);

    // Initialize Wi-Fi
    error = wifi_init();

    if (error == ESP_OK)
    {
        running_state = WIFI_CONNECTED;
    }
    else
    {
        ESP_LOGE("Main", "Wifi is initialized fail!");
    }
    server = start_webserver();
    // Start the first socket tasks and TCP server
    error = socket_tasks_init();

    if (error == ESP_OK)
    {
        ESP_LOGI("Main", "TCP server started!");
    }
    else
    {
        ESP_LOGE("Main", "TCP server is fail!");
    }

    while (running_state == TCP_SERVER_CREATED)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
#ifdef INA_219
    xTaskCreatePinnedToCore(read_power_task, "INA_tx", 4096, NULL, TX_TASK_PRIO, NULL, tskNO_AFFINITY);
#endif
    // xTaskCreatePinnedToCore(stats_task, "RunStats", 4096, NULL, 5, NULL, tskNO_AFFINITY);
}
