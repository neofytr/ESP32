#include <stdio.h>
#include "wifi_connect.h"
#include "esp_log_level.h"
#include "esp_log.h"

#define SERVER_IP "172.17.45.22"
#define SERVER_PORT (6969)

// networking headers; POSIX compliant

#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <netinet/in.h>

void connection_task(void)
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		ESP_LOGI("connection_task", "error socket: %s", strerror(errno));
		return;
	}

	fcntl(server_fd, F_SETFL, 0); // ensure blocking mode

	struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
	{
		ESP_LOGI("connection_task", "error inet_pton: %s", strerror(errno));
		close(server_fd);
		return;
	}

	if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		ESP_LOGI("connection_task", "error connect: %s", strerror(errno));
		close(server_fd);
		return;
	}

	#define RECV_BUF_LEN 1024
	char recv_buffer[RECV_BUF_LEN];
	int status = read(server_fd, recv_buffer, RECV_BUF_LEN - 1);
	#undef RECV_BUF_LEN

	if (status == -1)
	{
		ESP_LOGI("connection_task", "error read: %s", strerror(errno));
		close(server_fd);
	}
	else if (!status)
	{
		ESP_LOGI("connection_task", "Server closed the connection");
		close(server_fd);
		return;
	}
	else
	{
		recv_buffer[status] = 0;
		ESP_LOGI("connection_task", "Received from server: %s", recv_buffer);
	}

	status = write(server_fd, "hello server!", strlen("hello server!"));
	if (status == -1)
	{
		ESP_LOGI("connection_task", "error write: %s", strerror(errno));
	}

	// close the socket as well as the TCP connection
	close(server_fd);
}


void app_main(void)
{
	wifi_connect("CO", "raj", "12345678");
	while(wifi_status != 1010)
	{
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	ESP_LOGI("connected", "wifi_status: %d", wifi_status);

	// Sometimes the network stack needs a moment to fully initialize
	vTaskDelay(2000 / portTICK_PERIOD_MS);  // Add 2 second delay


	connection_task();
	while(true)
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
