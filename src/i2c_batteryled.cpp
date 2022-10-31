#include "i2c_batteryled.h"

//constructor - does nothing
BatteryLED::BatteryLED(void) {
    }

bool BatteryLED::init(int pinSDA, int pinSCL, uint32_t speed) {
    _i2c->begin(pinSDA, pinSCL, speed);      
    return true;
    }

bool BatteryLED::init(int pinSDA, int pinSCL) {
    //_i2c.begin(13, 16, I2C_SPEED);  //ESP32: I2C_SDA, I2C_SCL - ESP32-EVB
    _i2c->begin(pinSDA, pinSCL, I2C_SPEED);      //ESP32: I2C_SDA, I2C_SCL - ESP32-EVB
    return true;
    }

bool BatteryLED::init(void) {
    return init(4, 17);
    }

bool BatteryLED::compute(void) {
	_modules_data.commerror = false;
	
    //check module
    //Serial.println("Check Module");
    //Serial.println(_checkModule(I2C_ADDRESS_LED_MODULE));
	
    //locate
    //Serial.println("Locate");
    if (_modules_data.locate)
        _modules_data.commerror = _writedata(I2C_ADDRESS_LED_MODULE, 0x04, true);
    else    
        _modules_data.commerror = _writedata(I2C_ADDRESS_LED_MODULE, 0x04, false);

	if (_modules_data.commerror)
		return false;
		
    //soc enabled
    //Serial.println("SOC Enable");
    if (_modules_data.soc_enable)
        _modules_data.commerror = _writedata(I2C_ADDRESS_LED_MODULE, 0x05, true);
    else    
        _modules_data.commerror = _writedata(I2C_ADDRESS_LED_MODULE, 0x05, false);

	if (_modules_data.commerror)
		return false;
		
    //soc
    Serial.println("SOC");
	Serial.println(_modules_data.soc);
    _modules_data.commerror = _writedata(I2C_ADDRESS_LED_MODULE, 0x06, _modules_data.soc);

	if (_modules_data.commerror)
		return false;
		
    return true;
    }

//check if a i2c device is available on the bus. Returns true if board is there, otherwise false.
bool BatteryLED::_checkModule(byte i2cAddress) {
    _i2c->beginTransmission(i2cAddress);
    byte state = _i2c->endTransmission();
    if (!state) 
        return true;    
    return false;
    }

bool BatteryLED::_writedata(int i2cAddress, byte i2cRegister, uint16_t data) {
    //cehck if address is in range 
    if(i2cAddress > 127) 
        return false;

    _i2c->beginTransmission(i2cAddress);                   //queuing the slave address
    _i2c->write(i2cRegister);                              //queuing the register address/pointing regsiter
    _i2c->write(data >> 8);                                //higher byte first
    _i2c->write(data >> 0);
    byte busStatus = _i2c->endTransmission();              //transmit all queued data and bring STOP condition on I2C Bus
    if(busStatus != 0x00) {
        _modules_data.crcerrors++;                          //add up global crc error counter
        return false;
        }  
    return true;
    }

uint16_t BatteryLED::_readdata(int i2cAddress, byte i2cRegister) {
    //first tell the module which data has to be read from it
    _i2c->beginTransmission(i2cAddress);                   //queuing the slave address
    _i2c->write(i2cRegister);                              //queuing the register address/pointing regsiter   
    byte busStatus = _i2c->endTransmission();              //transmit all queued data and bring STOP condition on I2C Bus
    if(busStatus != 0x00) {
        return(0xFFFF);
        }   

    //now get the data containing of 2bytes of data and a CRC byte
    _i2c->requestFrom(i2cAddress, 3);                      //this is looping code; when 2-byte has arrived, the loop terminates
    byte byte1 = _i2c->read();                             //read the first byte from FIFO Buffer, higher
    byte byte2 = _i2c->read();
    uint16_t data = (byte1 << 8) | byte2;                 //16-bit data is formed

    //return the data if everything is ok
    return(data);                                         
    }