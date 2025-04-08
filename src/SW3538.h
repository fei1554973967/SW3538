#ifndef SW3538_h
#define SW3538_h

#include <Wire.h>

class SW3538 {
  public:
    SW3538(uint8_t sdaPin, uint8_t sclPin);
    void begin();
    void update();
    float getVin();
    float getVout();
    float getIoutA();
    float getIoutC();
    float getPout();
    uint8_t getProtocol();
    const char* getProtocolName(); // 新增函数，返回协议名称

  private:
    uint8_t _sdaPin;
    uint8_t _sclPin;
    float _Vin, _Vout, _Iout_A, _Iout_C, _Pout;
    uint8_t _prl;

    void _init();
    int _read_ADC(unsigned char addr);
    int _read_data(unsigned char addr);
};

#endif
