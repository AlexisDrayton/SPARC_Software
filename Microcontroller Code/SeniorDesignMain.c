#include <asf.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <interrupt.h>
#include <samd21j18a.h>
#include <pio/samd21j18a.h>
#include <core_cm0plus.h>
#include "ASF\sam0\drivers\sercom\usart\usart.h"

#define IR_SIG PIN_PA02						// analog IR sensor
#define A_SPARE_1 PIN_PA03
#define PIXY_XCOORD PIN_PA04				// analog pixy cam
#define PIXY_SUBJECT_DETECTED PIN_PA23		// digital pixy cam
#define D_SPARE_2 PIN_PA22
#define CHRG_EN PIN_PA08
#define EN_12V PIN_PA09
#define SIG_313 PIN_PA10		//antenna
#define SIG_433 PIN_PA11		//antenna
//#define MCC_EN PIN_PB10			//Motor
//#define MOTOR_DIS PIN_PB11
#define MOVE_B2 PIN_PB12
#define MOVE_F2 PIN_PB13
//#define FSB2 PIN_PB14
#define MOVE_B1 PIN_PA12
#define MOVE_F1 PIN_PA13
//#define FSB1 PIN_PA13
//#define USARTTX1 PIN_PA16
//#define USARTRX1 PIN_PA17
//#define USARTRX2 PIN_PA18
//#define USARTTX2 PIN_PA19
//#define USBDM PIN_PA24
//#define USBDP PIN_PA25

void usart_read_callback(struct usart_module *const usart_module);
void usart_write_callback(struct usart_module *const usart_module);

void configure_usart(void);
void configure_usart_callbacks(void);

void moveForward(void);
void moveBackward(void);
void moveLeft(void);
void moveRight(void);
void fullStop(void);
void setSchedule(uint8_t*);
void runAutonomous(void);
void reversePath(void);

struct usart_module usart_instance;
#define MAX_RX_BUFFER_LENGTH  1
#define MAX_PATH_LENGTH 10
#define MAX_SCHEDULE_LENGTH 3

volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];
volatile uint8_t manual_buffer[MAX_RX_BUFFER_LENGTH];
volatile uint8_t path_buffer[MAX_PATH_LENGTH];
volatile uint8_t schedule_buffer[MAX_SCHEDULE_LENGTH];
volatile uint8_t flipped_path[MAX_PATH_LENGTH];

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
	int i;
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_HIGH);
	
	for(i=0; i< 2000000; i++){} // 2 mil = ~1 sec
	
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_LOW);
	
	for(i=0; i< 2000000; i++){}
}

void moveBackward(void){
	int i;
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_HIGH);
	
	for(i=0; i< 2000000; i++){} // 2 mil = ~1 sec
	
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_LOW);
	
	for(i=0; i< 2000000; i++){}
}

void moveRight(void){
	int i;
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_HIGH);
	
	for(i=0; i< 2000000; i++){} // 2 mil = ~1 sec
	
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_LOW);
	
	for(i=0; i< 2000000; i++){}
}

void moveLeft(void){
	int i;
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_HIGH);
	
	for(i=0; i< 2000000; i++){} // 2 mil = ~1 sec
	
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_LOW);
	
	for(i=0; i< 2000000; i++){}
}

void fullStop(void){
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_LOW);
}

time_t now, scheduledTime;
int scheduleSet = 0;

