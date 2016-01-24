#include "temperatureSens.h"

//slave select
#define Sensor_SS       D,5

void initTempSens() {

	spi_init();
	__OUTPUT(Sensor_SS);
	__HIGH(Sensor_SS);
}

float actualTemperature() {

   float RoomTemperature,ADC_Temp_Code_dec;
    int TempVal =0;
    uint8_t SensorData[2]= {0x00,0x00};
    __LOW(Sensor_SS);
    _delay_us(10);
    //Read the temperature data 2 bytes

    spi_transfer_sync (SensorData,SensorData,2);

    _delay_us(10);
    __HIGH(Sensor_SS);


    // convert the temperature to celsius and display it:
    TempVal = (SensorData[0] << 8) + SensorData[1];
    ADC_Temp_Code_dec = (float)TempVal ;
    return (RoomTemperature = TempVal / 32.0);
}
