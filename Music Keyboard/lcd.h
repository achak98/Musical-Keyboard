#include <LPC17xx.H>

void init_LCD(void);
void delay(unsigned int x);
void init_LCD(void);
void write_Command(char data);
void write_Data (char data);
void command(char data);


#define LCD_RS 22
#define LCD_EN 23


