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

/*
    0,                      //0x00 ping register
    0,                      //0x01 firmwareVersion 1/100
    I2C_ADDRESS,            //0x02 i2cAddress
    0,                      //0x03 config mode
    0,                      //0x04 locate functionality
    0,                      //0x05 SOC enabled
    SOC_THRESHOLD_ERROR+1,  //0x06 SOC
    0,                      //0x07 charging state: 0 = off, 1 = charging, 2 = discharging
    MAX_INTENSITY,          //0x08 max intensity
    2,                      //0x09 state color display (overrides all above): 0x00 normal, 0x01 off, 0x02 blue, 0x03 green, 0x04 orange, 0x05 red
    2,                      //0x0A state animation display (overrides all above): 0x00 normal, 0x01 static, 0x02 wave up, 0x03 wave down, 0x04 blink
    UPDATE_RATE,            //0x0B led update rate
    LED_TICK_THRESHOLD,     //0x0C led wave update rate
*/

enum chargeDischargeState {
    OFF, CHARGING, DISCHARGING  };

class BatteryLED {
    public:
        BatteryLED(void);
        void setBus(TwoWire &bus)                			{_i2c = &bus;}
        bool init(int pinSDA, int pinSCL, uint32_t speed);
        bool init(int pinSDA, int pinSCL);
        bool init(void); 
        bool compute(void);


        uint16_t getFirmwareVersion(void)                   {return _modules_data.data_firmware_version;}

        void setAddress(uint8_t value)             	        {_modules_data.data_i2c_address = value;}
        uint8_t getAddress(void)                    		{return _modules_data.data_i2c_address;}

        void setConfigMode(bool value)             	        {_modules_data.data_config_mode = value;}
        bool getConfigMode(void)                    		{return _modules_data.data_config_mode;}	

        void setLocate(bool value)              			{_modules_data.data_locate_enabled = value;}
        bool getLocate(void)                    			{return _modules_data.data_locate_enabled;}

        void setSOCenable(bool value)           			{_modules_data.data_soc_enabled = value;}
        bool getSOCenable(void)                 			{return _modules_data.data_soc_enabled;}

        void setSOC(uint8_t value)             				{_modules_data.data_soc = value;}
        uint8_t getSOC(void)                   				{return _modules_data.data_soc;}

		void setChargingState(chargeDischargeState value)	{
                                                            if(value == CHARGING)           _modules_data.data_charge_discharge_state = 0x01; 
															else if (value == DISCHARGING)  _modules_data.data_charge_discharge_state = 0x02;
															else                            _modules_data.data_charge_discharge_state = 0x00;
															}

		void setMaxIntensity(uint8_t value)             	{_modules_data.data_max_intensity = value;}
        uint8_t getMaxIntensity(void)                   	{return _modules_data.data_max_intensity;}

        void setStateColor(byte value)             	        {_modules_data.data_state_color = value;}
        byte getStateColor(void)                    		{return _modules_data.data_state_color;}

        void setStateAnimation(byte value)             	    {_modules_data.data_state_animation = value;}
        byte getStateAnimation(void)                        {return _modules_data.data_state_animation;}

        void setUpdateRate(uint16_t value)             	    {_modules_data.data_update_rate = value;}
        uint16_t getUpdateRate(void)                    	{return _modules_data.data_update_rate;}

        void setWaveRate(uint16_t value)             	    {_modules_data.data_led_wave_rate = value;}
        uint16_t getWaveRate(void)                    		{return _modules_data.data_led_wave_rate;}

		bool getCommunicationErrorState(void)				{return _modules_data.communication;}
        uint32_t getCRCerrors(void)             			{return _modules_data.crcerrors;}

    private:
        TwoWire *_i2c;          //pointer to the wire lib: https://forum.arduino.cc/t/passing-an-i2c-object-to-a-class/617064/15
        bool _checkModule(byte i2cAddress);
        bool _writedata(int i2cAddress, byte i2cRegister, uint16_t data);
        uint16_t _readdata(int i2cAddress, byte i2cRegister);

    struct modules_data_struct { 

        uint16_t        data_ping_register;                         //0x00            
        uint16_t        data_firmware_version;                      //0x01
        uint8_t         data_i2c_address = I2C_ADDRESS_LED_MODULE;  //0x02
        bool            data_config_mode = false;                   //0x03
        bool            data_locate_enabled = false;                //0x04: module locate function
        bool            data_soc_enabled = false;                   //0x05: soc display enabled
        uint8_t         data_soc = 0;                               //0x06: soc value [0...100]
		byte			data_charge_discharge_state = 0x00;         //0x07: 0x00 = no charging, 0x01 = charging, 0x02 = discharging
		byte			data_max_intensity = 80;			        //0x08: max intensity of led
        byte            data_state_color = 0x00;                    //0x09: 0x00 normal, 0x01 off, 0x02 blue, 0x03 green, 0x04 orange, 0x05 red
        byte            data_state_animation = 0x00;                //0x0A: 0x00 normal, 0x01 static, 0x02 wave up, 0x03 wave down, 0x04 blink
        uint16_t        data_update_rate = 5;                       //0x0B: ms, overall update rate
        uint16_t        data_led_wave_rate = 30;                    //0x0C: update counter for wave

		bool			communication = true;				        //communication with board
        uint32_t        crcerrors = 0;                              //crc communication errors since system start
    };
    modules_data_struct _modules_data;
    
};
#endif