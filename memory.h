#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

typedef unsigned char byte;

#define MEMORY_SIZE 0x1000
#define ROM_OFFSET 0x200

/**
 * Controls memory usage.
 */
class Memory {
    private:
        // Number of pin SD card is connected to
        int pinSD;
        // Byte array representig CHIP-8 memory
        byte *memory;
        // ROM loaded indicator
        bool romLoaded;

        /**
         * Closes the ROM and clears the memory for next one.
         */
        void clearMemory();
        
        /**
         * Loads fonts into the memory.
         */
        void loadFonts();
        
    public:
        /**
         * Default constructor for Emulator.Memory object.
         * 
         * @param pinSD Number of pin SD card is connected to
         * @param memory Byte array representig memory
         */
        Memory(int pinSD, byte* memory);

        /**
         * Gets the byte from given location
         *
         * @param location Memory location to read from
         * @return Content of given memory location
         */
        byte getByte(int location);

        /**
         * Sets the byte at given location to given value.
         *
         * @param location Memory location to be written to
         * @param value Value to be written to given location
         */
        void setByte(int location, byte value);

        /**
         * Initializes the SD card.
         * 
         * @return @return <code>true</code> if operation is successful, <code>false</code> otherwise
         */
        bool initialize();

        /**
         * Loads the ROM into the memory.
         *
         * @param romName Name/path of rom on the SD card
         * @return <code>true</code> if operation is successful, <code>false</code> otherwise
         */
        bool loadRom(String romName);

        /**
         * Closes the ROM and clears the memory for next one.
         */
        void closeRom();

        /**
         * @return <code>true</code> if ROM is loaded, <code>false</code> otherwise
         */
        bool isRomLoaded();
        
};

#endif
