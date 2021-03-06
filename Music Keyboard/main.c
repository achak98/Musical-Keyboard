/********************************* Musical Keyboard on ARM Cortex M3 *******************************************
key 1:plays C
key 2:plays Db
key 3:plays D
key 4:plays Eb
key 5:plays E
key 6:plays F
key 7:plays Gb
key 8:plays G
key 9:plays Ab
key 10:plays A
key 11:plays Bb
key 12:plays B
key 13:decreases octave
key 14:increases octave
key 15:On/Off Button

Note: This plays octaves 2 to 5.

CONNECTIONS:

P2.0 is selected for PWM, i.e. connected to the Speaker

For Hexpad:

PC0-P0.23,PC1-P0.24,PC2-P0.25,PC3-P0.26	Act as input means columns
PA4-P0.19,PA5-P0.20,PA6-P0.21,PA7-P0.22 Act as Output	means Rows

For LCD:

data line - P0.27-P0.30
RS - P 1.22
EN - P 1.23
*********************************************************************************************************/

#include <math.h>
#include <LPC17xx.H>
#include "GLCD.H"
#include "Serial.h"
#include "lcd.h"

#define A4 440.0 //default value used to calculate other musical pitches

/* Function Prototypes */
void Display_Lcd(char data[]);
void delay_1(unsigned int i);
int keyscan(void);
void toggle(void);
double fToUms(double f);
void toggle(void);
void octaveInit(void);
void magic(void);
void toSpeaker(double f);

int status=0,octave=2,k;
char key,tone;
double C,Db,D,Eb,E,F,Gb,G,Ab,A,Bb,B,t;
char a[3];


/* Main starts here */
int main(){
	
	
	SystemInit();
	SystemCoreClockUpdate();
		
		
	/*Setting up the PWM*/
	LPC_PINCON->PINSEL4=(1<<0);
	LPC_PWM1->TCR=(1<<0)|(1<<3);
	LPC_PWM1->PR=0X00;
	LPC_PWM1->MCR=(1<<0);
	LPC_PWM1->MR0=100; //giving set starting pitch of 10
	LPC_PWM1->MR1=50;  //giving 50% duty cycle which is to be maintained all over for optimal volume and quality control in speaker
	delay_1(10000);
	LPC_PWM1->LER=(1<<0)|(1<<1);
	LPC_PWM1->PCR=(1<<9);
		
		
	/*Setting up the Hexpad*/
        LPC_SC->PCONP     |=  (1<<15);                                  /* POWER to GPIO */  
        LPC_PINCON->PINMODE1 &= ~0x003FC000;                             /* Pull up the register */ 
        LPC_PINCON->PINMODE1 |= (2<<14) | (2<<16)|(2<<18)|(2<<20);         /*pull down the register */ 
        LPC_GPIO0->FIODIR |= ((1<<19)|(1<<20)|(1<<21)|(1<<22));             /* PortC as OUTPUT */   
        LPC_GPIO0->FIODIR &= ~((1<<23)|(1<<24)|(1<<25)|(1<<26));         /* PortC as Input */   
        init_LCD();                                    /* Initialiazation of LCD */ 
        write_Command(0x0c);                        /* display on cursor off*/ 
        write_Command(0x80);                    /* Go to 1st line of LCD*/
				Display_Lcd("OFF");
				octaveInit();
        
        
        while(1){
			if(keyscan()==15)
			toggle();
			delay_1(1000);
			if(status==1){
		        magic();                            /* this is where the magic happens*/
						a[0]=key;
						a[1]=tone;
						a[2]=(octave+'0');
						delay_1(1000);
						write_Command(0x01);
						write_Command(0x80);                    /* Go to 1st line of LCD*/
						Display_Lcd("KEY PRESSED:");
						write_Command(0xc0);                    /* Move cursor to 2nd line of LCD*/
						Display_Lcd(a);
						delay_1(1000);
	        }
    
        }
}

