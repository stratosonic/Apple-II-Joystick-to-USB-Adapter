/* 
 * EEPROM routines
 * www.flitey.com
 */

#ifndef EEPROM_H
#define	EEPROM_H

/**
 * Gets the value in EEPROM at the specified address.
 * @param byteAddress the address of the byte
 * @return the value
 */
unsigned char EepromReadByte(uint8_t byteAddress);

/**
 * Sets the value in EEPROM with the specified value at the specified address.
 * @param byteAddress the address location where the value will be stored
 * @param byteData the value to store
 */
void EepromWriteByte(uint8_t byteAddress, uint8_t byteData);

/**
 * Gets two bytes from EEPROM starting at the specified address. 
 * @param startAddress the starting address location
 * @return the two byte value
 */
uint16_t EepromReadTwoBytes(uint8_t startAddress);

/**
 * Sets the value in EEPROM with the specified value at the specified address
 * @param startAddress the starting address location where the value will be stored
 * @param value the value to store
 */
void EepromWriteTwoBytes(uint8_t startAddress, uint16_t value);

#endif	/* EEPROM_H */

