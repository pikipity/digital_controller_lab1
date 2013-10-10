#include <reg52.h>

#define TH1value 0x63
#define TL1value 0xC0
#define Maxcount 50

unsigned char time=0;
unsigned char count=0;

//change decimal number to hex number for prepare to display
unsigned char Change10to16(unsigned char time){
	//calculate bit
    unsigned char time10=time/10;
	//calculate ten
	unsigned char time1=time%10;
	//get the final hex number (since the position of the LED is special, ten is behind one.)
	time=time10+time1*16;
	return time;
}

//display the time
void Display(unsigned char time){
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
	while(1){
		Display(time);
		if(count==Maxcount){
			count=0;
			if(time==99){
				time=0;
			}else{		
				time++;
		    }
	    }
	}
}

void int_external0() interrupt 0{
	TR1=~TR1;
}

void int_external1() interrupt 2{
	if(!TR1){
		//clear time
		time=0;
		//display time
		Display(time);
		//clear timer
		TH1=TH1value;
		TL1=TL1value;
	}
}

void int_timer1() interrupt 3{
	count++;
	TH1=TH1value;
	TL1=TL1value;
}
