/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License.
 If not, see <http://www.gnu.org/licenses/>.
 */

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

