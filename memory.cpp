#include "memory.h"

Memory::Memory(int pinSD, byte *memory) : pinSD(pinSD), memory(memory) {
    loadFonts();
    romLoaded = false;
}

void Memory::clearMemory() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0x0;
    }
}

void Memory::loadFonts() {
    const byte smallFont[] = {
        0xf9, 0x99, 0xf2, 0x62, 0x27,
        0xf1, 0xf8, 0xff, 0x1f, 0x1f,
        0x99, 0xf1, 0x1f, 0x8f, 0x1f,
        0xf8, 0xf9, 0xff, 0x12, 0x44,
        0xf9, 0xf9, 0xff, 0x9f, 0x1f,
        0xf9, 0xf9, 0x9e, 0x9e, 0x9e,
        0xf8, 0x88, 0xfe, 0x99, 0x9e,
        0xf8, 0xf8, 0xff, 0x8f, 0x88
    };

    for (int i = 0; i < 40; i++) {
        setByte(i * 2, smallFont[i] & 0xf0); // First hex
        setByte(i * 2 + 1, smallFont[i] << 4); // Second hex
    }

    const byte superFont[] = {
        0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C,
        0x18, 0x38, 0x58, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C,
        0x3E, 0x7F, 0xC3, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xFF, 0xFF,
        0x3C, 0x7E, 0xC3, 0x03, 0x0E, 0x0E, 0x03, 0xC3, 0x7E, 0x3C,
        0x06, 0x0E, 0x1E, 0x36, 0x66, 0xC6, 0xFF, 0xFF, 0x06, 0x06,
        0xFF, 0xFF, 0xC0, 0xC0, 0xFC, 0xFE, 0x03, 0xC3, 0x7E, 0x3C,
        0x3E, 0x7C, 0xC0, 0xC0, 0xFC, 0xFE, 0xC3, 0xC3, 0x7E, 0x3C,
        0xFF, 0xFF, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x60, 0x60,
        0x3C, 0x7E, 0xC3, 0xC3, 0x7E, 0x7E, 0xC3, 0xC3, 0x7E, 0x3C,
        0x3C, 0x7E, 0xC3, 0xC3, 0x7F, 0x3F, 0x03, 0x03, 0x3E, 0x7C
    };

    for(int i = 0; i < 100; i++) {
        setByte(0x50 + i, superFont[i]);
    }
}

byte Memory::getByte(int location) {
    if (location < MEMORY_SIZE)
        return memory[location];
    else return 0;
}

void Memory::setByte(int location, byte value) {
    if (location < MEMORY_SIZE)
        memory[location] = value;
}

bool Memory::initialize() {
    if (!SD.begin(pinSD)) {
        return false;
    }
    return true;
}

bool Memory::loadRom(String romName) {
    File rom = SD.open(romName, FILE_READ);
    if (!rom) {
        return false;
    }

    int i = ROM_OFFSET;

    while (rom.available()) {
        setByte(i++, rom.read());
    }
    rom.close();
    romLoaded = true;
    return true;

}

/*
    bool Memory::loadRom(String romName) {
    const byte ALONSY[] = {
        0xa2, 0x1e, 0xc2, 0x01, 0x32, 0x01, 0xa2, 0x1a, 0xd0, 0x14, 0x70, 0x04, 0x30, 0x40, 0x12, 0x00,
        0x60, 0x00, 0x71, 0x04, 0x31, 0x20, 0x12, 0x00, 0x12, 0x18, 0x80, 0x40, 0x20, 0x10, 0x20, 0x40,
        0x80, 0x10
    };
    romLoaded = true;
    for (int i = 0; i < 34; i++) {
        setByte(i + 0x200, ALONSY[i]);
        if (memory[ROM_OFFSET + i] != ALONSY[i])
            romLoaded = false;
    }


    return true;

    }*/

void Memory::closeRom() {
    clearMemory();
    romLoaded = false;
}

bool Memory::isRomLoaded() {
    return romLoaded;
}

