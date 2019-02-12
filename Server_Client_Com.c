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



const char key[] = { 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
			   0x31, 0x31, 0x31, 0x31, 0x31, 0x31 };


static void communication_thread(void *arg) {

	sc_conn_t *conn;
	err_t err;

	LWIP_UNUSED_ARG(arg);
	PRINTF("-> SSS Starting Client \r\n");

	while(1){


		conn=sc_init_client_comm(key);
		LWIP_ERROR("communication_thread: invalid conn", (conn != NULL), return;);
		IP4_ADDR(&server_address, serverIP_ADDR0, serverIP_ADDR1,serverIP_ADDR2, serverIP_ADDR3);
		PRINTF("Client Connection attempt\r\n");
		err=sc_connect_client_comm(conn, &server_address, 50002);

		/* Process the new connection. */
		if (err == ERR_OK) {
			uint8_t data[16] = "Este Es uNa prui";
			u16_t len;
			PRINTF("Client Connected! %p\r\n", conn);

			len = strlen((const char*)data);

			/*char* encrypt_data_pointer;
			encrypt_data_pointer= encrypt_ecb((void*)data);
			unsigned long crc_int_form;
			crc_int_form=crc32b((void*)encrypt_data_pointer);
			char* crc_pointer;
			crc_pointer=iTos(crc_int_form,0);
			char new_buf[26];

			unsigned int counter_cat=0;

			while(16>counter_cat){
				new_buf[counter_cat]=*encrypt_data_pointer;
				encrypt_data_pointer++;
				counter_cat++;
			}


			while(27>counter_cat){
				new_buf[counter_cat]=*crc_pointer;
				crc_pointer++;
				counter_cat++;

			}*/
			//err=netconn_write(conn->newconn,new_buf, 26, NETCONN_COPY);

			err=sc_send(conn, data, 26);

			if(err == ERR_OK) {
							PRINTF("sending...");

						}


		 vTaskDelay(3000);

	}

}

}

err_t sc_send(sc_conn_t* conn, uint8_t *dataptr, size_t size)
{


				char* encrypt_data_pointer;
				encrypt_data_pointer= encrypt_ecb((void*)dataptr);
				unsigned long crc_int_form;
				crc_int_form=crc32b((void*)encrypt_data_pointer);
				char* crc_pointer;
				crc_pointer=iTos(crc_int_form,0);
				char new_buf[26];

				unsigned int counter_cat=0;

				while(16>counter_cat){
					new_buf[counter_cat]=*encrypt_data_pointer;
					encrypt_data_pointer++;
					counter_cat++;
				}


				while(27>counter_cat){
					new_buf[counter_cat]=*crc_pointer;
					crc_pointer++;
					counter_cat++;

				}



	return netconn_write(conn->newconn,dataptr, 26, NETCONN_COPY);
}



err_t sc_connect_client_comm(sc_conn_t* conn,  const ip_addr_t *addr, u16_t port){
	conn->newconn = conn->conn;
	return netconn_connect(conn->conn, addr, port);
}


sc_conn_t* sc_init_client_comm(uint8_t key){
		sc_conn_t* conn = NULL;
		conn = pvPortMalloc(sizeof(sc_conn_t));

		if(NULL != conn) {
			conn->conn = netconn_new(NETCONN_TCP);
			conn->key = key;
		}

		return conn;
}




static uint8_t* decrypt_ecb(uint8_t* in) {
	uint8_t key[] = { 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
			0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31 };

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

void communication_init(){

	sys_thread_new("app_communication_thread", communication_thread, NULL,DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);



}
