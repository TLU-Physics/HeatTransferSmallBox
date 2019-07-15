/*
 * ThermoPlate.ino
 * Eric Ayars
 * 5/2/12
 * 
 * This version reads the DS18B20 addresses from eeprom, then reports
 * the temperatures on the serial line. Obviously the addresses must
 * be placed IN the eeprom first, in the right order, via a program 
 * such as address_storage.ino.
 *
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

#define NDevices 100                    // Number of DS18B20s
#define BUS 10                          // OneWire bus on pin 10

OneWire oneWire(BUS);
DallasTemperature sensors(&oneWire);    // pass to the DT library

DeviceAddress TAddress;                 // 8-bit T-sensor address
int EAddress;                           // EEPROM address pointer

void setup() {
    // Just get ready for communication. loop() does all the work.
    Serial.begin(57600);
    sensors.begin();
}

void loop() {
    
    // Send out a global "check temperatures" command
    sensors.requestTemperatures();

    // Now get those temperatures
    EAddress = 0;
    for (byte j=0;j<NDevices;j++) {     // loop over devices
        for (byte k=0;k<8;k++) {        // loop over address bytes
            TAddress[k] = EEPROM.read(EAddress);
            EAddress += 1;
        }
        // Now TAddress is the next sensor address, read it
        // and send the results on the serial line.
        Serial.print(sensors.getTempC(TAddress));
        Serial.print(" ");
    }
    // Now we're done with all sensors, so end the line.
    Serial.println("");
}

