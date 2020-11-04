#include <REG51F.H>
#define BUFFERSIZE 0x10

unsigned char TxBuffer[BUFFERSIZE], 
							RxBuffer[BUFFERSIZE],// = {'R','a','f','a','e','l',' ','S','u','t','i','l','$'}, 
							
							TxIn=0, 
							TxOut=0,
							RxIn=0,
							RxOut=0,
							TxEspera = 0,
							teste[BUFFERSIZE],
							recebeChar;

int v=1;

void init(){
  SCON = 0x90;              //modo 1 REN=1
  ES = 1;                   //habilita interrupcao serial
}

void interrupcao_serial (void) interrupt 4{
	if(RI == 1){
		RI = 0;
		recebeChar = SBUF;
		RxBuffer[RxIn] = recebeChar;
		RxIn++;
		if(recebeChar == '$')
			v = 0;
		if(RxIn == 16)
			RxIn = 0;
	}
	if(TI == 1){
		TI = 0;
		if(TxOut == TxIn) TxEspera = 0;
		else{
			SBUF = TxBuffer[TxOut];
			TxOut = (TxOut + 1) & 0xF;	//soma sem dar overflow da fila
		}
	}
}


char RxBufferVazio(){
	if(RxIn == RxOut){
		return 1;
	}else{
		return 0;
	}
}
void SendChar(char c){
	TxBuffer[TxIn] = c;
	TxIn = (TxIn + 1) & 0xF;

	if(!TxEspera) {
		TxEspera = 1;
		TI = 1;
	}
}

void SendString(char *s){
	char i=0;
	while(s[i] != '$'){
		SendChar(s[i]);
		i++;
	}
}



char ReceiveChar() {
	char c = RxBuffer[RxOut];
	RxOut = (RxOut +1) & 0xf;
	return c;
}

void ReceiveString(char *s){
	char x = ReceiveChar();
	int i=0;
	while(x != '$'){
		s[i] = x;
		i++;
		x = ReceiveChar();
	}
	s[i+1]='$';
}

void main(){
	init();
	EA = 1;
	
	
	while(v==1);
	ReceiveString(teste); //Tira de Rx e coloca na variavel teste
	SendString(teste);		//Tira da variavel teste e transmite
	while(1);
}

/*
long int = 17
int = 4

3: Peephole optimization
4: Register Variables
5: Common subexoression elimination
*/

