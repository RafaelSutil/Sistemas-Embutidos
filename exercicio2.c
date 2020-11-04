#include <REG51F.H>

sbit bit0 = P2^0;
sbit bit1 = P2^1;

unsigned int cont1Seg = 31233;

void funcaoA(){
	static unsigned int contador = 0;
	static char estado = 0;

	switch(estado){
		case 0: 
			if(bit0 == 1){
				P1 = (P1 & 0xF0) | (P0 & 0x0F);
				estado = 2;
			}else{
				contador = 0;
				estado = 1;
			}
		break;
		case 1: 
			if(contador++ == cont1Seg){
				P1 = P1 & 0xF0;
				estado = 0;
			}	
		break;
						
		case 2:	//espera o bit retornar para 0
			if(bit0 == 0){
				estado = 0;
			}
		break;
	}
}

void funcaoB(){
	static unsigned int contador = 0;
	static char estado = 0;

	switch(estado){
		case 0: 
			if(bit1 == 1){
				P1 = (P1 & 0x0F) | (P0 & 0xF0);
				estado = 2;
			}else{
				contador = 0;
				estado = 1;
			}
		break;
		case 1: 
			if(contador++ == cont1Seg){
				P1 = P1 & 0x0F;
				estado = 0;
			}
		break;
						
		case 2:	//espera o bit retornar para 0
			if(bit1 == 0){
				estado = 0;
			}
		break;
	}
}

void main(){
	
	while(1){
		funcaoA();
		funcaoB();
	}
}