//  ** updated 05/07/2019 **
// This is written for an Arduino Leonardo.
// - it uses Serial1 instead of Serial, if you change it to Serial it should work for an UNO or other boards
//
// For CS5464 chip from Cirrus
// Read the peak current on channel I2
// Check for zero value coming back from CS5464, if so, then reset the device
// Original Code from : https://corgitronics.com/2014/06/30/using-the-cirrus-logic-cs5464-for-ac-current-measurement/

#include <SPI.h>
#define Serial1 Serial

// Pin configurations
// set pin 12 as the slave select for the digital pot:
const int slaveSelectPin = 5;

// set pin 11 as the Reset pin for the CS5464:
const int resetPin = 16;

// Create a data type for the 4 Byte data and command packet
union FourByte {
  struct {
    unsigned long value: 24; //24bit register values go in here
    byte command: 8; //8bit command goes in here.
  };
  byte bit8[4]; //this is just used for efficient conversion of the above into 4 bytes.
};

// Initialize SPI and pins
void setup() {
  // reset the CS5464 device
  pinMode(resetPin, OUTPUT);
  delay(100);
  digitalWrite(resetPin, LOW);
  delay(100);
  digitalWrite(resetPin, HIGH);
  delay(100);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3); //I believe it to be Mode3
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  pinMode(slaveSelectPin, OUTPUT); // Do this manually, as the SPI library doesn't work right for all versions.
  digitalWrite(slaveSelectPin, HIGH);

  Serial1.begin(9600);
  delay(5000); // Pause allowing time to open the serial monitor
  Serial1.println("Setup...");

  //Perform software reset:
  SPI_writeCommand(0x80);
  delay(2000);
  unsigned long status;
  do {
    status = SPI_read(0b00011110); //read the status register
    status &= (1UL << 23);
    Serial1.print(".");
  } while (!status);

  Serial1.println("CS5464 Ready!!");

  // SPI_writeCommand(0xE0); //Set single conversion mode
  SPI_writeCommand(0xE8); //Set continuous conversion mode

  // Print the configuration register, to confirm communication with the CS5464
  unsigned long check = SPI_read(0x00); //read the config register.
  Serial1.print("Config = ");
  Serial1.println(check, HEX);

  /*
    //example of writing a register
    union FourByte data;
    data.command = 0b01000000; //Write to config register
    data.value = 1; //This is the default value from datasheet, just using it as an example.
    SPI_writeRegister(data);
  */
}

// Main loop
void loop() {
  delay(1000);
  //example of reading data
  // unsigned long voltage = SPI_read(0b00001110); //read Register 7 Instantaneous Current Channel 2
  unsigned long peak_current = SPI_read(0b00101100); //read Register 22 Peak Current Channel 2
  peak_current = peak_current >> 8;
  Serial1.print("Peak Current = ");
  Serial1.println(peak_current);

  // check if the device has accidentally reset
  // Found that noise from switching AC loads can cause the CS5464 to lock-up.
  // need to improve power or signal filtering, but this is a patch for initial testing.
  if (peak_current == 0) { // If we get a 0 reading, then reconfigure the device
    SPI_writeCommand(0xE8); //Set continuous conversion mode
    delay(1000);
    unsigned long check = SPI_read(0x00); //read the config register.
    Serial1.print("Config = ");
    Serial1.println(check, HEX);
  }
}

// Send a Command to the CS5464 - see the data sheet for commands
void SPI_writeCommand(byte command) {
  digitalWrite(slaveSelectPin, LOW); //SS goes low to mark start of transmission
  union FourByte data = {0xFEFEFE, command}; //generate the data to be sent, i.e. your command plus the Sync bytes.
  // Serial1.print("SPI_writeCommand");
  for (int i = 3; i >= 0; i--) {
    SPI.transfer(data.bit8[i]); //transfer all 4 bytes of data - command first, then Big Endian transfer of the 24bit value.
    // Serial1.print(i);
    // Serial1.print(data.bit8[i], HEX);
  }
  // Serial1.println();
  digitalWrite(slaveSelectPin, HIGH);
}

// Read a register from the CS5464 - just supply a command byte (see the datasheet)
unsigned long SPI_read(byte command) {
  digitalWrite(slaveSelectPin, LOW); //SS goes low to mark start of transmission
  union FourByte data = {0xFEFEFE, command}; //generate the data to be sent, i.e. your command plus the Sync bytes.
  // Serial1.print("SPI_Read");
  for (int i = 3; i >= 0; i--) {
    data.bit8[i] = SPI.transfer(data.bit8[i]); //send the data whilst reading in the result
    // Serial1.print(data.bit8[i], HEX);
  }
  // Serial1.println();
  digitalWrite(slaveSelectPin, HIGH); //SS goes high to mark end of transmission
  return data.value; //return the 24bit value recieved.
}