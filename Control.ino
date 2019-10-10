#include <mcp_can.h>
#include <SPI.h>
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif

const int SPI_CS_PIN = 10; //10号针脚(CAN Bus Shield v1.2)
unsigned char stmp[8] = {0xff, 0x01, 0x00, 0x64, 0x00, 0x80, 0, 128}; //电机归中
unsigned char bin[16];  //存放16位的二进制代码
int posIni;
int posDifPrev;

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  SERIAL.begin(9600); //串口波特率
  CAN.begin(CAN_1000KBPS);
  
  posIni = analogRead(A0);
  SERIAL.println("CAN BUS Shield init ok!");
  CAN.sendMsgBuf(0x01, 0, 8, stmp); //执行电机归中
}


void loop() {
  int posDif = analogRead(A0) - posIni;
  if (abs(posDif) > 2) {
    if (abs(posDifPrev - posDif) > 1) {
      dec2bin(128+posDif/8);
      stmp[7] = bin2dec(8, 15);
      stmp[8] = bin2dec(0, 7);
      CAN.sendMsgBuf(0x01, 0, 8, stmp);
      posDifPrev = posDif;
    }
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
