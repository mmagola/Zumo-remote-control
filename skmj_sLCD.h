#ifndef SKMJ_SLCD_H_
#define SKMJ_SLCD_H_

// zdefiniowanie ilosci pinow na poszczegolnych plaszczyznach
#define LCD_N_FRONT 8
#define LCD_N_BACK 4
// Makra dla segmentow, kazdy znak sterowany jest przez dwa piny
// Pin 1 -> (Digit*2 - 1), Pin 2 -> Digit*2
// Pin 1 Pin 2
// COM0 D Dec
// COM1 E C
// COM2 G B
// COM3 F A
#define LCD_S_D 0x11 // segment D
#define LCD_S_E 0x22 // segment E
#define LCD_S_G 0x44 // segment G
#define LCD_S_F 0x88 // segment F
#define LCD_S_DEC 0x11
#define LCD_S_C 0x22
#define LCD_S_B 0x44
#define LCD_S_A 0x88
#define LCD_C 0x00 // clear

// Makra dla kazdego pinu
#define LCD_FRONT0 37u
#define LCD_FRONT1 17u
#define LCD_FRONT2 7u
#define LCD_FRONT3 8u
#define LCD_FRONT4 53u
#define LCD_FRONT5 38u
#define LCD_FRONT6 10u
#define LCD_FRONT7 11u

#define LCD_BACK0 40u
#define LCD_BACK1 52u
#define LCD_BACK2 19u
#define LCD_BACK3 18u

void sLCD_Init(void);
void sLCD_set(uint8_t value,uint8_t digit);	// value (0...15), digit (1...4)
void sLCD_setDot(uint8_t digit);						// digit (1...4)
void sLCD_clrDot(uint8_t digit);						// digit (1...4)
void sLCD_setByType(uint32_t value, char type);
// type (	'd' value (0...9999)	- decimal
//				'b' value (0...15)		- binary
//				'h' value (0...65535)	- hexadecimal
//				'o' value (0...4095)	)	- octal

void sLCD_error(void);											// shows 'Err'

#endif
