#include <reg51.h>

#define TH1value 0x3c
#define TL1value 0xb0
#define Maxcount 20

unsigned int time=0;
unsigned int count=0;

//change decimal number to hex number for prepare to display
unsigned int Change10to16(unsigned int time){
	//calculate bit
    unsigned int time10=time/10;
	//calculate ten
	unsigned int time1=time%10;
	//get the final hex number (since the position of the LED is special, ten is behind one.)
	time=time10+time1*16;
	return time;
}

//display the time
void Display(unsigned int time){
	//P2 is used to display.
	P2=Change10to16(time);
}

//Use timer1 to count time
//timer1 initial
//timer -> 0.01s
void init_timer1(){
	TMOD=0x10;
	TH1=TH1value;
	TL1=TL1value;
	TR1=1;
}

//External int init
void init_external(){
	IT0=1;
	IT1=1;
}

//int init
void init_int(){
	EA=1;
	ET1=1;
	EX0=1;
	EX1=1;
}

main(){
	init_timer1();
	init_external();
	init_int();
	Display(time);
	while(1);
}

void int_external0() interrupt 0{
	if(TR1==1){
		TR1=0;
	}else{
		TR1=1;
	}
}

void int_external1() interrupt 2{
	//store TR1 station
	unsigned int TR1store=TR1;
	//stop timer
	TR1=0;
	//clear time
	time=0;
	//display time
	Display(time);
	//clear timer
	TH1=TH1value;
	TL1=TL1value;
	//take TR1 back
	TR1=TR1store;
}

void int_timer1() interrupt 3{
	if(count==Maxcount){
		count=0;
		if(time==99){
			time=0;
		}else{		
			time++;
		}
		Display(time);
	}else{
		count++;
	}
}