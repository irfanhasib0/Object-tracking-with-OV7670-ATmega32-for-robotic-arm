#include <inttypes.h>

#ifndef USART_H
#define USART_H
/****************************************************************************************************/

//Macros
#define RECEIVE_BUFF_SIZE 6
#define USART_RXC_VECT USART_RXC_vect	//This change from mcu to mcu


#define UBRR_VALUE(baud) ((F_CPU/(baud<<4))-1)


//Varriables
void USARTInit(uint16_t ubrrvalue);
volatile char URBuff[RECEIVE_BUFF_SIZE];	//USART Receive Buffer
volatile int8_t UQFront;
volatile int8_t UQEnd;

//Functions
 char USART_Receive(void);
void USARTInit(uint16_t baudrate);

uint8_t UDataAvailable();

char UReadData();

void UWriteData(char data);
void UWriteString(char *str);




/****************************************************************************************************/


#endif




