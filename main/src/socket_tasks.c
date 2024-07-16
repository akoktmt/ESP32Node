// socket_tasks.c
#include "socket_tasks.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "json_utils.h"
//#include "EKF.h"
//#include "GPS_Measurement.h"
//#include "Heading_Measurement.h"
// extern EKF EKFexe;
// extern GPS GPSexe; 
// extern Heading Headingexe;
// extern double EncoderVel;
// extern double distance2lot;
// extern double heading_diff;
static const char *TAG = "SOCKET_TASKS";

// Task stack size and priority
#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY 5

// TCP socket instances
static tcp_server_t socket1;
static tcp_server_t socket2;

// Task handles
static TaskHandle_t socket1_task_handle = NULL;
static TaskHandle_t socket2_task_handle = NULL;

// Task function prototypes
void socket1_task(void *pvParameters);
void socket2_task(void *pvParameters);

esp_err_t socket_tasks_init(void)
{
    // Initialize the TCP sockets
    esp_err_t socket1_status = tcp_socket_init(&socket1, 3333);
    esp_err_t socket2_status = tcp_socket_init(&socket2, 4444);

    if (socket1_status != ESP_OK || socket2_status != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize one or both sockets");
        if (socket1_status == ESP_OK)
        {
            tcp_socket_deinit(&socket1);
        }
        if (socket2_status == ESP_OK)
        {
            tcp_socket_deinit(&socket2);
        }
        return ESP_FAIL;
    }

    // Create the tasks for both sockets
    if (xTaskCreate(&socket1_task, "socket1_task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &socket1_task_handle) != pdPASS ||
        xTaskCreate(&socket2_task, "socket2_task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &socket2_task_handle) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create one or both tasks");
        tcp_socket_deinit(&socket1);
        tcp_socket_deinit(&socket2);
        return ESP_FAIL;
    }

    return ESP_OK;
}

void socket1_task(void *pvParameters)
{
    char recv_buffer[128];
    int len;

    while (true)
    {
        if (tcp_socket_accept(&socket1) == ESP_OK)
        {
            ESP_LOGI(TAG, "Socket1 accepted a connection");

            // Receive data
            while ((len = tcp_socket_receive(&socket1, recv_buffer, sizeof(recv_buffer))) > 0)
            {
                recv_buffer[len] = 0; // Null-terminate the received data
                ESP_LOGI(TAG, "Socket1 received: %s", recv_buffer);

                // Check if request is GET_JSON_DATA
                if (strcmp(recv_buffer, "GET_FILTER_DATA") == 0)
                {
                     float speed = 0;
                     double distance = 0;
                     double latitude = 0;
                     double longitude = 0;
                     float heading = 0;
                     const char *json_data = prepare_json_data(speed, latitude, longitude, heading,distance);
                    if (json_data)
                    {
                        tcp_socket_send(&socket1, json_data, strlen(json_data));
                        free((void *)json_data); // Free the allocated memory for JSON string
                    }
                    else
                    {
                        ESP_LOGE(TAG, "Failed to create JSON data");
                    }
                }
                else
                {
                    ESP_LOGW(TAG, "Unknown request: %s", recv_buffer);
                }
            }

            tcp_socket_close(&socket1);
        }
    }

    tcp_socket_deinit(&socket1);
    vTaskDelete(NULL);
}

void socket2_task(void *pvParameters)
{
    char recv_buffer[128];
    int len;

    while (true)
    {
        if (tcp_socket_accept(&socket2) == ESP_OK)
        {
            ESP_LOGI(TAG, "Socket2 accepted a connection");

            // Receive data
            while ((len = tcp_socket_receive(&socket2, recv_buffer, sizeof(recv_buffer))) > 0)
            {
                recv_buffer[len] = 0; // Null-terminate the received data
                ESP_LOGI(TAG, "Socket2 received: %s", recv_buffer);

                // Check if request is GET_SENSOR_DATA
                if (strcmp(recv_buffer, "GET_GPSDRAW_DATA") == 0)
                {
                    float latitude = 0;
                    float longitude =0;
                    const char *json_data = prepare_sensor_data(latitude, longitude);
                    if (json_data)
                    {
                        tcp_socket_send(&socket2, json_data, strlen(json_data));
                        free((void *)json_data); 
                    }
                    else
                    {
                        ESP_LOGE(TAG, "Failed to create JSON data");
                    }
                }
                else
                {
                    ESP_LOGW(TAG, "Unknown request: %s", recv_buffer);
                }
            }

            tcp_socket_close(&socket2);
        }
    }
    tcp_socket_deinit(&socket2);
    vTaskDelete(NULL);
}