void setSchedule(uint8_t* schedule){
	
	// 	uint8_t scheduledDay = schedule[0];
	// 	uint8_t hour_tens = schedule[1];
	// 	uint8_t hour_ones = schedule[2];
	// 	uint8_t scheduledHour = (((hour_tens-48)*10)+(hour_ones-48));
	// 	time_t currentDay;
	// 	uint8_t currentDayConverted;
	//
	// 	now = time(NULL);
	// 	currentDayConverted = ((((now-14400)/86400) + 4) % 7);
	// 	currentDay = (now - (now % 86400));
	//
	// 	switch(scheduledDay)
	// 	{
	// 		case 77: // M_onday
	// 		{
	// 			if(currentDayConverted == 1)
	// 			{
	// 				currentDay = currentDay + 86400;
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 2)
	// 			{
	// 				currentDay = currentDay + (2*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 3)
	// 			{
	// 				currentDay = currentDay + (3*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 4)
	// 			{
	// 				currentDay = currentDay + (4*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 5)
	// 			{
	// 				currentDay = currentDay + (5*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 6)
	// 			{
	// 				currentDay = currentDay + (6*86400);
	// 				break;
	// 			}
	// 			else
	// 			{
	// 				break;
	// 			}
	// 		} // case M
	// 		case 84: // T_uesday
	// 		{
	// 			if(currentDayConverted == 0)
	// 			{
	// 				currentDay = currentDay + (6*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 2)
	// 			{
	// 				currentDay = currentDay + (1*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 3)
	// 			{
	// 				currentDay = currentDay + (2*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 4)
	// 			{
	// 				currentDay = currentDay + (3*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 5)
	// 			{
	// 				currentDay = currentDay + (4*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 6)
	// 			{
	// 				currentDay = currentDay + (5*86400);
	// 				break;
	// 			}
	// 			else
	// 			{
	// 				break;
	// 			}
	// 		} // case T
	// 		case 87: // W_ednesday
	// 		{
	// 			if(currentDayConverted == 0)
	// 			{
	// 				currentDay = currentDay + (5*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 1)
	// 			{
	// 				currentDay = currentDay + (6*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 3)
	// 			{
	// 				currentDay = currentDay + (1*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 4)
	// 			{
	// 				currentDay = currentDay + (2*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 5)
	// 			{
	// 				currentDay = currentDay + (3*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 6)
	// 			{
	// 				currentDay = currentDay + (4*86400);
	// 				break;
	// 			}
	// 			else
	// 			{
	// 				break;
	// 			}
	// 		} // case W
	// 		case 72: // t_H_ursday
	// 		{
	// 			if(currentDayConverted == 0)
	// 			{
	// 				currentDay = currentDay + (5*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 1)
	// 			{
	// 				currentDay = currentDay + (6*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 2)
	// 			{
	// 				currentDay = currentDay + (1*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 4)
	// 			{
	// 				currentDay = currentDay + (2*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 5)
	// 			{
	// 				currentDay = currentDay + (3*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 6)
	// 			{
	// 				currentDay = currentDay + (4*86400);
	// 				break;
	// 			}
	// 			else
	// 			{
	// 				break;
	// 			}
	// 		} // case H
	// 		case 70: // F_riday
	// 		{
	// 			if(currentDayConverted == 0)
	// 			{
	// 				currentDay = currentDay + (3*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 1)
	// 			{
	// 				currentDay = currentDay + (4*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 2)
	// 			{
	// 				currentDay = currentDay + (5*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 3)
	// 			{
	// 				currentDay = currentDay + (6*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 5)
	// 			{
	// 				currentDay = currentDay + (1*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 6)
	// 			{
	// 				currentDay = currentDay + (2*86400);
	// 				break;
	// 			}
	// 			else
	// 			{
	// 				break;
	// 			}
	// 		} // case F
	// 		case 83: // S_aturday
	// 		{
	// 			if(currentDayConverted == 0)
	// 			{
	// 				currentDay = currentDay + (2*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 1)
	// 			{
	// 				currentDay = currentDay + (3*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 2)
	// 			{
	// 				currentDay = currentDay + (4*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 3)
	// 			{
	// 				currentDay = currentDay + (5*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 4)
	// 			{
	// 				currentDay = currentDay + (6*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 6)
	// 			{
	// 				currentDay = currentDay + (1*86400);
	// 				break;
	// 			}
	// 			else
	// 			{
	// 				break;
	// 			}
	// 		} // case S
	// 		case 85: // s_U_nday
	// 		{
	// 			if(currentDayConverted == 0)
	// 			{
	// 				currentDay = currentDay + (1*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 1)
	// 			{
	// 				currentDay = currentDay + (2*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 2)
	// 			{
	// 				currentDay = currentDay + (3*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 3)
	// 			{
	// 				currentDay = currentDay + (4*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 4)
	// 			{
	// 				currentDay = currentDay + (5*86400);
	// 				break;
	// 			}
	// 			else if(currentDayConverted == 5)
	// 			{
	// 				currentDay = currentDay + (6*86400);
	// 				break;
	// 			}
	// 			else
	// 			{
	// 				break;
	// 			}
	// 		} // case U
	// 	} // scheduled day switch-case
	//
	// 	scheduledTime = currentDay + (scheduledHour * 3600);
	
	scheduleSet = 1;
	
} // set schedule function

