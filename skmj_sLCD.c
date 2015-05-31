#include "MKL46Z4.h"
#include "skmj_sLCD.h"

// Zmienne tworzace dwie tablice
const uint8_t LCD_Front_Pin[] = {LCD_FRONT0, LCD_FRONT1, LCD_FRONT2, LCD_FRONT3, LCD_FRONT4, LCD_FRONT5, LCD_FRONT6, LCD_FRONT7};
//const static uint8_t LCD_Back_Pin[] = {LCD_BACK0, LCD_BACK1, LCD_BACK2, LCD_BACK3};

const uint8_t LCD_Digit_2[] = {					( LCD_S_D | LCD_S_E | LCD_S_F ),						//0
																				( LCD_C ),																	//1
																				( LCD_S_G | LCD_S_E | LCD_S_D ),						//2
																				( LCD_S_D | LCD_S_G ),											//3
																				( LCD_S_G | LCD_S_F ),											//4
																				( LCD_S_D | LCD_S_G | LCD_S_F ),						//5
																				( LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G ), 	//6
																				( LCD_C ),																	//7
																				( LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G ),	//8
																				( LCD_S_D | LCD_S_F | LCD_S_G ),						//9
																				( LCD_S_E | LCD_S_F | LCD_S_G ),						//A
																				( LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G ),	//b
																				( LCD_S_D | LCD_S_E | LCD_S_F ),						//C
																				( LCD_S_D | LCD_S_E | LCD_S_G ),						//d
																				( LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G ),	//E
																				( LCD_S_E | LCD_S_F | LCD_S_G ),						//F
																				( LCD_S_E | LCD_S_G ),											//r
																				( LCD_S_F | LCD_S_G ) };										//'

			
																				
const uint8_t LCD_Digit_1[] = {					( LCD_S_A | LCD_S_B | LCD_S_C ),	//0
																				( LCD_S_B | LCD_S_C ),						//1
																				( LCD_S_A | LCD_S_B ),						//2
																				( LCD_S_A | LCD_S_B | LCD_S_C ),	//3
																				( LCD_S_B | LCD_S_C ),						//4
																				( LCD_S_A | LCD_S_C ),						//5
																				( LCD_S_A | LCD_S_C ),						//6
																				( LCD_S_A | LCD_S_B | LCD_S_C ),	//7
																				( LCD_S_A | LCD_S_B | LCD_S_C ),	//8
																				( LCD_S_A | LCD_S_B | LCD_S_C ),	//9
																				( LCD_S_A | LCD_S_B | LCD_S_C ),	//A
																				( LCD_S_C ),											//b
																				( LCD_S_A ),											//C
																				( LCD_S_B | LCD_S_C ),						//d
																				( LCD_S_A ),											//E
																				( LCD_S_A ),											//F
																				( LCD_C ),												//r
																				( LCD_S_A | LCD_S_B ) };					//'



