// This program is a demo of the drawBitmap()-function.
/************************************************************************
	This code and library will only work with:
	"3.2 inch 320x240 Touch LCD (C)" (Link: https://goo.gl/GAUREi )
		+
	"3.2 inch 320x240 Touch LCD (C) Shield" (Link:  )
		+
	Arduino Mega 2560 (Link: https://goo.gl/XQ5pkX )
	
	Module Developed by: 
		Md. Fahimul Islam
		Research Engineer
		fahimul@techshopbd.com
		http://www.TechShopBD.com 
************************************************************************/
#include <TechShopBDScreen.h>
#include <avr/pgmspace.h>

TechShopBDScreen    myGLCD(ILI9325D_16,38,39,40,41);
extern uint8_t SmallFont[];

//extern unsigned int icon1[0x400];
//extern unsigned int icon2[0x400];
///extern unsigned int tux[0x1000];
char frame [64][48];
int count=0,pixel=0;
long int T1=0,T2=0;
void setup()
{
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.fillScr(255, 255, 255);
  myGLCD.setFont(SmallFont);
  myGLCD.setColor(255,255,0);
  myGLCD.setBackColor(64, 64, 64);
   // myGLCD.printNumI(frame[i][j], 200,200,3,200);
  myGLCD.print("imachine", CENTER, 227);
  Serial.begin(9600);
DDRK &=0x00; 
pinMode(2,INPUT);
pinMode(3,INPUT);
pinMode(4,INPUT);
}

void loop()
{
// Draw a 4 by 4 grid of a 32x32 icon.
  
  long int i,j,t;

  //while(1){ 
    myGLCD.clrScr();
    myGLCD.setColor(255,255,0);
    myGLCD.setBackColor(64, 64, 64);
    myGLCD.printNumI(T2, 200,200,4);
    myGLCD.printNumI(frame[25][25], 120,200,3);
    myGLCD.printNumI(frame[40][40], 160,200,3);
    //Serial.println((int)frame[0][0]);
    myGLCD.print("imachine...", CENTER, 227);
 // }
  for(i=0;i<48;i++)
  {
    for(j=0;j<64;j++){
    int color=(int)frame[i][j];
    myGLCD.setColor(color,color,color);
    myGLCD.drawPixel (i, j);
    
  }

  //delay(1000);
  } 
 T1=millis();
 readFrame();
 T2=T1-millis();
           
      
}
void readFrame()
{
Serial.println("frame");
//Vsync
 memset(frame,0,sizeof(frame)); 
 
 while(digitalRead(4)!=1){}
 Serial.print(digitalRead(4));
 while(digitalRead(4)==1){}
 Serial.print(digitalRead(4));
 //attachInterrupt(digitalPinToInterrupt(2),getPixel,RISING);
 int j=0,t=0;
 while(j<48)
 {
 int i=0;
 count=0;
 while(digitalRead(3)==0){}//href
 //Serial.print(digitalRead(3));
 while(i<64)
 {
 if(digitalRead(2)){
 frame[i][j]=PINK;
 i++;
 t=114;
 while(t--){}
 }
 }
while(digitalRead(3)==1){}/// href high
//Serial.print(digitalRead(3));
delayMicroseconds(882); //////////784*2*9*(1/16)
j++;
}
//detachInterrupt(digitalPinToInterrupt(2));
while(digitalRead(4)==0){}; 
Serial.print(digitalRead(4));
Serial.print("end"); 
}

void getPixel()
{ 
  pixel=PINK;
  count++;
  if(count==19)count==0;
}