//function to initialize octave
void octaveInit(void){
	A=A4*pow(2.0,(double)(octave-4));
	C=A*pow(2.0,-9.0/12.0);
	Db=A*pow(2.0,-8.0/12.0);
	D=A*pow(2.0,-7.0/12.0);
	Eb=A*pow(2.0,-6.0/12.0); 
	E=A*pow(2.0,-5.0/12.0);
	F=A*pow(2.0,-4.0/12.0);
	Gb=A*pow(2.0,-3.0/12.0);
	G=A*pow(2.0,-2.0/12.0);
	Ab=A*pow(2.0,-1.0/12.0);
	Bb=A*pow(2.0,1.0/12.0);
	B=A*pow(2.0,2.0/12.0);
}

//function to send pitch to speaker
void toSpeaker(double f){
	t=fToUms(f);
	LPC_PWM1->MR0=t;
	LPC_PWM1->MR1=t/2.0;
	delay_1(5000);
	write_Command(0x01); //clear
	write_Command(0x80);                    /* Go to 1st line of LCD*/
	Display_Lcd("KEY PRESSED:");
	write_Command(0xc0);                    /* Move cursor to 2nd line of LCD*/
	Display_Lcd("NONE");
	write_Command(0xc0);
	delay_1(5000);
	
}
//function to convert frequency to u seconds to set the MR values in ADC register
double fToUms(double f){
	return (1000.0/f);
}

//function that plays the music
void magic(void){
	k=keyscan();
	tone=' ';
	switch(k){
		case 1:
			key='C';
			delay_1(1000);
			toSpeaker(C);
			break;
		case 2:
			key='D';
			tone='b';
			toSpeaker(Db);
			break;
		case 3:
			key='D';
			toSpeaker(D);
			break;
		case 4:
			key='E';
			tone='b';
			toSpeaker(Eb);
			break;
		case 5:
			key='E';
			toSpeaker(E);
			break;
		case 6:
			key='F';
			toSpeaker(F);
			break;
		case 7:
			key='G';
			tone='b';
			toSpeaker(Gb);
			break;
		case 8:
			key='G';
			toSpeaker(G);
			break;
		case 9:
			key='A';
			tone='b';
			toSpeaker(Ab);
			break;
		case 10:
			key='A';
			toSpeaker(A);
			break;
		case 11:
			key='B';
			tone='b';
			toSpeaker(Bb);
			break;
		case 12:
			key='B';
			toSpeaker(B);
			break;
		case 13:
			if(octave>2)
				octave=octave-1;
				octaveInit();
				delay_1(1000);
			break;
		case 14:
			if(octave<5)
				octave=octave+1;
				octaveInit();
				delay_1(1000);
			break;
		default:
			break;
	}
	
}

//function to change the power status of the keyboard
void toggle(void){
	if(status==1){
		status=0;
		write_Command(0x01);                    /*Clear the screen*/
		write_Command(0x80);                    /* Go to 1st line of LCD*/
		Display_Lcd("OFF");
		delay_1(1000);
	}
	else{
		status=1;
		write_Command(0x01);                    /*Clear the screen*/
		write_Command(0x80);                    /* Go to 1st line of LCD*/
		Display_Lcd("KEY PRESSED:");
		write_Command(0xc0);                    /* Move cursor to 2nd line of LCD*/
		Display_Lcd("NONE");
		delay_1(1000);
	}
}

/* Delay Routine */ 
void delay_1(unsigned int i)     {
    unsigned int j,k;
    for(k=0;k<10;k++)
  for(j=0;j<i;j++);
}

/* Key Scan function */
int keyscan(void){
	unsigned int i,s,code,j;
	while(1){
		code = 0;
		for(i=0x00080000; i<=0x00400000; i<<=1){        /* Check for 4 Scan lines */
            LPC_GPIO0->FIOPIN = ((LPC_GPIO0->FIOPIN & 0xFF87FFFF) | i);    /* Make 1 scan line high  */
            delay_1(3200);    
            s= LPC_GPIO0->FIOPIN & 0x07800000;
            s = s >> 23;                            /* Since we are using P0.23 to P0.26 */ 
            if(s>0){                                /* Read the scanned line */
                for(j=0;j<4;j++){                    /* Check for 4 keys */ 
                  s>>=1;                                /* shifting data 1 bit right */
                  if(s==0)
                  return(code+j);                    /* If key Pressed return its code */ 
              	}
            }
            code += 4;
    	}
  	}
}

/* Function for displaying on lcd*/
void Display_Lcd(char data[])
{
    int i;
    for(i=0;i<data[i];i++)
    	write_Data(data[i]);
    
}

