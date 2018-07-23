#include <asf.h>
#include <stdio.h>
#include <stdlib.h>
#include <interrupt.h>
#include <samd21j18a.h>
#include <pio/samd21j18a.h>
#include <core_cm0plus.h>
#include "ASF\sam0\drivers\sercom\usart\usart.h"

#define IR_SIG PIN_PA02
#define A_SPARE_1 PIN_PA03
#define A_SPARE_2 PIN_PA04
#define CHRG_EN PIN_PA08
#define EN_12V PIN_PA09
#define SIG_313 PIN_PA10 //antenna
#define SIG_433 PIN_PA11 //antenna
#define MCC_EN PIN_PB10 //Motor
#define MOTOR_DIS PIN_PB11
#define MOVE_B2 PIN_PB12
#define MOVE_F2 PIN_PB13
#define FSB2 PIN_PB14
#define MOVE_B1 PIN_PB15
#define MOVE_F1 PIN_PA12
#define FSB1 PIN_PA13
//#define USARTTX1 PIN_PA16
//#define USARTRX1 PIN_PA17
//#define USARTRX2 PIN_PA18
//#define USARTTX2 PIN_PA19
#define USBDM PIN_PA24
#define USBDP PIN_PA25


void usart_read_callback(struct usart_module *const usart_module);
void usart_write_callback(struct usart_module *const usart_module);

void configure_usart(void);
void configure_usart_callbacks(void);

void moveForward(void);
void moveBackward(void);
void moveLeft(void);
void moveRight(void);

struct usart_module usart_instance;
#define MAX_RX_BUFFER_LENGTH   1

volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];

void usart_read_callback(struct usart_module *const usart_module)
{
	usart_write_buffer_job(&usart_instance,
	(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
}

void usart_write_callback(struct usart_module *const usart_module)
{
	//port_pin_toggle_output_level(LED_0_PIN);
}

void configure_usart(void)
{
	struct usart_config config_usart;

	usart_get_config_defaults(&config_usart);

	config_usart.baudrate    = 9600;
	config_usart.mux_setting = USART_RX_1_TX_0_XCK_1;       //	USART_RX_1_TX_0_XCK_1 = (SERCOM_USART_CTRLA_RXPO(1) | SERCOM_USART_CTRLA_TXPO(0)),
	config_usart.pinmux_pad0 = PINMUX_PA16D_SERCOM3_PAD0;
	config_usart.pinmux_pad1 = PINMUX_PA17D_SERCOM3_PAD1;
	config_usart.pinmux_pad2 = PINMUX_UNUSED;				// I think these just need to be turned on.
	config_usart.pinmux_pad3 = PINMUX_UNUSED;				// <---------- Likely PA18D_SERCOM3_PAD2 and PA19D_SERCOM3_PAD3
	config_usart.generator_source = GCLK_GENERATOR_0;

	while (usart_init(&usart_instance,
	EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
	}

	usart_enable(&usart_instance);
}

void configure_usart_callbacks(void)
{
	usart_register_callback(&usart_instance,
	usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_instance,
	usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);

	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);

}
// Movement control functions -----------------------------------------------

void moveForward(void){
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_HIGH);
	delay_ms(250);
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_LOW);
}

void moveBackward(void){
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_HIGH);
	delay_ms(250);
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_LOW);
}

void moveRight(void){
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_HIGH);
	delay_ms(250);
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_LOW);
}

void moveLeft(void){
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_HIGH);
	delay_ms(250);
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_LOW);
}

int main(void)
{
	system_init();
	
	delay_init();
	
	configure_usart();
	configure_usart_callbacks();

	system_interrupt_enable_global();


	//uint8_t string[] = "Hello World!\r\n";
	//usart_write_buffer_wait(&usart_instance, string, sizeof(string));
	
	ioport_set_pin_dir(MCC_EN, IOPORT_DIR_OUTPUT);			// MCC EN
	ioport_set_pin_dir(MOTOR_DIS, IOPORT_DIR_OUTPUT);		// MOTOR DIS
	ioport_set_pin_dir(MOVE_B2, IOPORT_DIR_OUTPUT);			// MOVE B2
	ioport_set_pin_dir(MOVE_F2, IOPORT_DIR_OUTPUT);			// MOVE F2
	ioport_set_pin_dir(FSB2, IOPORT_DIR_INPUT);				// FSB 2, INPUT
	ioport_set_pin_dir(MOVE_B1, IOPORT_DIR_OUTPUT);			// MOVE B1
	ioport_set_pin_dir(MOVE_F1, IOPORT_DIR_OUTPUT);			// MOVE F1
	ioport_set_pin_dir(FSB1, IOPORT_DIR_INPUT);				// FSB 1, INPUT
	
	ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);			// MCC EN
	ioport_set_pin_level(MOTOR_DIS, IOPORT_PIN_LEVEL_LOW);		// MOTOR DIS
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_LOW);		// MOVE B2
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_LOW);		// MOVE F2
	//ioport_set_pin_level(FSB2, IOPORT_PIN_LEVEL_LOW);			// FSB 2 - INPUT
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);		// MOVE B1
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_LOW);		// MOVE F1
	//ioport_set_pin_level(FSB1, IOPORT_PIN_LEVEL_LOW);			// FSB 1 - INPUT

	while (true) {
		rx_buffer[0] = 0;
		usart_read_buffer_job(&usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
		
		//char input = rx_buffer[0];
		switch (rx_buffer[0])
		{
			case 65: // A
			{
				ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
				moveForward();
				ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
				//printf("Stuff");
				break;
			}
			case 66: // B
			{
				ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
				moveBackward();
				ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
				break;
			}
			case 67: // C
			{
				ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
				moveLeft();
				ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
				break;
			}
			case 68: // D
			{
				ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
				moveRight();
				ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
				break;
			}
		}//switch
		
	} //while
	
}//main







