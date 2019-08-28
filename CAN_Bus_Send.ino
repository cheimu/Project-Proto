#include <mcp_can.h>
#include <SPI.h>
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif

const int SPI_CS_PIN = 10; //10号针脚(CAN Bus Shield v1.2)

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  SERIAL.begin(115200); //串口波特率
  while (CAN_OK != CAN.begin(CAN_1000KBPS)) { //检测CAN总线模块是否可用，通讯速率1000kbps
      SERIAL.println("CAN BUS Shield init fail");
      SERIAL.println(" Init CAN BUS Shield again");
      delay(100);
  }
  SERIAL.println("CAN BUS Shield init ok!");
}

unsigned char stmp[8] = {0xff, 0x01, 0x00, 0x0a, 0x00, 0x0c, 0, 0}; //传输内容

void loop() {
    CAN.sendMsgBuf(0x01, 0, 8, stmp); //发送内容
    delay(100);
}
