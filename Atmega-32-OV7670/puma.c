#include"puma.h"

int pulseIn(int a)

{
float t1=0,t2=0,i=0;

while((PINA&(1<<a))!=0){}

while((PINA&(1<<a))==0){}

t1=0;
t2=0;
while((PINA&(1<<a))!=0){
t1++;
//_delay_us(1);
}
while((PINA&(1<<a))==0){
t2++;
//_delay_us(1);
}
return (int)((t1/(t1+t2))*100);
}




struct angle get_angle(struct position pos)
{
 float a,b,c;
 int p,q;
 //struct angle an;
 float sin_a,cos_a,sin_b,cos_b,tan_a,tan_b,k2,k1,x1,x2;

 x1=sqrt(pos.x*pos.x+pos.z*pos.z);

 //if(x1>25||x1<5)return;

 a=(float)((x1*x1)+(pos.y*pos.y));

 b=l1*l1+l2*l2;
 c=l1*l2;

 cos_b=(a-b)/(float)(2*c);
 sin_b=sqrt(1-cos_b*cos_b);

 ang.b=acos(cos_b);
 ang.b=(ang.b*180)/3.1415;
 ang.b=ang.b;


 k1=l1+l2*cos_b;
 k2=l2*sin_b;

 ang.a=atan(pos.y/(float)x1)-atan(k2/(float)k1);
 if(ang.a<0)
 ang.a=-ang.a;
 ang.a=(ang.a*180)/3.1416;

 ang.c=asin(pos.z/(float)x1);
 ang.c=(ang.c*180)/3.1416;
 ang.d=ang.b-ang.a;
 ang.d=ang.d-90;



 p=ang.a;
 LCDWriteIntXY(0,1,p,5);

 q=ang.b;
 LCDWriteIntXY(7,1,q,5);
 LCDWriteIntXY(7,0,ang.c,5);

 LCDWriteIntXY(0,0,x1,5);
 return ang;
}



void move(int x,int y,int z,int u)
{




DDRD |=0xFF;
DDRA |=0x00;
DDRB |=0xFF;
DDRC |=0xFF;






pos.x=x;
pos.y=y;
pos.z=z;

ang=get_angle(pos);
if((90-ang.c)>1)
Timer_0(90-ang.c);
if(ang.a>1)
Timer_1A(ang.a);
if(ang.b>1)
Timer_1B(ang.b);
//if((90-ang.d)>1)
//Timer_2(90-ang.d);



//_delay_ms(1);





}