void sLCD_Init(void){
// konfiguracja zegara dla portow wykorzystywanych przy obsludze wyswietlacza
// i modulu sLCD
SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_SLCD_MASK;
// wstepne wylaczenie i reset kontrolera
LCD->GCR |= LCD_GCR_PADSAFE_MASK; //  Wylaczenie pinow przedniego i tylnego
LCD->GCR &= ~LCD_GCR_LCDEN_MASK; // Clear LCDEN w trakcie konfiguracji

	// konfiguracja multiplekserow do operacji portow jako kontroler LCD
PORTD->PCR[0] = PORT_PCR_MUX(0u);
PORTE->PCR[4] = PORT_PCR_MUX(0u);
PORTB->PCR[23] = PORT_PCR_MUX(0u);
PORTB->PCR[22] = PORT_PCR_MUX(0u);
PORTC->PCR[17] = PORT_PCR_MUX(0u);
PORTB->PCR[21] = PORT_PCR_MUX(0u);
PORTB->PCR[7] = PORT_PCR_MUX(0u);
PORTB->PCR[8] = PORT_PCR_MUX(0u);
PORTE->PCR[5] = PORT_PCR_MUX(0u);
PORTC->PCR[18] = PORT_PCR_MUX(0u);
PORTB->PCR[10] = PORT_PCR_MUX(0u);
PORTB->PCR[11] = PORT_PCR_MUX(0u);
	
// konfiguracja rejestrow LCD
LCD->GCR = // GCR-General Controll Register
LCD_GCR_RVTRIM(0x00) |
LCD_GCR_CPSEL_MASK |
LCD_GCR_LADJ(0x03) |
LCD_GCR_VSUPPLY_MASK |
LCD_GCR_ALTDIV(0x00) |
LCD_GCR_SOURCE_MASK |
LCD_GCR_LCLK(0x01) |
LCD_GCR_DUTY(0x03);
// konfiguracja migania wyswietlacza
LCD->AR = LCD_AR_BRATE(0x03); // podzial LCDclock/2^12+3
// konfiguracja rejestru FDCR
LCD->FDCR = 0x00000000; // wylaczenie prescalera na szynie zegarowej, sprawdzenie bledu co 4 takty, wylaczenie sprawdzania bledu,
// aktywowanie 12 pinow do kontroli wyswietlaczem (dwa rejestry po 32 bity)
LCD->PEN[0] =
LCD_PEN_PEN( 1u<<8 ) | // LCD_P8
LCD_PEN_PEN( 1u<<7 ) |
LCD_PEN_PEN( 1u<<10 ) |
LCD_PEN_PEN( 1u<<11 ) |
LCD_PEN_PEN( 1u<<17 ) |
LCD_PEN_PEN( 1u<<18 ) |
LCD_PEN_PEN( 1u<<19 );
LCD->PEN[1] =
LCD_PEN_PEN(1u<<(37-32)) |
LCD_PEN_PEN(1u<<(38-32)) |
LCD_PEN_PEN(1u<<(40-32)) |
LCD_PEN_PEN(1u<<(52-32)) |
LCD_PEN_PEN(1u<<(53-32));
// skonfigurowanie 4 pinow plaszczyzny tylnej (dwa rejestry po 32 bity)
LCD->BPEN[0] =
LCD_BPEN_BPEN(1u<<18) |
LCD_BPEN_BPEN(1u<<19);
LCD->BPEN[1] =
LCD_BPEN_BPEN(1u<<(40-32)) |
LCD_BPEN_BPEN(1u<<(52-32));
// konfiguracja rejestrow przebiegow (Waveform register) 4 aktywne, reszta nie
// konfiguracja polega na rownomiernym rozlozeniu faz, w tym przypadku 4, na 8 bitach
// (44.3.7 w KL46 Reference Manual)
LCD->WF[0] =
LCD_WF_WF0(0x00) |
LCD_WF_WF1(0x00) |
LCD_WF_WF2(0x00) |
LCD_WF_WF3(0x00);
LCD->WF[1] =
LCD_WF_WF4(0x00) |
LCD_WF_WF5(0x00) |
LCD_WF_WF6(0x00) |
LCD_WF_WF7(0x00);
LCD->WF[2] =
LCD_WF_WF8(0x00) |
LCD_WF_WF9(0x00) |
LCD_WF_WF10(0x00) |
LCD_WF_WF11(0x00);
LCD->WF[3] =
LCD_WF_WF12(0x00) |
LCD_WF_WF13(0x00) |
LCD_WF_WF14(0x00) |
LCD_WF_WF15(0x00);
LCD->WF[4] =
LCD_WF_WF16(0x00) |
LCD_WF_WF17(0x00) |
LCD_WF_WF18(0x88) | // COM3 (10001000)
LCD_WF_WF19(0x44); // COM2 (01000100)
LCD->WF[5] =
LCD_WF_WF20(0x00) |
LCD_WF_WF21(0x00) |
LCD_WF_WF22(0x00) |
LCD_WF_WF23(0x00);
LCD->WF[6] =
LCD_WF_WF24(0x00) |
LCD_WF_WF25(0x00) |
LCD_WF_WF26(0x00) |
LCD_WF_WF27(0x00);
LCD->WF[7] =
LCD_WF_WF28(0x00) |
LCD_WF_WF29(0x00) |
LCD_WF_WF30(0x00) |
LCD_WF_WF31(0x00);
LCD->WF[8] =
LCD_WF_WF32(0x00) |
LCD_WF_WF33(0x00) |
LCD_WF_WF34(0x00) |
LCD_WF_WF35(0x00);
LCD->WF[9] =
LCD_WF_WF36(0x00) |
LCD_WF_WF37(0x00) |
LCD_WF_WF38(0x00) |
LCD_WF_WF39(0x00);
LCD->WF[10] =
LCD_WF_WF40(0x11) | // COM0 (00010001)
LCD_WF_WF41(0x00) |
LCD_WF_WF42(0x00) |
LCD_WF_WF43(0x00);
LCD->WF[11] =
LCD_WF_WF44(0x00) |
LCD_WF_WF45(0x00) |
LCD_WF_WF46(0x00) |
LCD_WF_WF47(0x00);
LCD->WF[12] =
LCD_WF_WF48(0x00) |
LCD_WF_WF49(0x00) |
LCD_WF_WF50(0x00) |
LCD_WF_WF51(0x00);
LCD->WF[13] =
LCD_WF_WF52(0x22) | // COM1 (00100010)
LCD_WF_WF53(0x00) |
LCD_WF_WF54(0x00) |
LCD_WF_WF55(0x00);
LCD->WF[14] =
LCD_WF_WF56(0x00) |
LCD_WF_WF57(0x00) |
LCD_WF_WF58(0x00) |
LCD_WF_WF59(0x00);
LCD->WF[15] =
LCD_WF_WF60(0x00) |
LCD_WF_WF61(0x00) |
LCD_WF_WF62(0x00) |
LCD_WF_WF63(0x00);
// koniec konfiguracji, wiec clear PADSAFE i wlaczenie wyswietlacza
LCD->GCR &= ~LCD_GCR_PADSAFE_MASK; // odblokowanie pinow wyswietlacza
LCD->GCR |= LCD_GCR_LCDEN_MASK; // wlaczenie wyswietlacza
} 





