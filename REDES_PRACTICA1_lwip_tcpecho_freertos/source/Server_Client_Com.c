/*
 * Server_Client_Com.c
 *
 *  Created on: 11/02/2019
 *      Author: David
 */
#include "Server_Client_Com.h"
#include "tcp_app.h"
#include "aes.h"
#include "lwip/opt.h"
#include "Crc32.h"
#include <stdint.h>
#include "lwip/sys.h"
#include "lwip/api.h"
#include <string.h>
#include <fsl_debug_console.h>
#include <fsl_clock.h>
#include <fsl_gpio.h>
#include <fsl_port.h>
#include "board.h"
#include <stdint.h>

#define SERVER_APP (0)
#define CLIENT_APP (1)

#if (CLIENT_APP)
#define serverIP_ADDR0 192
#define serverIP_ADDR1 168
#define serverIP_ADDR2 0
#define serverIP_ADDR3 101
ip4_addr_t server_address;
#endif

char data_buffer[256] = { 0 };
char buff_data_crc[10] = { 0 };
const uint8_t recv_ok[16] = "El mensaje -OK--";
const uint8_t recv_not_ok[16] = "El mensaje NOTOK";

const char key[] = { 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
		0x31, 0x31, 0x31, 0x31, 0x31, 0x31 };

static void communication_thread(void *arg) {

	sc_conn_t *conn;
	err_t err;

	LWIP_UNUSED_ARG(arg);

#if (CLIENT_APP)

	while(1) {

		PRINTF("-> SSS Starting Client \r\n");
		conn=sc_init_client_comm(key);
		LWIP_ERROR("communication_thread: invalid conn", (conn != NULL), return;);
		IP4_ADDR(&server_address, serverIP_ADDR0, serverIP_ADDR1,serverIP_ADDR2, serverIP_ADDR3);
		PRINTF("Client Connection attempt\r\n");
		err=sc_connect_client_comm(conn, &server_address, 50002);

		/* Process the new connection. */
		if (err == ERR_OK) {
			uint8_t data[16] = "Este Es uNa prui";
			PRINTF("Client Connected! %p\r\n", conn);
			err=sc_send(conn, data, 26);
			if(err == ERR_OK) {
				PRINTF("sending...");
			}

			vTaskDelay(3000);

		}

	}
#endif

#if (SERVER_APP)

	conn = sc_init_server_comm(IP_ADDR_ANY, 50000, key);

	while (1) {

		/* Grab new connection. */
		err = sc_connect_server_comm(conn);

		/* Process the new connection. */
		if (err == ERR_OK) {
			uint16_t len;
			PRINTF("Server Connected! %p\r\n", conn);

			while ((err = sc_receive(conn, data_buffer, 256)) == ERR_OK) {
				PRINTF("Received: ");
				PRINTF("%s\r\n", data_buffer);

				buff_data_crc[0] = data_buffer[16] - 48;
				buff_data_crc[1] = data_buffer[17] - 48;
				buff_data_crc[2] = data_buffer[18] - 48;
				buff_data_crc[3] = data_buffer[19] - 48;
				buff_data_crc[4] = data_buffer[20] - 48;
				buff_data_crc[5] = data_buffer[21] - 48;
				buff_data_crc[6] = data_buffer[22] - 48;
				buff_data_crc[7] = data_buffer[23] - 48;
				buff_data_crc[8] = data_buffer[24] - 48;
				buff_data_crc[9] = data_buffer[25] - 48;

				if (0 == take_value_crc32(data_buffer, buff_data_crc, 1)) {
					printf("Paquete correcto\n");
					PRINTF("%s\r\n", decrypt_ecb(get_data_without_crc()));
					err = sc_send(conn, recv_ok, 26);
					if (err == ERR_OK) {
						PRINTF("sending...");
					}

				} else {
					printf("Paquete incorrecto\n");
					err = sc_send(conn, recv_not_ok, 26);
				}

			}
			PRINTF("Connection closed\n");

			vTaskDelay(3000);

		}

	}

#endif

}

err_t sc_receive(sc_conn_t* conn, uint8_t *dataptr, size_t max_size) {
	struct netbuf *buf;
	err_t err;
	void *data;
	u16_t len;
	uint8_t chksum = 0;

	err = netconn_recv(conn->newconn, &buf);
	if (err == ERR_OK) {
		netbuf_data(buf, &data, &len);
		if (len > max_size)
			len = max_size;
		memcpy(dataptr, data, len);
		netbuf_delete(buf);

	}

	return err;

}

err_t sc_connect_server_comm(sc_conn_t* conn) {
	return netconn_accept(conn->conn, &(conn->newconn));
}

sc_conn_t* sc_init_server_comm(const ip_addr_t *addr, u16_t port, uint8_t key) {
	sc_conn_t* conn = NULL;

	conn = pvPortMalloc(sizeof(sc_conn_t));

	if (NULL != conn) {
		conn->conn = netconn_new(NETCONN_TCP);
		netconn_bind(conn->conn, addr, port);
		netconn_listen(conn->conn);
		conn->key = key;
	}

	return conn;
}

err_t sc_send(sc_conn_t* conn, uint8_t *dataptr, size_t size) {

	char* encrypt_data_pointer;
	encrypt_data_pointer = encrypt_ecb((void*) dataptr);
	unsigned long crc_int_form;
	crc_int_form = crc32b((void*) encrypt_data_pointer);
	char* crc_pointer;
	crc_pointer = iTos(crc_int_form, 0);
	char new_buf[26];

	unsigned int counter_cat = 0;

	while (16 > counter_cat) {
		new_buf[counter_cat] = *encrypt_data_pointer;
		encrypt_data_pointer++;
		counter_cat++;
	}

	while (27 > counter_cat) {
		new_buf[counter_cat] = *crc_pointer;
		crc_pointer++;
		counter_cat++;

	}

	return netconn_write(conn->newconn, new_buf, 26, NETCONN_COPY);
}

err_t sc_connect_client_comm(sc_conn_t* conn, const ip_addr_t *addr, u16_t port) {
	conn->newconn = conn->conn;
	return netconn_connect(conn->conn, addr, port);
}

sc_conn_t* sc_init_client_comm(uint8_t key) {
	sc_conn_t* conn = NULL;
	conn = pvPortMalloc(sizeof(sc_conn_t));

	if (NULL != conn) {
		conn->conn = netconn_new(NETCONN_TCP);
		conn->key = key;
	}

	return conn;
}


static uint8_t* decrypt_ecb(uint8_t* in) {

	static uint8_t buffer[25];

	AES_ECB_decrypt(in, key, buffer, 25);

	return buffer;
}

static char* encrypt_ecb(char* in) {
	char key[] = { 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
			0x31, 0x31, 0x31, 0x31, 0x31, 0x31 };
	static signed char a[16];

	char buffer1[16];

	AES_ECB_encrypt(in, key, buffer1, 16);

	for (int i = 0; i < 16; i++) {
		if (buffer1[i] > 127) {
			a[i] = buffer1[i] - 256;
		} else {
			a[i] = buffer1[i];
		}
	}

	return a;

}

void communication_init() {

	sys_thread_new("app_communication_thread", communication_thread, NULL,
	DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);

}
