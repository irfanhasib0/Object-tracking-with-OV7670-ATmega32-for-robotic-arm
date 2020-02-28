#include<avr/io.h>
#include<stdio.h>
#include <avr/interrupt.h>
#include<util/delay.h>
#include"pwm.h"
#include<usart.h>
#include<math.h>
#include"lcd.h"
#define l1 15
#define l2 15
#define s 1

struct position
{
 float x;
 float y;
 float z;

}pos;
struct angle
{
 float a;
 float b;
 float c;
 float d;
}ang;


struct angle get_angle(struct position pos);

void move(int x,int y,int z,int u);
int pulseIn(int a);
