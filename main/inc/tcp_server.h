// tcp_server.h
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <esp_err.h>
#include <sys/socket.h>

// Structure to hold TCP server information
typedef struct
{
    int server_socket;
    int client_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
} tcp_server_t;

/**
 * @brief Initialize the TCP server
 *
 * @param server Pointer to the tcp_server_t structure
 * @param port Port number to listen on
 * @return ESP_OK on success, or an appropriate error code on failure
 */
esp_err_t tcp_socket_init(tcp_server_t *server, uint16_t port);

/**
 * @brief Accept a client connection
 *
 * @param server Pointer to the tcp_server_t structure
 * @return ESP_OK on success, or an appropriate error code on failure
 */
esp_err_t tcp_socket_accept(tcp_server_t *server);

/**
 * @brief Send data to the client
 *
 * @param server Pointer to the tcp_server_t structure
 * @param data Pointer to the data to send
 * @param len Length of the data to send
 * @return Number of bytes sent, or -1 on failure
 */
int tcp_socket_send(tcp_server_t *server, const void *data, size_t len);

/**
 * @brief Receive data from the client
 *
 * @param server Pointer to the tcp_server_t structure
 * @param buffer Pointer to the buffer to receive data
 * @param len Length of the buffer
 * @return Number of bytes received, or -1 on failure
 */
int tcp_socket_receive(tcp_server_t *server, void *buffer, size_t len);

/**
 * @brief Close the client connection
 *
 * @param server Pointer to the tcp_server_t structure
 */
void tcp_socket_close(tcp_server_t *server);

/**
 * @brief Deinitialize the TCP server
 *
 * @param server Pointer to the tcp_server_t structure
 */
void tcp_socket_deinit(tcp_server_t *server);

#endif // TCP_SERVER_H
