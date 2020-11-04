#include <REG51F.H>

sbit bit0 = P2^0;
sbit bit1 = P2^1;


#define FrClk 12000000
#define FreqTimer0_emHz 100
#define TH0_Inicial ((65536-(FrClk/(12*FreqTimer0_emHz))+118)>>8)
#define TL0_Inicial ((65536-(FrClk/(12*FreqTimer0_emHz))+118)&0xFF)

static char funcao;
int contador;



void timer0_inicializa(){
	TR0 = 0;
	TMOD = (TMOD & 0xF0)|0x01;
	TH0 = TH0_Inicial; 
	TL0 = TL0_Inicial;
	ET0 = 1; 
}

void timer0_int (void) interrupt 1 using 2{
	TR0 = 0;
	TL0 += TL0_Inicial;
	TH0 += TH0_Inicial + (unsigned char) CY;
	if(contador == 100){
		contador = 0;
		if(funcao == 'A'){
			P1 = P1 & 0xF0;
		}
		if(funcao == 'B'){
			P1 = P1 & 0x0F;
		}
	}else{
		contador++;
		TR0 = 1;
	}
}

void funcaoA(){
	static char estado = 0;

	switch(estado){
		case 0: 
			if(bit0 == 1){
				P1 = (P1 & 0xF0) | (P0 & 0x0F);
				estado = 2;
			}else{
				estado = 1;
			}
		break;
		case 1:
			TR0 = 1;
			funcao = 'A';
			estado = 3;
		break;
						
		case 2:
			if(bit0 == 0){
				estado = 0;
			}
		break;
			
		case 3:
				if(bit0 == 1){
					estado = 0;
				}
	}
}

void funcaoB(){
	static char estado = 0;

	switch(estado){
		case 0: 
			if(bit1 == 1){
				P1 = (P1 & 0x0F) | (P0 & 0xF0);
				estado = 2;
			}else{
				estado = 1;
			}
		break;
		case 1: 
			TR0 = 1;
			funcao = 'B';
			estado = 3;
		break;
						
		case 2:
			if(bit1 == 0){
				estado = 0;
			}
		break;
		case 3:
			if(bit1 == 1){
				estado = 0;
			}	
	}
}

void main(){
	timer0_inicializa();
	EA = 1;
	while(1){
		funcaoA();
		funcaoB();
		
	}
}