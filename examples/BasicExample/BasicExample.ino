#include <SW3538.h>

SW3538 sw3538(6, 7); // SDA 和 SCL 引脚

void setup() {
  Serial.begin(115200);
  sw3538.begin();
}

void loop() {
  sw3538.update();
  Serial.print("Vin: ");
  Serial.print(sw3538.getVin());
  Serial.print(" V, ");
  Serial.print("Vout: ");
  Serial.print(sw3538.getVout());
  Serial.print(" V, ");
  Serial.print("Iout_A: ");
  Serial.print(sw3538.getIoutA());
  Serial.print(" A, ");
  Serial.print("Iout_C: ");
  Serial.print(sw3538.getIoutC());
  Serial.print(" A, ");
  Serial.print("Pout: ");
  Serial.print(sw3538.getPout());
  Serial.print(" W, ");
  Serial.print("Protocol: ");
  Serial.println(sw3538.getProtocolName()); // 显示协议名称

  delay(100);
}
