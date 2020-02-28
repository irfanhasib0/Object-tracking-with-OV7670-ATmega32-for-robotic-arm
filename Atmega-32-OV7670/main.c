////////////////gausian
#include<avr/io.h>
#include<stdio.h>
#include <avr/interrupt.h>
#include<util/delay.h>
#include"pwm.h"
#include"usart.h"
#include<math.h>
#include"lcd.h"
#define l1 15
#define l2 15
#define M 40
#include"spi.h"





int main()
{
//USARTInit(103);
Timer_init();
LCDInit(LS_ULINE);
DDRD |=0b01111111;
uint8_t y[M][M];
uint8_t i=0,j=0,Cb,Cr,Y_NULL,i_max,j_max,min,t;
unsigned int max=0,i_min=0,j_min=0,dis=0,prev_dis=0,prev_i_min=0,prev_j_min=0,r;
double x,val,p;
//int t,a,b,c,d;
DDRB|=0XFF;
DDRC|=0b11111100;
//DDRC|=0XFF;

 while(1)
 {
 
 while(PINC!=1);
 while(PINC==1);
 //while(PINC!=0);//vsync
 j=0;
 while(j<M)
 {
 i=0;
 while(PINC==0);//href
 while(i<M)///Cr=PINA;///Cr---
 {
 if(PIND>127)
 {
 y[i][j]=PINA;
    
//Y+++++++++++++++
 //CB
 //if(y[i][j]<128)continue;
t=24;
while(t--);
i=i++;
//Y_NULL=PINA;//Y NULL

}

 //a=pulseIn(1);
 //LCDWriteIntXY(1,0,y[0],3);
 //_delay_ms(100);
 //PORTD|=(1<<0);PORTD&=(~(1<<0));
  
 }  
 while(PINC==2);
 
 
_delay_us(800);
  j++;
 
 }
 ////////////////////gamma correction
min=0;
 for(j=0;j<M;j++)
 {
for(i=0;i<M;i++)
{
p=y[i][j]/16.0;
y[i][j]=p*p;

if(y[i][j]<=100)
{
y[i][j]=0;
}
if(y[i][j]>100)
{
y[i][j]=y[i][j]-100;
y[i][j]=y[i][j]*1.67;
//if(y[i][j]>min+2)
//{
//min=y[i][j];
//}
y[i][j]=y[i][j]/25;
y[i][j]=y[i][j]*25;

}
}
}
////////////////////////////////
//////////////avg corr
/*r=0;
for(j=0;j<M;j++)
 {
for(i=0;i<M;i++)
{
r=r+y[i][j]/10.0;


}
}
min=0;
r=r/1600.0;
r=r*10;
for(j=0;j<M;j++)
 {
for(i=0;i<M;i++)
{
if(y[i][j]<=r)y[i][j]=0;
if(y[i][j]>r)y[i][j]=y[i][j]-r;
if(y[i][j]>min+2)
{
min=y[i][j];
}

}
}
*/
///////////////////////////



//////////////////color correction
 for(j=0;j<M;j++)
 {
for(i=0;i<M-3;i++)
{
p=y[i][j]+y[i+1][j]+y[i+2][j]+y[i+3][j];
r=p/4.0;

y[i][j]=r;
y[i+1][j]=r;
y[i+2][j]=r;
y[i+3][j]=r;


}
}




////////////////////////////////////


//////////////////gaussiuan 3X3 mask
 for(j=1;j<M-1;j++)
 {
for(i=1;i<M-1;i++)
{
p=0.0625*y[i-1][j-1]+0.125*y[i][j-1]+0.0625*y[i+1][j-1];
p=p+0.125*y[i-1][j]+0.25*y[i][j]+0.125*y[i+1][j];
p=p+0.0625*y[i-1][j+1]+0.125*y[i][j+1]+0.0625*y[i+1][j+1];

r=p/25.0;
r=r*25;
y[i][j]=p;




}
}




////////////////////////////////////




////////////////////////////////////////grad(sebel operator)


min=0;
for(j=1;j<M-1;j++)
 {
for(i=1;i<M-1;i++)
{
p=0.25*(y[i+1][j-1]-y[i-1][j-1]);
p=p+0.5*(y[i+1][j]-y[i-1][j]);
p=p+0.25*(y[i+1][j+1]-y[i-1][j+1]);
r=p*p;
x=0.25*(y[i-1][j+1]-y[i-1][j-1]);
x=x+0.5*(y[i][j-1]-y[i][j+1]);
x=x+0.25*(y[i+1][j-1]-y[i+1][j+1]);
r=r+x*x;
y[i][j]=sqrt(r);


if(y[i][j]>min+2)
{
min=y[i][j];
}

//y[i][j]=y[i][j]/10.0;
//y[i][j]=y[i][j]*10;

////////////////filtering pixels
x=x/p;
if(x>=-.26&&x<0.26)//////0  deg
{
if(y[i][j]<y[i+1][j]||y[i][j]<y[i-1][j])y[i][j]=0;
}

if(x>0.57&&x<1.73)//////45 deg
{
if(y[i][j]<y[i+1][j+1]||y[i][j]<y[i-1][j-1])y[i][j]=0;
}
if(x>3.73||x<-3.73)////////////90 deg
{
if(y[i][j]<y[i][j+1]||y[i][j]<y[i][j-1])y[i][j]=0;
}
if(x>-1.73&&x<-.57)//////////135 deg
{
if(y[i][j]<y[i-1][j+1]||y[i][j]<y[i+1][j-1])y[i][j]=0;
}

/////////////hysterisis
if(y[i][j]<=25)y[i][j]=0;
else if(y[i][j]>25&&y[i][j]<=30)
{
if(y[i+1][j]<40&&y[i-1][j]<40&&y[i][j+1]<40&&y[i][j-1]<40)
 y[i][j]=0;
}
else if(y[i][j]>30&&y[i][j]<40)
{
if(y[i+1][j]<40&&y[i-1][j]<40&&y[i][j+1]<40&&y[i][j-1]<40&&y[i+1][j+1]<40&&y[i-1][j-1]<40&&y[i-1][j+1]<40&&y[i+1][j-1]<40)
 y[i][j]=0;
}
//else if(y[i][j]<40)y[i][j]=0;

//////////////////
}
y[M-1][j]=y[M-2][j];
y[0][j]=y[1][j];
}
for(i=0;i<M;i++)
{
y[i][M-1]=y[i][M-2];
y[i][0]=y[i][1];
}


/////////////////////////////////


 
 max=0;

 for(j=0;j<M-1;j=j++)
 {
 //if(j%2==0)i=0;
 //if(j%2==1)i=1;
for(i=0;i<M-1;i=i++)
{

if(y[i][j]>max+2)
{
max=y[i][j];
i_max=i;
j_max=j;
}
//if((y[i+1][j+1]-y[i][j])>max)
//{
//max=y[i+1][j+1]-y[i][j];
//i_max=i;
//j_max=j;
//}


}
}


val=0;
x=0;
 dis=0;
for(i=0;i<M;i=i++)
{
//if(y[i][j_max]>(y[i_max][j_max]-10))
//{
val=val+y[i][j_max];
x=x+(i+1)*y[i][j_max];
if(y[i][j_max]>10)
dis++;
//}
}
x=x/val;
x=x*10;
i_min=(int)x;

val=0;
x=0;
//if(j_max%2==0)
//{
for(j=0;j<M;j=j++)
{
//if(y[i_max][j]>(y[i_max][j_max]-10))
//{
val=val+y[i_max][j];
x=x+(j+1)*y[i_max][j];
if(y[i_max][j]>10)
dis++;
//}
}
x=x/val;
x=x*10;
j_min=(int)x;



//}
/*
if(j_max%2==1)
{
for(j=1;j<M;j=j+2)
{
val=val+y[i_max][j];
x=x+(j/2)*y[i_max][j];
}
x=x/val;
x=x*10;
j_min=(int)x;
}
*/
 /*
 if(USART_Receive()=='a')
 {
 _delay_ms(1);
UWriteData('a');
 for(j=0;j<40;j++)
 {
 for(i=0;i<40;i++)
 { USART_Receive();
 _delay_ms(1);
 UWriteData(y[i][j]);

 }
 }
 }*/

//_delay_ms(50);

dis=(int)(dis*4);
i_min=(int)(i_min/2);
j_min=(int)(j_min/2);


prev_dis=(prev_dis+dis)/2;
prev_i_min=(prev_i_min+i_min)/2;
prev_j_min=(prev_j_min+j_min)/2;

dis=(dis+prev_dis)/2;
i_min=(i_min+prev_i_min)/2;
j_min=(j_min+prev_j_min)/2;

dis=(int)(dis/10)*10;
i_min=(int)(i_min/10)*10;
j_min=(int)(j_min/10)*10;



LCDWriteIntXY(1,0,dis,3);
LCDWriteIntXY(5,0,i_max,3);
LCDWriteIntXY(9,0,j_max,3);
LCDWriteIntXY(13,0,y[20][20],3);

LCDWriteIntXY(1,1,min,3);
LCDWriteIntXY(5,1,i_min,3);
LCDWriteIntXY(9,1,j_min,3);
LCDWriteIntXY(13,1,PINC,3);

Timer_1A(i_min+5);
Timer_1B(j_min+5);
Timer_0(dis+5);
/*if(USART_Receive()=='n')
 {
 _delay_us(100);
UWriteData('a');
_delay_us(100);
UWriteData('a');
_delay_us(100);
UWriteData('a');
}*/
/*  LCDWriteIntXY(1,0,(int)y[39][39],3);
while(1)
{

PORTD|=(1<<0);
//_delay_us(.0625);
PORTD&=(~(1<<0));

}
*/

}
}