#include <LPC17xx.H>
#include "lcd.h"

/*------------------------------------------------------------------------------------------------------------------------
Description 	:	Delay 
Date			:	12 Nov 2017

-------------------------------------------------------------------------------------------------------------------------*/
void delay(unsigned int x)
{
  unsigned int i,j;
  for(i=0; i<100; i++)
  {
    for(j=0; j<x; j++);
  }
}


/*------------------------------------------------------------------------------------------------------------------------
Description 	:	Write command to lcd
Date			:	12 Nov 2017

-------------------------------------------------------------------------------------------------------------------------*/
void command(char cmd) {
				char temp,y;
				temp=((cmd)&0x0f);									/* Masking higher nibbles */
				y=((cmd&0xf0)>>4);									/* Masking lower nibbles */
	
				/*Sending MSB first */
				LPC_GPIO0->FIOPIN=(y<<27);							/*P0.27 onwards are data line i.e p0.27-P0.30 */
				LPC_GPIO1->FIOPIN &=~(1<<LCD_RS);					/* Enabling RS as command */
        		delay(300);
				LPC_GPIO1->FIOPIN &= ~(1<<LCD_EN);					/* High to low pulse */
	      		delay(300);
				LPC_GPIO1->FIOPIN |=(1<<LCD_EN);
				delay(300);
	
				/* Sending LSB */
				LPC_GPIO0->FIOPIN=(temp<<27);
				LPC_GPIO1->FIOPIN &=~(1<<LCD_RS);					/* Enabling RS as command */
	      		delay(300);
        		LPC_GPIO1->FIOPIN &= ~(1<<LCD_EN);					/* High to low pulse */
	      		delay(300);
				LPC_GPIO1->FIOPIN |=(1<<LCD_EN);	
      			delay(300);
}
/*------------------------------------------------------------------------------------------------------------------------
Description 	:	initialiazation of LCD
Date			:	12 Nov 2017

-------------------------------------------------------------------------------------------------------------------------*/


void init_LCD(void) {
        int count=0;
		char init[]={0x33,0x32,0x28,0x0E,0x01,0x06,0x80};
		LPC_PINCON->PINSEL3 &= ~0x0000F000;							/*	selecting port pin as GPIO */
		LPC_PINCON->PINMODE3 &= ~(3<<15);							/* 	pull up register clear */
		LPC_PINCON->PINMODE3 |= (1<<15);							/* Enable pull up register	  */
		LPC_GPIO0->FIODIR |= 0x78000000;		 					/* Configure P0.27-P0.30 pins as Outputs */
		LPC_GPIO1->FIODIR |= 0x00C00000;							/* Configure RS E as P1.22 and p1.23 output */
	
		LPC_GPIO1->FIOPIN |=(1<<LCD_EN);							/* Enable high */
		 delay(100);
		while(init[count]!=0)
		{
			command(init[count]);									/* sending Each command from init_array   */
			count++;												/* Count++ */
		}

}
/*------------------------------------------------------------------------------------------------------------------------
Description 	:	Write data to lcd
Date			:	12 Nov 2017

-------------------------------------------------------------------------------------------------------------------------*/

void write_Data (char data) {
		char temp,y;
		temp=(data&0x0f);								/* Masking higher nibbles */
		y=((data&0xf0)>>4);								/* Masking lower nibbles */
		
		/*Sending MSB first */
		LPC_GPIO0->FIOPIN=(y<<27);
		LPC_GPIO1->FIOPIN |=(1<<LCD_RS);				/* Enabling RS as Data */
		delay(300);
		LPC_GPIO1->FIOPIN &= ~(1<<LCD_EN);				/* High to low pulse */
		delay(300);
		LPC_GPIO1->FIOPIN |=(1<<LCD_EN);
		
		delay(500);
		
		/* Sending LSB */
		LPC_GPIO0->FIOPIN=(temp<<27);
		LPC_GPIO1->FIOPIN |=(1<<LCD_RS);				/* Enabling RS as Data */
		delay(300);
		LPC_GPIO1->FIOPIN &= ~(1<<LCD_EN);				/* High to low pulse */
		delay(300);
		LPC_GPIO1->FIOPIN |=(1<<LCD_EN);
		
		delay(500);

}

void write_Command(char data) {
        command (data);
        delay (4000);
}




