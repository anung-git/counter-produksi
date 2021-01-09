#ifndef Eprom_H_
#define Eprom_H_
#include <Wire.h>
#include <Arduino.h>
class Eprom {
private:
    /* data */
public:
    Eprom(/* args */);
    ~Eprom();
    // void writeByteWD( int deviceaddress, unsigned int eeaddress, uint8_t data );
    void writeByte( int deviceaddress, unsigned int eeaddress, uint8_t data );
    void writeInt( int deviceaddress, unsigned int eeaddress, uint16_t data );
    void write_page( int deviceaddress, unsigned int eeaddresspage, uint8_t* data, uint8_t length ) ;
    char readByte( int deviceaddress, unsigned int eeaddress );
    uint16_t readInt( int deviceaddress, unsigned int eeaddress );
    void readBuffer( int deviceaddress, unsigned int eeaddress, uint8_t *buffer, int length );
    void floatToByte(float pecah);


};
#endif