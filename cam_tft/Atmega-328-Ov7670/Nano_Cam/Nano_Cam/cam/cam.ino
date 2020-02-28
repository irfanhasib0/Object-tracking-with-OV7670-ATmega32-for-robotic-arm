#define F_CPU 16000000UL
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "ov7670.h"
/* Configuration: this lets you easily change between different resolutions
 * You must only uncomment one
 * no more no less*/
//#define useVga
#define useQvga
//#define useQqvga

static inline void serialWrB(uint8_t dat){
	while(!( UCSR0A & (1<<UDRE0)));//wait for byte to transmit
	UDR0=dat;
	while(!( UCSR0A & (1<<UDRE0)));//wait for byte to transmit
}
unsigned char serialRvB(void){
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}
static void StringPgm(const char * str){
	do{
		serialWrB(pgm_read_byte_near(str));
	}while(pgm_read_byte_near(++str));
}
static void captureImg_1(uint16_t wg,uint16_t hg){
	uint16_t lg2;
	uint8_t skip;
#ifdef useQvga
	uint8_t buf[640];
#elif defined(useQqvga)
	uint8_t buf[320];
#endif
	StringPgm(PSTR("RDY"));
	//Wait for vsync it is on pin 3 (counting from 0) portD
	while(!(PIND&8));//wait for high
	while((PIND&8));//wait for low
#ifdef useVga
	while(hg--){
		lg2=wg;
		while(lg2--){
			if(hg%10==0){
			while((PIND&4));//wait for low // Packet 1
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low  //Packet 2
			UDR0=(PINC&15)|(PIND&240);
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low //Packet 3
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low // Packet 4
			while(!(PIND&4));//wait for high
			
			while((PIND&4));//wait for low // Packet 5
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low // Packet 6
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low // Packet 7
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low // Packet 8
			while(!(PIND&4));//wait for high
			}
			else{
				while((PIND&4));//wait for low // Packet 1
				while(!(PIND&4));//wait for high
				while((PIND&4));//wait for low  //Packet 2
			//	UDR0=(PINC&15)|(PIND&240);
				while(!(PIND&4));//wait for high
				while((PIND&4));//wait for low //Packet 3
				while(!(PIND&4));//wait for high
				while((PIND&4));//wait for low // Packet 4
				while(!(PIND&4));//wait for high
				
				while((PIND&4));//wait for low // Packet 5
				while(!(PIND&4));//wait for high
				while((PIND&4));//wait for low // Packet 6
				while(!(PIND&4));//wait for high
				while((PIND&4));//wait for low // Packet 7
				while(!(PIND&4));//wait for high
				while((PIND&4));//wait for low // Packet 8
				while(!(PIND&4));//wait for high
			}
			//while (!(UCSR0A & (1 << UDRE0)));//wait for byte to transmit
			
			//serialWrB(hg);
		}
	}
#elif defined(useQvga)
	/*We send half of the line while reading then half later */
	while(hg--){
		uint8_t*b=buf,*b2=buf;
		lg2=wg/2;
		while(lg2--){
			while((PIND&4));//wait for low
			*b++=(PINC&15)|(PIND&240);
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low
			*b++=(PINC&15)|(PIND&240);
			UDR0=*b2++;
			while(!(PIND&4));//wait for high
		}
		/* Finish sending the remainder during blanking */
		lg2=wg/2;
		while(!( UCSR0A & (1<<UDRE0)));//wait for byte to transmit
		while(lg2--){
			UDR0=*b2++;
			while(!( UCSR0A & (1<<UDRE0)));//wait for byte to transmit
		}
	}
#else
	/* This code is very similar to qvga sending code except we have even more blanking time to take advantage of */
	while(hg--){
		uint8_t*b=buf,*b2=buf;
		lg2=wg/5;
		while(lg2--){
			while((PIND&4));//wait for low
			*b++=(PINC&15)|(PIND&240);
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low
			*b++=(PINC&15)|(PIND&240);
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low
			*b++=(PINC&15)|(PIND&240);
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low
			*b++=(PINC&15)|(PIND&240);
			while(!(PIND&4));//wait for high
			while((PIND&4));//wait for low
			*b++=(PINC&15)|(PIND&240);
			UDR0=*b2++;
			while(!(PIND&4));//wait for high
		}
		/* Finish sending the remainder during blanking */
		lg2=320-(wg/5);
		while(!( UCSR0A & (1<<UDRE0)));//wait for byte to transmit
		while(lg2--){
			UDR0=*b2++;
			while(!( UCSR0A & (1<<UDRE0)));//wait for byte to transmit
		}
	}
#endif
}




