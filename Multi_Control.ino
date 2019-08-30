#include <mcp_can.h>
#include <SPI.h>
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif

unsigned char stmp[8] = {0xff, 0x01, 0x00, 0x64, 0x00, 0x80, 0, }; //8段控制代码
unsigned char bin[16];  //存放16位的二进制代码

int posIni0;
int posDifPrev0;
int posDif0;

int posIni1;
int posDifPrev1;
int posDif1;

MCP_CAN CAN0(10);
MCP_CAN CAN1(9);

void setup() {
  SERIAL.begin(9600); //串口波特率
  CAN0.begin(CAN_1000KBPS);
  CAN1.begin(CAN_1000KBPS);
  
  posIni0 = analogRead(A0);
  posIni1 = analogRead(A1);
  
  CAN0.sendMsgBuf(0x01, 0, 8, stmp); //执行电机归中
  CAN1.sendMsgBuf(0x01, 0, 8, stmp); //执行电机归中
}

void loop() {
  posDif0 = analogRead(A0) - posIni0;
  if (abs(posDifPrev0 - posDif0) > 2) {
    dec2bin(128 + posDif0 / 8);
    stmp[7] = bin2dec(8, 15);
    stmp[8] = bin2dec(0, 7);
    CAN0.sendMsgBuf(0x01, 0, 8, stmp);
    posDifPrev0 = posDif0;
  }
  delay(10);
  
  posDif1 = analogRead(A1) - posIni1;
  if (abs(posDifPrev1 - posDif1) > 2) {
    dec2bin(128 + posDif1 / 8);
    stmp[7] = bin2dec(8, 15);
    stmp[8] = bin2dec(0, 7);
    CAN1.sendMsgBuf(0x01, 0, 8, stmp);
    posDifPrev1 = posDif1;
  }
  delay(10);
}

void dec2bin(int num) {  //十进制转二进制并存入bin
  for (int i = 15; i >= 0; i--) {
    bin[i] = num % 2;
    num = num / 2;
  }
}

int bin2dec(int INI, int TER) { //二进制转十进制
  int b2d = 0;
  int power = 0;
  for (int i = TER; i >= INI; i--) {
    b2d += bin[i] * int(pow(2, power)*100+1)/100; //解决精度问题
    power++;
  }
  return b2d;
}
