#include <REG51F.H>

unsigned char receivedChar=0;

void init(){
  TR1 = 0;

  SCON = 0x50;              //modo 1 01010000
  TH1 = 230;                //ajusta baudrate para 1201bps
  TMOD = 0x20;              //timer 1 modo 2
  PCON = (PCON & 0x7F);     //SMOD = 0

  ES = 1;                   //habilita interrupcao serial
  ET1 = 0;                  //desabilita interrupcao do timer 1
  EA = 1;                   //habilita tratamento de interrupcoes

  TR1 = 1;                  //inicia Timer 1
}

void interrupcao_serial (void) interrupt 4{
  if(RI == 1){
    receivedChar = SBUF;    //copia o char recebido
    SBUF = receivedChar + 1;//manda para interface somando 1
    RI = 0;                 //limpa o flag de interrupcao Receive
  }
  if(TI == 1){
    TI = 0;                 //limpa a flag de interrupcao Transmit
  }
}

void main(){
  init();
  while(1);
}