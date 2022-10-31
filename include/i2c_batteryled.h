/*
uses i2c channel 0
*/
#ifndef I2C_BATTERYLED_H
#define I2C_BATTERYLED_H
#include <Arduino.h>
#include <Wire.h>

//#define USED_I2C_HARDWARE       0
#define I2C_SPEED               1000ul
#define I2C_ADDRESS_LED_MODULE  0x02

class BatteryLED {
    public:
        BatteryLED(void);
        void setBus(TwoWire &bus)                {_i2c = &bus;}
        bool init(int pinSDA, int pinSCL, uint32_t speed);
        bool init(int pinSDA, int pinSCL);
        bool init(void); 
        bool compute(void);
        void setSOCenable(bool value)           {_modules_data.soc_enable = value;}
        bool getSOCenable(void)                 {return _modules_data.soc_enable;}
        void setSOC(byte value)                 {_modules_data.soc = value;}
        bool getSOC(void)                       {return _modules_data.soc;}
        void setLocate(bool value)              {_modules_data.locate = value;}
        bool getLocate(void)                    {return _modules_data.locate;}
        uint32_t getCRCerrors(void)             {return _modules_data.crcerrors;}

    private:
        TwoWire *_i2c;          //pointer to the wire lib: https://forum.arduino.cc/t/passing-an-i2c-object-to-a-class/617064/15
        bool _checkModule(byte i2cAddress);
        bool _writedata(int i2cAddress, byte i2cRegister, uint16_t data);
        uint16_t _readdata(int i2cAddress, byte i2cRegister);

    struct modules_data_struct { 
        bool            soc_enable = false;                 //soc display enabled
        uint8_t         soc = 0;                            //soc value [0...100]
        bool            locate = false;                     //module locate function
        uint32_t        crcerrors = 0;                      //crc communication errors since system start
    };
    modules_data_struct _modules_data;
    
};
#endif