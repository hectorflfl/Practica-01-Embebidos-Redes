/*
 * Server_Client_Com.h
 *
 *  Created on: 11/02/2019
 *      Author: David
 */

#ifndef SERVER_CLIENT_COM_H_
#define SERVER_CLIENT_COM_H_


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

typedef struct {
	uint8_t key;
	struct netconn* conn;
	struct netconn* newconn;
}sc_conn_t;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	Esta función se encarga de recivir y alamacenar los datos en un buffer seleccionado
 	 \param[in] sc_conn_t* conn, uint8_t *dataptr, size_t max_size
 	 \return err_t
 	 \Too
 */
err_t sc_receive(sc_conn_t* conn, uint8_t *dataptr, size_t max_size);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	Esta función realiza la conexion  del servidor seguro con el cliente
 	 \param[in] sc_conn_t* conn
 	 \return err_t
 	 \Too
 */
err_t sc_connect_server_comm(sc_conn_t* conn);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	Esta función realiza la inizialización del servidor en modo seguro
 	 \param[in] const ip_addr_t *addr, u16_t port, uint8_t key
 	 \return sc_conn_t*
 	 \Too
 */
sc_conn_t* sc_init_server_comm(const ip_addr_t *addr, u16_t port, uint8_t key);



/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	Transmite el buffer de datos de servidor a cliente o de cliente a servidor de manera segura
 	 \param[in] sc_conn_t* conn, uint8_t *dataptr, size_t size
 	 \return err_t
 	 \Too
 */
err_t sc_send(sc_conn_t* conn, uint8_t *dataptr, size_t size);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	Esta función realiza la conexion  del cliente seguro con el servidor
 	 \param[in] sc_conn_t* conn,  const ip_addr_t *addr, u16_t port
 	 \return err_t
 	 \Too
 */
err_t sc_connect_client_comm(sc_conn_t* conn,  const ip_addr_t *addr, u16_t port);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	Esta función realiza la inizialización del cliente en modo seguro
 	 \param[in] uint8_t key
 	 \return sc_conn_t*
 	 \Too
 */
sc_conn_t* sc_init_client_comm(uint8_t key);




static char* encrypt_ecb(char* in);
static unsigned char* decrypt_ecb(unsigned char* in);
void communication_init();

#endif /* SERVER_CLIENT_COM_H_ */
