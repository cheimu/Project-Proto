int Num=18000; //需要转换的数字

unsigned char bin[16];  //存放16位的二进制代码

void setup() {
  Serial.begin(9600); //启用串口
}

void loop() {
  dec2bin();
  for (int i = 0; i <= 15; i++) {
    Serial.print(bin[i]); //打印出二进制代码
  }
  Serial.println();
  bin2dec(0, 7);  //将高八位转换回十进制
  bin2dec(8, 15); //将第八位转换为十进制
  while(1);
}

void dec2bin() {  //十进制转二进制
  for (int i = 15; i >= 0; i--) {
    bin[i] = Num % 2;
    Num = Num / 2;
  }
}

int bin2dec(int INI, int TER) { //二进制转十进制，INI为LIST
  int b2d = 0;
  int power = 0;
  for (int i = TER; i >= INI; i--) {
    b2d += bin[i] * int(pow(2, power)*100+1)/100; //解决精度问题
    power++;
  }
  return b2d;
}
