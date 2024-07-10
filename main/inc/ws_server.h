#ifndef MAIN_WS_APP_H_
#define MAIN_WS_APP_H_
#include <esp_http_server.h>

httpd_handle_t start_webserver(void);
float bytes2Float(uint8_t bytes[4]);
void float2Bytes( uint8_t bytes_temp[4],float float_variable);
#endif