void sLCD_setDot(uint8_t digit){
	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] |= ( LCD_S_DEC );
}	

void sLCD_clrDot(uint8_t digit){
	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] &= ( ~LCD_S_DEC );
}

void sLCD_set(uint8_t value,uint8_t digit){
	if( (value < 16) && (value >= 0) && (digit < 5) && (digit > 0) ){
		LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = LCD_Digit_2[value] ;
		LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = LCD_Digit_1[value] | ( LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] & LCD_S_DEC);
	}
	else{
		sLCD_error();
	}
}

void sLCD_setByType(uint32_t value, char type){	
// type (	'd' value (0...9999)	- decimal
//				'b' value (0...15)		- binary
//				'h' value (0...65535)	- hexadecimal
//				'o' value (0...4095)	)	- octal
	
	if( (type == 'd') && (value >= 0) && (value < 10000) ){
		
		// konwersja na system dziesietny i wyswietlanie
		sLCD_set(value/1000,1);
		sLCD_set((value/100)%10,2);
		sLCD_set((value/10)%10,3);
		sLCD_set((value%10),4);
	}
	else if( (type == 'b') && (value >=0) && (value < 16) ){
		uint8_t i=0;
		uint8_t x;
		
		// konwersja na system dwojkowy i wyswietlanie
		for( i=0; i<4; ++i){
			
			x = (value >> (3-i)) & 0x01;
			sLCD_set( x,i+1);
		}
	}
	else if(type == 'h'){
		uint8_t i=0;
		uint16_t x;
		
		// konwersja na system szesnastkowy i wyswietlanie
		for( i=0; i<4; ++i){
			
			x = (value >> (12-4*i)) & 0x0f;
			sLCD_set( x,i+1);
		}
	}
	else if( (type == 'o') && (value < 4096) && (value >= 0) ){
		uint8_t i=0;
		uint16_t x;
		
		// konwersja na system osemkowy i wyswietlanie
		for( i=0; i<4; ++i){
			
			x = (value >> (9-3*i)) & 0x07;
			sLCD_set( x,i+1);
		}
	}
	else{
		sLCD_error();
	}
}
	
void sLCD_error(void){
	
	LCD->WF8B[LCD_Front_Pin[((2*1)-2)]] = 0;
	LCD->WF8B[LCD_Front_Pin[((2*1)-1)]] = 0;
	
	// 'E'
	LCD->WF8B[LCD_Front_Pin[((2*2)-2)]] = LCD_Digit_2[14];
	LCD->WF8B[LCD_Front_Pin[((2*2)-1)]] = LCD_Digit_1[14];	
	
	// 'r'
	LCD->WF8B[LCD_Front_Pin[((2*3)-2)]] = LCD_Digit_2[16];
	LCD->WF8B[LCD_Front_Pin[((2*3)-1)]] = LCD_Digit_1[16];

	// 'r'
	LCD->WF8B[LCD_Front_Pin[((2*4)-2)]] = LCD_Digit_2[16];
	LCD->WF8B[LCD_Front_Pin[((2*4)-1)]] = LCD_Digit_1[16];
}


//void sLCD_set(uint8_t value,uint8_t digit){
//	
//// value wyswietlana wartosc,
//// digit pozycja na ktoej ma byc wyswietlona wartosc
//if(value==0x00){ // aby wyswietlic "0" zapalamy segmenty
//LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = // D, E, F oraz A, B, C
//(LCD_S_D | LCD_S_E |LCD_S_F); // (patrz tabelki w zadaniu 2.1)
//LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] =
//(LCD_S_A | LCD_S_B | LCD_S_C);
//}
//else if(value==0x01){
//LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] =
//(LCD_C);
//LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] =
//(LCD_S_B | LCD_S_C);
//}
//else if(value==0x02){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_G | LCD_S_E | LCD_S_D );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A | LCD_S_B );
//}
//else if(value==0x03){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_D | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A | LCD_S_B | LCD_S_C );
//}
//else if(value==0x04){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_G | LCD_S_F );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_B | LCD_S_C );
//}
//else if(value==0x05){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_D | LCD_S_G | LCD_S_F );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A | LCD_S_C );
//}
//else if(value==0x06){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A | LCD_S_C );
//}
//else if(value==0x07){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_C );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A | LCD_S_B | LCD_S_C );
//}
//else if(value==0x08){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A | LCD_S_B | LCD_S_C );
//}
//else if(value==0x09){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_D | LCD_S_F | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A | LCD_S_B | LCD_S_C );
//}
//else if(value==0x0A){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_E | LCD_S_F | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A | LCD_S_B | LCD_S_C );
//}
//else if(value==0x0B){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_C );
//}
//else if(value==0x0C){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A );
//}
//else if(value==0x0D){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_D | LCD_S_E | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_B | LCD_S_C );
//}
//else if(value==0x0E){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A );
//}
//else if(value==0x0F){
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = ( LCD_S_E | LCD_S_F | LCD_S_G );
//	LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = ( LCD_S_A );
//}
//}
