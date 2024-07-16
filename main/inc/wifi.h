// wifi.h

#ifndef __WIFI_H__
#define __WIFI_H__

#include "esp_err.h"

/**
 * @brief Initialize the wifi connection
 *
 * @param None
 * @return ESP_OK on success, or an error code on failure
 */
esp_err_t wifi_init(void);
#endif /*__WIFI_H__*/
