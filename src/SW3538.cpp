#include "SW3538.h"

SW3538::SW3538(uint8_t sdaPin, uint8_t sclPin) {
  _sdaPin = sdaPin;
  _sclPin = sclPin;
}

void SW3538::begin() {
  Wire.begin(_sdaPin, _sclPin);
  Wire.setClock(100000);
  _init();
}

void SW3538::update() {
  _Vout = _read_ADC(0x05) * 0.006; // 输出电压
  _Iout_C = _read_ADC(0x01) * 0.0025; // C口电流
  _Iout_A = _read_ADC(0x02) * 0.0025; // A口电流
  _Vin = _read_ADC(0x06) * 0.01; // 输入电压
  _prl = _read_data(0x09); // 协议类型
  _Pout = _Vout * (_Iout_A + _Iout_C); // 总输出功率
}

float SW3538::getVin() {
  return _Vin;
}

float SW3538::getVout() {
  return _Vout;
}

float SW3538::getIoutA() {
  return _Iout_A;
}

float SW3538::getIoutC() {
  return _Iout_C;
}

float SW3538::getPout() {
  return _Pout;
}

uint8_t SW3538::getProtocol() {
  return _prl & 0x0F; // 只返回低 4 位
}

const char* SW3538::getProtocolName() {
  static const char* protocols[] = {
      "NONE", "QC2.0", "QC3.0", "QC3+", "FCP", "SCP", "PD FIX", "PD PPS",
      "PE1.1", "PE2.0", "VOOC1.0", "VOOC4.0", "SFCP", "AFC", "TFCP", "UNKNOWN"
  };
  uint8_t protocol_index = _prl & 0x0F;
  if (protocol_index < 16) {
    return protocols[protocol_index];
  } else {
    return "UNKNOWN";
  }
}

void SW3538::_init() {
  Wire.beginTransmission(0x3c);
  Wire.write(0x10);
  Wire.write(0x20);
  Wire.endTransmission(1);

  Wire.beginTransmission(0x3c);
  Wire.write(0x10);
  Wire.write(0x40);
  Wire.endTransmission(1);

  Wire.beginTransmission(0x3c);
  Wire.write(0x10);
  Wire.write(0x80);
  Wire.endTransmission(1);

  Wire.beginTransmission(0x3c);
  Wire.write(0x15);
  Wire.write(0x20);
  Wire.endTransmission(1);

  Wire.beginTransmission(0x3c);
  Wire.write(0x15);
  Wire.write(0x40);
  Wire.endTransmission(1);

  Wire.beginTransmission(0x3c);
  Wire.write(0x15);
  Wire.write(0x80);
  Wire.endTransmission(1);

  Wire.beginTransmission(0x3c);
  Wire.write(0x18);
  Wire.write(0x40);
  Wire.endTransmission(1);
}

int SW3538::_read_ADC(unsigned char addr) {
  unsigned char tmp_L = 0, tmp_H = 0;
  int read_tmp = 0;

  Wire.beginTransmission(0x3c);
  Wire.write(0x40);
  Wire.write(addr);
  Wire.endTransmission(1);

  Wire.beginTransmission(0x3c);
  Wire.write(0x41);
  Wire.endTransmission(false);
  Wire.requestFrom(0x3c, 1);
  tmp_L = Wire.read();

  Wire.beginTransmission(0x3c);
  Wire.write(0x42);
  Wire.endTransmission(false);
  Wire.requestFrom(0x3c, 1);
  tmp_H = Wire.read();

  read_tmp = tmp_H << 8 | tmp_L;
  return read_tmp;
}

int SW3538::_read_data(unsigned char addr) {
  unsigned char read_data;
  Wire.beginTransmission(0x3c);
  Wire.write(addr);
  Wire.endTransmission(false);
  Wire.requestFrom(0x3c, 1);
  if (Wire.available()) {
    read_data = Wire.read();
  } else {
    read_data = 0xFF; // Indicate error
  }
  return read_data;
}