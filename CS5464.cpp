/******************************************************** */
/* CS5464 Initialization and object oriented chip access  */
/* The CS5464 Energy Monitor Object allows direct reading */
/* of measurement parameters                              */
/**********************************************************/
#include <CS5464.h>
#include <SPI.h>

union FourByte {
    struct
    {
        unsigned long value : 24; //24bit register values go in here
        uint8_t command : 8;      //8bit command goes in here.
    };
    uint8_t bit8[4]; //this is just used for efficient conversion of the above into 4 bytes.
};

CS5464::CS5464(int pin)
{
    _cs = pin;
}

void CS5464::InitEnergyIC()
{
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE3); //I believe it to be Mode3
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    pinMode(_cs, OUTPUT); // Do this manually, as the SPI library doesn't work right for all versions.
    digitalWrite(_cs, HIGH);

    //Perform software reset:
    WriteCommand(SOFT_RESET);
    delay(2000);
    unsigned long status;
    do
    {
        // TODO: Verify datasheet for status register STATUS = 0x0F
        status = ReadRegister(EM_STATUS << 1); //read the status register
        status &= (1UL << 23);
    } while (!status);
}

void CS5464::WriteCommand(uint8_t command)
{
    digitalWrite(_cs, LOW);         //SS goes low to mark start of transmission
    union FourByte data = {0xFEFEFE, command}; //generate the data to be sent, i.e. your command plus the Sync bytes.
    for (int i = 3; i >= 0; i--)
    {
        SPI.transfer(data.bit8[i]); //transfer all 4 bytes of data - command first, then Big Endian transfer of the 24bit value.
    }
    digitalWrite(_cs, HIGH);
}

// Read a register from the CS5464 - just supply a command byte (see the datasheet)
unsigned long CS5464::ReadRegister(uint8_t reg)
{
    digitalWrite(_cs, LOW);                     //SS goes low to mark start of transmission
    union FourByte data = {0xFEFEFE, reg}; //generate the data to be sent, i.e. your command plus the Sync bytes.
    for (int i = 3; i >= 0; i--)
    {
        data.bit8[i] = SPI.transfer(data.bit8[i]); //send the data whilst reading in the result
    }
    digitalWrite(_cs, HIGH); //SS goes high to mark end of transmission
    return data.value;       //return the 24bit value recieved.
}