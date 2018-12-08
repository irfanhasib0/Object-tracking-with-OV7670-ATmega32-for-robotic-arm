#include"pwm.h"
#define max 255 

    
	
	void Timer_init()
	{
	sei();
  //Timer_1A(150);
  //Timer_1B(200);
  //Timer_0(130);
  //Timer_2(150);
  _delay_ms(10);
  ICR1=0;
  TCCR1A |=0x00;
  TCNT1 |=0x00;
  TCCR1B |=0x00;
  TCNT1 |=0x00;
  TCCR0 |=0x00;
  TCNT0 |=0x00;
  TCCR2 |=0x00;
  TCNT2 |=0x00;
  TIMSK = 0x00; 
  TIFR |=(1<<TOV1);
  TIFR |=(1<<OCF0);
  TIFR |=(1<<OCF2);
  }

	void Timer_1A(int ocr1a)
	{
	ICR1=max;
	OCR1A=ocr1a;
	TCCR1A|=0b10100010;
	TCCR1B|=0b00011011;
	TIMSK |= (1<<TOIE1);
	//_delay_ms(1);
	
	
	}
	void Timer_1B(int ocr1b)
	{
	ICR1=max;
	OCR1B=ocr1b;
	TCCR1A|=0b10100010;
	TCCR1B|=0b00011011;
	//_delay_ms(1);
	
	}
	void Timer_0(int ocr0)
	{
	ICR1=max;
	TCCR1A|=0b10100010;
	TCCR1B|=0b00011011;
	TIMSK |= (1<<TOIE1);

	OCR0=ocr0;
	TCCR0 |=0b01001011;
	//_delay_ms(1);
	
	}
	//void Timer_2(int ocr2)
	//{
	//ICR1=max;
	//TCCR1A|=0b10100010;
	//TCCR1B|=0b00011010;
	//TIMSK |= (1<<TOIE1);
	
	
	//OCR2=D+ocr2;
    //TCCR2 |=0b01101010;	
	//_delay_ms(1);
	//}
	
ISR(TIMER1_OVF_vect)
{
  PORTD ^=(1<<3);
  //PORTD ^=(1<<6);
  
  
   
   //TIFR|=(1<<OCF2);
   TIFR|=(1<<OCF0);
   //TIMSK |= (1<< OCIE2);
   TIMSK |= (1<< OCIE0);
   //TCNT2=0;
   TCNT0=0;


}
//ISR(TIMER2_COMP_vect)
//{
		
  //PORTD &= ~(1<<6);
  //TIMSK &= ~(1<< OCIE2);
   


//}
ISR(TIMER0_COMP_vect)
{
		
  PORTD &= ~(1<<3);
  TIMSK &= ~(1<< OCIE0);
   
}

/*	
TCCR1A

Bit         7        6    5     4      3     2    1     0
         COM1A1 COM1A0 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10 TCCR1A
Read/Write R/W    R/W     R/W    R/W   W    W    R/W   R/W
Initial Value 0     0      0       0    0    0    0      0


TCCR1A|=0b10100010;

Bit            7     6           5     4      3     2    1    0
             ICNC1 ICES1   –  WGM13  WGM12  CS12  CS11  CS10 TCCR1B
Read/Write    R/W   R/W   R    R/W   R/W    R/W   R/W  R/W
Initial Value 0      0    0     0     0      0     0     0

TCCR1B|=0b00011010;


//COM 10 ..... clear on compare match
//wgm 1110 ... Fast PWM + TOP->ICR1
//CS  010..... PREScAllER 8


Fpwm=(Fcpu/pre-scalar)/(1+TOP);


              Bit 7     6      5     4    3    2       1     0
                 FOC0  WGM00 COM01 COM00 WGM01 CS02  CS01   CS00  TCCR0
Read/Write        W   R/W    R/W    R/W   R/W  R/W    R/W   R/W
Initial Value     0      0     0      0    0     0     0       0
TCCR0 |=01101010;


              Bit   7     6      5      4      3      2     1    0
                  FOC2  WGM20  COM21  COM20  WGM21  CS22  CS21  CS20  TCCR2
      Read/Write   W     R/W   R/W    R/W    R/W    R/W   R/W  R/W
   Initial Value   0      0     0      0      0       0     0   0
   TCCR2 |=01101010;
//COM 10 ..... clear on compare match
//wgm 11 ... Fast PWM + TOP=255
//CS  010..... PREScAllER 8
*/