static int pathSet = 0;
static int manualMode = 0;
static int scheduleMode = 0;

void reversePath(void){
	
	int i = 0, k = 0, f = 0;
	for(i = 9; i >= 0; i--){
		if((path_buffer[i] == 65) && (f == 0)){
			f = 1;
			i--;
		}
		if((path_buffer[i] != 0) && (f == 1)){
			if(path_buffer[i] == 67){
				flipped_path[k] = 68;
				k++;
				}else if(path_buffer[i] == 68){
				flipped_path[k] = 67;
				k++;
				}else if(path_buffer[i] == 65){
				flipped_path[k] = 65;
				k++;
				}else {
				//error case, this shouldn't happen
			}
		}
	} // for loop
}

void runAutonomous(void){
	//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(EN_12V, IOPORT_PIN_LEVEL_HIGH);
	int i = 0;
	moveBackward();
	moveLeft();
	moveLeft();
	while((flipped_path[i] != 0) && (i < 10)){
		if(flipped_path[i] == 65){
			moveForward();
			i++;
			}else if(flipped_path[i] == 67){
			moveLeft();
			i++;
			}else if(flipped_path[i] == 68){
			moveRight();
			i++;
		}
	}
	ioport_set_pin_level(EN_12V, IOPORT_PIN_LEVEL_LOW);
	//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
}

