/* 
 * EEPROM routines
 * www.flitey.com
 */

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

unsigned char EepromReadByte(uint8_t byteAddress) {
    EEADR = byteAddress;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    Nop(); //Nop may be required for latency at high frequencies
    Nop(); //Nop may be required for latency at high frequencies
    return (EEDATA); 
}

void EepromWriteByte(uint8_t byteAddress, uint8_t byteData) {
    bool GIE_BIT_VAL;

    EEADR = byteAddress;
    EEDATA = byteData;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    GIE_BIT_VAL = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    EECON2 = 0x55; // critical unlock sequence
    EECON2 = 0xAA;
    EECON1bits.WR = 1; // end critical sequence
    while (EECON1bits.WR); //Wait till the write completion
    INTCONbits.GIE = GIE_BIT_VAL;
    EECON1bits.WREN = 0;
}

uint16_t EepromReadTwoBytes(uint8_t startAddress) {
    unsigned char highByte = EepromReadByte(startAddress);
    unsigned char lowByte = EepromReadByte(startAddress + 1);
    uint16_t result = (highByte << 8) | lowByte;
    return result;
}

void EepromWriteTwoBytes(uint8_t startAddress, uint16_t value) {
    uint8_t high = (value & 0xFF00) >> 8;
    uint8_t low = (value & 0x00FF);

    EepromWriteByte(startAddress, high);
    EepromWriteByte(startAddress + 1, low);
}