// socket_tasks.h
#ifndef SOCKET_TASKS_H
#define SOCKET_TASKS_H

#include "tcp_server.h"
#include <esp_err.h>

/**
 * @brief Initialize the socket tasks
 *
 * @return ESP_OK if initialization is successful, or an appropriate error code on failure
 */
esp_err_t socket_tasks_init(void);

#endif // SOCKET_TASKS_H