int main(void)
{
	system_init();
	
	//delay_init();
	
	configure_usart();
	configure_usart_callbacks();

	system_interrupt_enable_global();


	//uint8_t string[] = "Hello World!\r\n";
	//usart_write_buffer_wait(&usart_instance, string, sizeof(string));
	
	//ioport_set_pin_dir(MCC_EN, IOPORT_DIR_OUTPUT);			// MCC EN
	//ioport_set_pin_dir(MOTOR_DIS, IOPORT_DIR_OUTPUT);		// MOTOR DIS
	ioport_set_pin_dir(MOVE_B2, IOPORT_DIR_OUTPUT);			// MOVE B2
	ioport_set_pin_dir(MOVE_F2, IOPORT_DIR_OUTPUT);			// MOVE F2
	//ioport_set_pin_dir(FSB2, IOPORT_DIR_INPUT);				// FSB 2, INPUT
	ioport_set_pin_dir(MOVE_B1, IOPORT_DIR_OUTPUT);			// MOVE B1
	ioport_set_pin_dir(MOVE_F1, IOPORT_DIR_OUTPUT);			// MOVE F1
	//ioport_set_pin_dir(FSB1, IOPORT_DIR_INPUT);				// FSB 1, INPUT
	ioport_set_pin_dir(EN_12V, IOPORT_DIR_OUTPUT);			// 12V ENABLE
	
	//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);			// MCC EN
	//ioport_set_pin_level(MOTOR_DIS, IOPORT_PIN_LEVEL_LOW);		// MOTOR DIS
	ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_LOW);		// MOVE B2
	ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_LOW);		// MOVE F2
	//ioport_set_pin_level(FSB2, IOPORT_PIN_LEVEL_LOW);			// FSB 2 - INPUT
	ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);		// MOVE B1
	ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_LOW);		// MOVE F1
	//ioport_set_pin_level(FSB1, IOPORT_PIN_LEVEL_LOW);			// FSB 1 - INPUT
	ioport_set_pin_level(EN_12V, IOPORT_PIN_LEVEL_LOW);			// 12V ENABLE

	//ioport_set_pin_level(MOVE_B2, true);
	
	int k = 0;
	for(k = 0; k < 10; k++){
		path_buffer[k] = 0;
	}
	
	int j = 0;
	for(j = 0; j < 3; j++){
		schedule_buffer[j] = 0;
	}
	
	// 	scheduleSet = 0;

	// SEAN, TESTING CODE IS HERE
	
	int i;
	
	// --------------------------
	
	// -------------------------------------- RELAY TEST CYCLES ---------------------------------------------
		
		for(i=0; i<2000000; i++){}
		
		ioport_set_pin_level(MOVE_B2, true);
		
		for(i=0; i<2000000; i++){} // 2 mil = ~1 sec
		
		ioport_set_pin_level(MOVE_B2, false);
		
		for(i=0; i<2000000; i++){}
		
		ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
		
		for(i=0; i<2000000; i++){} // 2 mil = ~1 sec
		
		ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);
		
		for(i=0; i<2000000; i++){}
		
		ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
		
		for(i=0; i<2000000; i++){} // 2 mil = ~1 sec
		
		ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);
		
		for(i=0; i<2000000; i++){}
		
		ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
		
		for(i=0; i<2000000; i++){} // 2 mil = ~1 sec
		
		ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);
		
		for(i=0; i<2000000; i++){}
		
		ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
		
		for(i=0; i<2000000; i++){} // 2 mil = ~1 sec
		
		ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_LOW);
		
		
		
		// -------------------------------------- RELAY TEST CYCLES ------------------------------------------

	while(true){ // ------------------------- MAIN WHILE LOOP ----------------------------
		
		//ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_HIGH);
		//moveForward();
		
		if(scheduleSet == 1)
		{
			// add time comparison here
			runAutonomous();
		}
		
		
		
		rx_buffer[0] = 0;
		usart_read_buffer_job(&usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
		
		switch(rx_buffer[0]) // MODE SELECTION SWITCH-CASE
		{
			case 90: // Z - MAUNAL CONTROL ENABLE
			{
				manualMode = 1; // enable manual mode
				int p = 0;
				
				while (true) { // -------- MANUAL CONTROL LOOP ----------
					
					rx_buffer[0] = 0;
					usart_read_buffer_job(&usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
					
					//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
					ioport_set_pin_level(EN_12V, IOPORT_PIN_LEVEL_HIGH);
					
					
					switch (rx_buffer[0])
					{
						case 65: // A
						{
							if((manual_buffer[0] == 65) && (p > 0)){
								fullStop();
								p = 0;
								break;
								}else if(manual_buffer[0] == 66){
								fullStop();
								p = 0;
								}else if(manual_buffer[0] == 67){
								fullStop();
								p = 0;
								}else if(manual_buffer[0] == 68){
								fullStop();
								p = 0;
							}
							ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_HIGH);
							ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_HIGH);
							p++;
							break;
						}
						case 66: // B
						{
							if((manual_buffer[0] == 66) && (p > 0)){
								fullStop();
								p = 0;
								break;
								}else if(manual_buffer[0] == 67){
								fullStop();
								p = 0;
								}else if(manual_buffer[0] == 68){
								fullStop();
								p = 0;
								}else if(manual_buffer[0] == 65){
								fullStop();
								p = 0;
							}
							ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
							ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_HIGH);
							p++;
							break;
						}
						case 67: // C
						{
							if((manual_buffer[0] == 67) && (p > 0)){
								fullStop();
								p = 0;
								break;
								}else if(manual_buffer[0] == 68){
								fullStop();
								p = 0;
								}else if(manual_buffer[0] == 65){
								fullStop();
								p = 0;
								}else if(manual_buffer[0] == 66){
								fullStop();
								p = 0;
							}
							ioport_set_pin_level(MOVE_B1, IOPORT_PIN_LEVEL_HIGH);
							ioport_set_pin_level(MOVE_F2, IOPORT_PIN_LEVEL_HIGH);
							p++;
							break;
						}
						case 68: // D
						{
							if((manual_buffer[0] == 68) && (p > 0)){
								fullStop();
								p = 0;
								break;
								}else if(manual_buffer[0] == 67){
								fullStop();
								p = 0;
								}else if(manual_buffer[0] == 66){
								fullStop();
								p = 0;
								}else if(manual_buffer[0] == 65){
								fullStop();
								p = 0;
							}
							ioport_set_pin_level(MOVE_F1, IOPORT_PIN_LEVEL_HIGH);
							ioport_set_pin_level(MOVE_B2, IOPORT_PIN_LEVEL_HIGH);
							p++;
							break;
						}
						case 90: // Z
						{
							fullStop();
							p = 0;
							manualMode = 0; // disable manual mode
							break;
						}
						
					} // switch
					manual_buffer[0] = rx_buffer[0];
					
					if(manualMode == 0) // check if manual is still enabled
					{
						ioport_set_pin_level(EN_12V, IOPORT_PIN_LEVEL_LOW);
						//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
						break; // exit manual mode
					}
				} //while ------------ MANUAL CONTROL LOOP ---------------------
				break;
			} // case "Z"
			
			case 88: // X - SCHEDULING MODE ENABLE
			{
				scheduleMode = 1; // enable scheduling mode
				//scheduleSet = 0;
				
				usart_read_buffer_job(&usart_instance, (uint8_t *)schedule_buffer, MAX_SCHEDULE_LENGTH);
				//setSchedule((uint8_t*)schedule_buffer);
				
				for(k = 0; k < 10; k++){
					path_buffer[k] = 0;
				}
				
				i = 0;
				while(true) // -------- SCHEDULED PATH SETTING LOOP ----------------
				{
					rx_buffer[0] = 0;
					usart_read_buffer_job(&usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
					
					ioport_set_pin_level(EN_12V, IOPORT_PIN_LEVEL_HIGH);
					
					switch (rx_buffer[0])
					{
						case 65: // A
						{
							if(i < 10){
								path_buffer[i] = 65;
								//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
								moveForward();
								//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
								i++;
							}
							break;
						}
						case 66: // B
						{
							if(i < 10){
								path_buffer[i] = 66;
								//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
								moveBackward();
								//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
								i++;
							}
							break;
						}
						case 67: // C
						{
							if(i < 10){
								path_buffer[i] = 67;
								//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
								moveLeft();
								//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
								i++;
							}
							break;
						}
						case 68: // D
						{
							if(i < 10){
								path_buffer[i] = 68;
								//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_HIGH);
								moveRight();
								//ioport_set_pin_level(MCC_EN, IOPORT_PIN_LEVEL_LOW);
								i++;
							}
							break;
						}
						case 88: // X
						{
							usart_read_buffer_job(&usart_instance, (uint8_t *)schedule_buffer, MAX_SCHEDULE_LENGTH);
							scheduleMode = 0; // disable scheduling mode
							break;
						}
						
					} // switch
					
					if(scheduleMode == 0) // check if scheduling is still enabled
					{
						for(j = 0; j < 3; j++){
							schedule_buffer[j] = 0;
						}
						
						ioport_set_pin_level(EN_12V, IOPORT_PIN_LEVEL_LOW);

						scheduleMode = 0;
						pathSet = 1; // path should be set and recorded at this point
						break; // exit scheduling mode
					}
				} // while ------------ SCHEDULED PATH SETTING LOOP ----------------
			} // case "X"
			
			default:
			scheduleSet = 0;
			break;
		} // outer switch
		
		
		
	} // --------------------------------- MAIN WHILE LOOP -----------------------------
	
} // main


