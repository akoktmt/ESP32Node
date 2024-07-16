// tcp_server.c
#include "tcp_server.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <esp_log.h>

static const char *TAG = "TCP_SERVER";

esp_err_t tcp_socket_init(tcp_server_t *server, uint16_t port)
{
    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_socket < 0)
    {
        ESP_LOGE(TAG, "Failed to create socket");
        return ESP_FAIL;
    }

    server->server_addr.sin_family = AF_INET;
    server->server_addr.sin_addr.s_addr = INADDR_ANY;
    server->server_addr.sin_port = htons(port);

    if (bind(server->server_socket, (struct sockaddr *)&server->server_addr, sizeof(server->server_addr)) < 0)
    {
        ESP_LOGE(TAG, "Socket bind failed");
        close(server->server_socket);
        return ESP_FAIL;
    }

    if (listen(server->server_socket, 1) < 0)
    {
        ESP_LOGE(TAG, "Socket listen failed");
        close(server->server_socket);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Server listening on port %d", port);
    return ESP_OK;
}

esp_err_t tcp_socket_accept(tcp_server_t *server)
{
    socklen_t addr_len = sizeof(server->client_addr);
    server->client_socket = accept(server->server_socket, (struct sockaddr *)&server->client_addr, &addr_len);
    if (server->client_socket < 0)
    {
        ESP_LOGE(TAG, "Failed to accept client connection");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Client connected");
    return ESP_OK;
}

int tcp_socket_send(tcp_server_t *server, const void *data, size_t len)
{
    int bytes_sent = send(server->client_socket, data, len, 0);
    if (bytes_sent < 0)
    {
        ESP_LOGE(TAG, "Failed to send data");
    }
    return bytes_sent;
}

int tcp_socket_receive(tcp_server_t *server, void *buffer, size_t len)
{
    int bytes_received = recv(server->client_socket, buffer, len, 0);
    if (bytes_received < 0)
    {
        ESP_LOGE(TAG, "Failed to receive data");
    }
    return bytes_received;
}

void tcp_socket_close(tcp_server_t *server)
{
    if (server->client_socket >= 0)
    {
        close(server->client_socket);
        server->client_socket = -1;
        ESP_LOGI(TAG, "Client connection closed");
    }
}

void tcp_socket_deinit(tcp_server_t *server)
{
    if (server->server_socket >= 0)
    {
        close(server->server_socket);
        server->server_socket = -1;
        ESP_LOGI(TAG, "Server deinitialized");
    }
}
