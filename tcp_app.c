/*
 * Based on the LwIP TCP echo demo app.
 * Works on the K64 Board.
 * 
 * Author: Sergio Santana <sergiosantana@iteso.mx>
 *
 */
#include "tcp_app.h"
#include "aes.h"
#include "lwip/opt.h"
//#include "lwip_tcpecho_freertos."
#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

#include <string.h>
#include <fsl_debug_console.h>
#include <fsl_clock.h>
#include <fsl_gpio.h>
#include <fsl_port.h>
#include "board.h"

#define TCP_SERVER_APP (1)
#define TCP_CLIENT_APP (0)

#if (TCP_CLIENT_APP)
#define serverIP_ADDR0 192
#define serverIP_ADDR1 168
#define serverIP_ADDR2 0
#define serverIP_ADDR3 100

ip4_addr_t server_address;
#endif

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_PORT BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW3_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_NAME BOARD_SW3_NAME

char tcp_app_data_buffer[256] = {0};
/* Whether the SW button is pressed */
volatile bool g_ButtonPress = false;
/*-----------------------------------------------------------------------------------*/

static uint8_t* decrypt_ecb(uint8_t* in)
{
	uint8_t key[] = { 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31};


  static uint8_t buffer[16];

  AES_ECB_decrypt(in, key, buffer, 16);

  return buffer;
}

static char* encrypt_ecb(char* in)
{
	char key[] = { 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31};
	static signed char a[16];

   char buffer1[16];



  AES_ECB_encrypt(in, key, buffer1, 16);

  for(int i = 0; i<16;i++){
	  if(buffer1[i] > 127){
  	 a[i] = buffer1[i]-256;
	  }else{
		  a[i] = buffer1[i];
	  }
   }

  return a;

}


static void 
tcp_app_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err;
  LWIP_UNUSED_ARG(arg);

  while (1) {
    /* Create a new connection identifier. */
    conn = netconn_new(NETCONN_TCP);
#if (TCP_SERVER_APP)
    /* Bind connection to well known port number 7. */
    netconn_bind(conn, IP_ADDR_ANY, 50000);
#endif
    LWIP_ERROR("tcp_app: invalid conn", (conn != NULL), return;);

#if (TCP_SERVER_APP)
    /* Tell connection to go into listening mode. */
    netconn_listen(conn);
#endif

#if (TCP_CLIENT_APP)
    /* Connect to a server */
  	IP4_ADDR(&server_address, serverIP_ADDR0, serverIP_ADDR1, serverIP_ADDR2, serverIP_ADDR3);
 		PRINTF("Client Connection attempt\r\n");
		err = netconn_connect(conn, &server_address, 50000);
  	newconn = conn;
#else /*SERVER MODE*/
    /* Grab new connection. */
    err = netconn_accept(conn, &newconn);
#endif
    /* Process the new connection. */
    if (err == ERR_OK) {
      PRINTF("Connected! %p\r\n", newconn);
    	struct netbuf *buf;
      uint8_t *data;
      u16_t len;
      
      while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
        PRINTF("Received: ");
        do {
             netbuf_data(buf, &data, &len);
             /*Print the received data*/
             memcpy(tcp_app_data_buffer, data, len);
             tcp_app_data_buffer[len] = 0;
             /*Code implementation by Hector Flores ie703475@iteso.mx*/


             /*------------------------------------------------------*/
             PRINTF("%s\r\n",  decrypt_ecb(tcp_app_data_buffer));
             /* Respond */
             if(g_ButtonPress) {
            	 data = (void*)"Event occurred!";
             }
             else {
            	 data = (void*)"No event";
             }
             g_ButtonPress = false;
             len = strlen((const char*)data);

             err = netconn_write(newconn, encrypt_ecb("Hola como  estas"), 16, NETCONN_COPY);

            if (err != ERR_OK) {
              PRINTF("tcp_app: netconn_write: error \"%s\"\n", lwip_strerr(err));
            }

        } while (netbuf_next(buf) >= 0);
        netbuf_delete(buf);
      }
      PRINTF("Connection closed\n");

      /* Close connection and discard connection identifier. */
      netconn_close(newconn);
      netconn_delete(newconn);

      vTaskDelay(1000);
    }
  }
}
/*-----------------------------------------------------------------------------------*/
void
tcp_app_init(void)
{
	gpio_pin_config_t pin_config;
	/*Init Pins*/
	/**
	 * Geen LED
	 * */
	CLOCK_EnableClock(kCLOCK_PortE);
	pin_config.pinDirection = kGPIO_DigitalOutput;
	pin_config.outputLogic = 1;
	GPIO_PinInit(GPIOE, 26, &pin_config);

	/**
	 * SW3
	 * */
	CLOCK_EnableClock(kCLOCK_PortA);
	const port_pin_config_t porta4_pin38_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_HighDriveStrength,                                 /* High drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, &porta4_pin38_config);  /* PORTA4 (pin 38) is configured as PTA4 */
  PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
  EnableIRQ(BOARD_SW_IRQ);
	pin_config.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(GPIOA, 4, &pin_config);

  sys_thread_new("tcp_app_thread", tcp_app_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/
/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function toggles the LED
 */
void BOARD_SW_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
    /* Change state of button. */
    g_ButtonPress = true;
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}


#endif /* LWIP_NETCONN */
