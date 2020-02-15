//  ** updated 05/07/2019 **
// This is written for an Arduino Leonardo.
// - it uses Serial1 instead of Serial, if you change it to Serial it should work for an UNO or other boards
//
// For CS5464 chip from Cirrus
// Read the peak current on channel I2
// Check for zero value coming back from CS5464, if so, then reset the device
// Original Code from : https://corgitronics.com/2014/06/30/using-the-cirrus-logic-cs5464-for-ac-current-measurement/

#include <SPI.h>
#include <CS5464.h>

#define Serial1 Serial

// Pin configurations
// set pin 12 as the slave select for the digital pot:
const int slaveSelectPin = 5;
CS5464 emon_ic(slaveSelectPin);

// Initialize SPI and pins
void setup() {
  Serial1.begin(9600);
  delay(5000); // Pause allowing time to open the serial monitor
  Serial1.println("Setup...");

  //Perform software reset:
  emon_ic.InitEnergyIC();

  Serial1.println("CS5464 Ready!!");

  // Print the configuration register, to confirm communication with the CS5464
  unsigned long check = emon_ic.ReadRegister(CONFIG); //read the config register.
  Serial1.print("Config = ");
  Serial1.println(check, HEX);
}

// Main loop
void loop() {
  delay(1000);
  //example of reading data
  // unsigned long voltage = SPI_read(0b00001110); //read Register 7 Instantaneous Current Channel 2
  // TODO: Verify Peak Current register datasheet suggests PEAK_CUR2 = 0x16
  unsigned long peak_current = emon_ic.ReadRegister(PEAK_CUR2<<1); //read Register 22 Peak Current Channel 2
  peak_current = peak_current >> 8;
  Serial1.print("Peak Current = ");
  Serial1.println(peak_current);

  // check if the device has accidentally reset
  // Found that noise from switching AC loads can cause the CS5464 to lock-up.
  // need to improve power or signal filtering, but this is a patch for initial testing.
  if (peak_current == 0) { // If we get a 0 reading, then reconfigure the device
    emon_ic.WriteCommand(CONV_CONT); //Set continuous conversion mode
    delay(1000);
    unsigned long check = emon_ic.ReadRegister(CONFIG); //read the config register.
    Serial1.print("Config = ");
    Serial1.println(check, HEX);
  }
}