static void captureImg(uint16_t wg, uint16_t hg){
  uint16_t y, x;

  StringPgm(PSTR("*RDY*"));

  while (!(PIND & 8));//wait for high
  while ((PIND & 8));//wait for low

    y = hg;
  while (y--){
        x = wg;
      //while (!(PIND & 256));//wait for high
    while (x--){
      while ((PIND & 4));//wait for low
            UDR0 = (PINC & 15) | (PIND & 240);
          while (!(UCSR0A & (1 << UDRE0)));//wait for byte to transmit
      while (!(PIND & 4));//wait for high
      while ((PIND & 4));//wait for low
      while (!(PIND & 4));//wait for high
    }
    //  while ((PIND & 256));//wait for low
  }
    _delay_ms(100);
}

void setup(){
	cli();//disable interrupts
	/* Setup the 8mhz PWM clock 
	 * This will be on pin 11*/
	DDRB|=(1<<3);//pin 11
	ASSR &= ~( _BV(EXCLK) | _BV(AS2));
	TCCR2A=(1<<COM2A0)|(1<<WGM21)|(1<<WGM20);
	TCCR2B=(1<<WGM22)|(1<<CS20);
	OCR2A=0;//(F_CPU)/(2*(X+1))
	DDRC&=~15;//low d0-d3 camera
	DDRD&=~252;//d7-d4 and interrupt pins
	_delay_ms(3000);
	//set up twi for 100khz
	TWSR&=~3;//disable prescaler for TWI
	TWBR=72;//set to 100khz
	//enable serial
	UBRR0H=0;
	UBRR0L=1;//0 = 2M baud rate. 1 = 1M baud. 3 = 0.5M. 7 = 250k 207 is 9600 baud rate.
	UCSR0A|=2;//double speed aysnc
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);//Enable receiver and transmitter
	UCSR0C=6;//async 1 stop bit 8bit char no parity bits
	serialWrB(97);
	camInit();
  serialWrB(98);
	
	
#ifdef useVga
	setRes(VGA);
	setColorSpace(BAYER_RGB);
	wrReg(0x11,25);
#elif defined(useQvga)
  serialWrB(99);
  setRes(QVGA);
	setColorSpace(YUV422);
  serialWrB(99);
	wrReg(0x11,11);
  serialWrB(99);
#else
	setRes(QQVGA);
	setColorSpace(YUV422);
	wrReg(0x11,3);
#endif
	/* If you are not sure what value to use here for the divider (register 0x11)
	 * Values I have found to work raw vga 25 qqvga yuv422 12 qvga yuv422 21
	 * run the commented out test below and pick the smallest value that gets a correct image */
	
}
void loop(){
		/* captureImg operates in bytes not pixels in some cases pixels are two bytes per pixel
		 * So for the width (if you were reading 640x480) you would put 1280 if you are reading yuv422 or rgb565 */
		/*uint8_t x=63;//Uncomment this block to test divider settings note the other line you need to uncomment
		  do{
		  wrReg(0x11,x);
		  _delay_ms(1000);*/
		
//while(!serialRvB()=='A');
#define useQVga
#ifdef useVga
		captureImg(640,480);

#elif defined(useQvga)
		serialWrB(100);
		captureImg(320,240);
#else
		captureImg(160*2,120);
#endif
		//}while(--x);//Uncomment this line to test divider settings
	}

