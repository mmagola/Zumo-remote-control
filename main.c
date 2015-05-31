/**
 * @file		main.c
 * @brief		Example of use of bluetooth library. Testing with Putty terminal. 
 */
#include "MKL46Z4.h"
#include "bluetooth.h"
#include "skmj_sLCD.h"
#include "motorDriver.h"
#include "leds.h"

#include <string.h>


char tab[BUFF_SIZE];			// If your array is big, define it as global.
	
volatile uint8_t dzielnik_buz = 0;
volatile uint8_t buzzer = 0;
volatile uint8_t buzzer_enable = 0;
	
void buzzer_init(void){
	
	//pta12 - buzzer - zworka 328P
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; 
  PORTA->PCR[12] = PORT_PCR_MUX(1);
	PTA->PDDR |= (1u<<12);
	PTA->PCOR |= (1u<<12);
}

void WRC_start(void){
	
	uint32_t x;
	uint32_t t;
	
	for(x=0; x<500; x++){
		PTA->PTOR |= (1u<<12);
		for(t=0; t<10000; t++){}
	}
	PTA->PCOR |= (1u<<12);
	for(x=0; x<500; x++){
		for(t=0; t<10000; t++){}
	}
	
	for(x=0; x<500; x++){
		PTA->PTOR |= (1u<<12);
		for(t=0; t<10000; t++){}
	}
	PTA->PCOR |= (1u<<12);
	for(x=0; x<500; x++){
		for(t=0; t<10000; t++){}
	}
	
	for(x=0; x<500; x++){
		PTA->PTOR |= (1u<<12);
		for(t=0; t<10000; t++){}
	}
	PTA->PCOR |= (1u<<12);
	for(x=0; x<500; x++){
		for(t=0; t<10000; t++){}
	}
	
	for(x=0; x<1200; x++){
		PTA->PTOR |= (1u<<12);
		for(t=0; t<5000; t++){}
	}
	PTA->PCOR |= (1u<<12);
}

	
uint32_t horner_get_val(const char * str){
	
	uint32_t ile = 0;
	uint16_t i = 1;
	
	while( str[i] != '.' ){
		ile = 10*(ile)+(str[i]-'0');
		i++;
	}
	return ile;
}	
	


void SysTick_Handler(void) {
	
	if( buzzer_enable ){
		
		dzielnik_buz++;
		
		if( dzielnik_buz > buzzer ){
			PTA->PTOR |= (1u<<12);
			dzielnik_buz = 0;
		}
	}
}	
	
	
int main(void){

	char tab[20];
	char c;
	
	
	ledsInitialize();
	sLCD_Init();											// Initialize LCD
	sLCD_setByType(8888,'d');					// Test all segments
	bt_init( BAUD_RATE );							// Initialize Bluetooth/UART module
	motorDriverInit();

	buzzer_init();
	//WRC_start();
	bt_sendChar( 'a');
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 10000);		// Interrupt every 100ms
	
		
	while(1){													// Main loop
		
		bt_getStr( tab );
		c = *tab;
		
		if(c != '\0' ) bt_sendStr( tab );
		
		switch(c){
			
			case 'q':
				driveStop();
				buzzer_enable = 0;
				break;
			
			case 'w':
				driveForward( horner_get_val(tab) );
				break;
			
			case 's':
				driveReverse( horner_get_val(tab) );
				break;
			
			case 'a':
				driveReverseLeftTrack( horner_get_val(tab) );
				driveForwardRightTrack( horner_get_val(tab) );
				break;
			
			case 'd':
				driveForwardLeftTrack( horner_get_val(tab) );
				driveReverseRightTrack( horner_get_val(tab) );
				break;
			
			case 'b':
				buzzer = horner_get_val(tab)/10;
				buzzer_enable = 1;
				break;
			
			default:
				break;		
		}
	}
}
