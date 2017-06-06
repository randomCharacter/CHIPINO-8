#include "cpu.h"

CPU::CPU(Memory memory, Screen screen, Keyboard keyboard, Speaker speaker, int seed) : memory(memory), screen(screen), keyboard(keyboard), speaker(speaker), seed(seed) {
    reset();
    pinMode(seed, INPUT);
    randomSeed(analogRead(seed));
    lastMillis = millis() + TIMER_DELAY;
}

void CPU::reset() {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        regV[i] = 0x0;
    }
    pc = PC_START;
    regStack = STACK_START;
    regI = 0;
    timerDelay = 0;
    timerSound = 0;
}

void CPU::decrementTimers() {
    int t = millis();
    if (lastMillis < t) {
        lastMillis = t + TIMER_DELAY;
        if (timerSound > 0) {
            timerSound--;
            speaker.playSound();
        } else {
            speaker.muteSound();
        }
        if (timerDelay > 0) {
            timerDelay--;
        }
    }
}

void CPU::executeNextCommand() {
    byte opcode1 = memory.getByte(pc);
    int opcode = (int)opcode1 << 8;
    byte opcode2 = memory.getByte(pc + 1);
    //
    opcode += opcode2;
    pc += 2;
    execute(opcode);
    decrementTimers();
}

void CPU::execute(int opcode) {
    int command = (opcode & 0xF000) >> 12;
    int reg, reg1, reg2, val, location;
    switch (command) {
        case 0x0:
            switch (opcode & 0x00FF) {
                case 0xE0: // Clears the screen.
                    clearScreen();
                    break;
                case 0xEE: // Returns from a subroutine.
                    returnFromSubrutine();
                    break;
            }
            break;
        case 0x1: // Jumps to address.
            location = opcode & 0x0FFF;
            jumpToAddress(location);
            break;
        case 0x2: // Calls subroutine.
            location = opcode & 0x0FFF;
            callSubroutine(location);
            break;
        case 0x3: // Skips the next instruction if register equals value.
            reg = (opcode & 0x0F00) >> 8;
            val = (opcode & 0x00FF);
            skipIfRegisterEqualValue(reg, val);
            break;
        case 0x4: // Skips the next instruction if register doesn't equal value.
            reg = (opcode & 0x0F00) >> 8;
            val = (opcode & 0x00FF);
            skipIfRegisterNotEqualValue(reg, val);
            break;
        case 0x5: // Skips the next instruction if register equals register.
            reg1 = (opcode & 0x0F00) >> 8;
            reg2 = (opcode & 0x00F0) >> 4;
            skipIfRegisterEqualRegister(reg1, reg2);
            break;
        case 0x6: // Sets register to a value.
            reg = (opcode & 0x0F00) >> 8;
            val = (opcode & 0x00FF);
            setRegisterToValue(reg, val);
            break;
        case 0x7: // Adds value to register.
            reg = (opcode & 0x0F00) >> 8;
            val = (opcode & 0x00FF);
            addValueToRegister(reg, val);
            break;
        case 0x8:
            reg1 = (opcode & 0x0F00) >> 8;
            reg2 = (opcode & 0x00F0) >> 4;
            switch (opcode & 0x000F) {
                case 0x0: // Sets reg1 to the value of reg2.
                    registerMove(reg1, reg2);
                    break;
                case 0x1: // Sets reg1 to the value of reg1 or reg2.
                    registerOr(reg1, reg2);
                    break;
                case 0x2: // Sets reg1 to the value of reg1 and reg2.
                    registerAnd(reg1, reg2);
                    break;
                case 0x3: // Sets reg1 to the value of reg1 xor reg2.
                    registerXor(reg1, reg2);
                    break;
                case 0x4: // Adds reg2 to reg1. VF is set to 1 when there's a carry, and to 0 when there isn't.
                    registerAdd(reg1, reg2);
                    break;
                case 0x5: // reg2 is subtracted from reg1. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    registerSubN(reg1, reg2);
                    break;
                case 0x6: // Shifts reg1 right by one. VF is set to the value of the least significant bit of reg1 before the shift.
                    registerShiftRight(reg1);
                    break;
                case 0x7: // Sets reg1 to reg2 minus reg1. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    registerSub(reg1, reg2);
                    break;
                case 0xE: // Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
                    registerShiftLeft(reg1);
                    break;
            }
            break;
        case 0x9: // Skips the next instruction if register doesn't equal register.
            reg1 = (opcode & 0x0F00) >> 8;
            reg2 = (opcode & 0x00F0) >> 4;
            skipIfRegisterNotEqualRegister(reg1, reg2);
            break;
        case 0xA: // Sets I to the address.
            location = (opcode & 0x0FFF);
            setIToAddress(location);
            break;
        case 0xB: // Jumps to the address plus V0.
            location = (opcode & 0x0FFF);
            jumpToAddressPlusV0(location);
            break;
        case 0xC: // Sets register to the result of a bitwise and operation on a random number and value.
            reg = (opcode & 0x0F00) >> 8;
            val = (opcode & 0x00FF);
            setRegisterToRandomValue(reg, val);
            break;
        case 0xD: // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
            reg1 = (opcode & 0x0F00) >> 8;
            reg2 = (opcode & 0x00F0) >> 4;
            val = (opcode & 0x000F);
            //screen.displayText("Dosao!");
            drawSprite(reg1, reg2, val);
            break;
        case 0xE:
            switch (opcode & 0x00FF) {
                case 0x9E: // Skips the next instruction if the key stored in register is pressed.
                    reg = (opcode & 0x0F00) >> 8;
                    skipIfKeyPressed(reg);
                    break;
                case 0xA1: // Skips the next instruction if the key stored in register isn't pressed.
                    reg = (opcode & 0x0F00) >> 8;
                    skipIfKeyNotPressed(reg);
                    break;
            }
            break;
        case 0xF:
            reg = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x07: // Sets register to the value of the delay timer.
                    setRegisterToDelayTimer(reg);
                    break;
                case 0x0A: // A key press is awaited, and then stored in register.
                    waitForKey(reg);
                    break;
                case 0x15: // Sets the delay timer to register value.
                    setDelayTimer(reg);
                    break;
                case 0x18: // Sets the sound timer to register value.
                    setSoundTimer(reg);
                    break;
                case 0x1E: // Adds VX to I. [3]<- Wikipedia
                    addRegisterToI(reg);
                    break;
                case 0x29: // Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    loadIWithSprite(reg);
                    break;
                case 0x33: // Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.
                    storeDecimalInMemory(reg);
                    break;
                case 0x55: // Stores V0 to VX (including VX) in memory starting at address I.[4]
                    storeRegistersInMemory(reg);
                    break;
                case 0x65: // Fills V0 to VX (including VX) with values from memory starting at address I. [4]
                    storeMemoryToRegisters(reg);
                    break;
            }
            break;
    }
}

void CPU::run() {
    if (timerDelay == 0) {
        executeNextCommand();
    }
    decrementTimers();
    /*  if (millis() - lastMillis > 1000) {
        decrementTimers();
        lastMillis += 1000;
        }*/
    //delay(CPU_DELAY);
}

// 0x0XXX
void CPU::clearScreen() {
    screen.clear();
}

void CPU::returnFromSubrutine() {
    regStack--;
    pc = memory.getByte(regStack) << 8;
    regStack--;
    pc += memory.getByte(regStack);
}

// 0x1XXX
void CPU::jumpToAddress(int location) {
    pc = location;
}

// 0x2XXX
void CPU::callSubroutine(int location) {
    memory.setByte(regStack, (pc & 0x00FF));
    regStack++;
    memory.setByte(regStack, ((pc & 0xFF00) >> 8));
    regStack++;
    pc = location;
}

// 0x3XXX
void CPU::skipIfRegisterEqualValue(int reg, int val) {
    if (regV[reg] == (byte)val) {
        pc += 2;
    }
}

// 0x4XXX
void CPU::skipIfRegisterNotEqualValue(int reg, int val) {
    if (regV[reg] != val) {
        pc += 2;
    }
}

// 0x5XXX
void CPU::skipIfRegisterEqualRegister(int reg1, int reg2) {
    if (regV[reg1] == regV[reg2]) {
        pc += 2;
    }
}

// 0x6XXX
void CPU::setRegisterToValue(int reg, int val) {
    regV[reg] = (byte)val;
}

// 0x7XXX
void CPU::addValueToRegister(int reg, int val) {
    regV[reg] += (byte)val;
}

// 0x8XXX
void CPU::registerMove(int reg1, int reg2) {
    regV[reg1] = regV[reg2];
}

void CPU::registerOr(int reg1, int reg2) {
    regV[reg1] |= regV[reg2];
}

void CPU::registerAnd(int reg1, int reg2) {
    regV[reg1] &= regV[reg2];
}

void CPU::registerXor(int reg1, int reg2) {
    regV[reg1] ^= regV[reg2];
}

void CPU::registerAdd(int reg1, int reg2) {
    if (regV[reg1] + regV[reg2] < regV[reg2] || regV[reg1] + regV[reg2] < regV[reg1]) {
        regV[0xF] = 0x01;
    } else {
        regV[0xF] = 0x00;
    }
    regV[reg1] += regV[reg2];
}

void CPU::registerSubN(int reg1, int reg2) {
    if (regV[reg1] > regV[reg2]) {
        regV[0xF] = 0x01;
    } else {
        regV[0xF] = 0x00;
    }
    regV[reg1] -= regV[reg2];
}

void CPU::registerShiftRight(int reg) {
    regV[0xF] = regV[reg] & 0x01;
    regV[reg] = regV[reg] >> 1;
}

void CPU::registerSub(int reg1, int reg2) {
    if (regV[reg1] < regV[reg2]) {
        regV[0xF] = 0x01;
    } else {
        regV[0xF] = 0x00;
    }
    regV[reg1] -= regV[reg2];
}

void CPU::registerShiftLeft(int reg) {
    regV[0xF] = regV[reg] & 0x80;
    regV[reg] <<= 1;
}

// 0x9XXX
void CPU::skipIfRegisterNotEqualRegister(int reg1, int reg2) {
    if (regV[reg1] != regV[reg2]) {
        pc += 2;
    }
}

// 0xAXXX
void CPU::setIToAddress(int location) {
    regI = location;
}

// 0xBXXX
void CPU::jumpToAddressPlusV0(int location) {
    //pc = (location + regI) & 0x0FFF;
    pc = (location + regV[0x0]) & 0x0FFF;
}

// 0xCXXX
void CPU::setRegisterToRandomValue(int reg, int val) {
    regV[reg] = (byte) (val & random(0xFF));
}

// 0xDXXX
void CPU::drawSprite(int reg1, int reg2, int val) {
    regV[0xF] = 0;

    for (int j = 0; j < val; j++) {

        byte colorByte = memory.getByte(regI + j);
        int y = regV[reg2] + j;
        y = y % screen.getHeight();

        int mask = 0x80;

        for (int i = 0; i < 8; i++) {
            int x = regV[reg1] + i;
            x = x % screen.getWidth();

            bool turnOn = (colorByte & mask) != 0;
            bool currentOn = screen.isPixelOn(x, y);

            if (turnOn && currentOn) {
                regV[0xF] |= 1;
                turnOn = false;
            } else if (!turnOn && currentOn) {
                turnOn = true;
            }

            screen.markPixel(x, y, turnOn);
            mask = mask >> 1;
        }
    }
    screen.show();
}

// 0xEXXX
void CPU::skipIfKeyPressed(int reg) {
    if (keyboard.getKeyPressed() == regV[reg]) {
        pc += 2;
    }
}

void CPU::skipIfKeyNotPressed(int reg) {
    if (keyboard.getKeyPressed() != regV[reg]) {
        pc += 2;
    }
}

// 0xFXXX
void CPU::setRegisterToDelayTimer(int reg) {
    regV[reg] = timerDelay;
}

void CPU::waitForKey(int reg) {
    byte key = keyboard.getKeyPressed();
    while (key == NO_KEY_PRESSED) {
        delay(KEY_DELAY);
        key = keyboard.getKeyPressed();
    }

    regV[reg] = key;
}

void CPU::setDelayTimer(int reg) {
    timerDelay = regV[reg];
}

void CPU::setSoundTimer(int reg) {
    timerSound = regV[reg];
}

void CPU::addRegisterToI(int reg) {
    regI = regI + regV[reg];
}

void CPU::loadIWithSprite(int reg) {
    regI = regV[reg] * 5;
}

void CPU::storeDecimalInMemory(int reg) {
    memory.setByte(regI, (regV[reg] / 100));
    memory.setByte(regI + 1, ((regV[reg] % 100) / 10));
    memory.setByte(regI + 2, ((regV[reg] % 100) % 10));
}

void CPU::storeRegistersInMemory(int reg) {
    for (int i = 0; i <= reg; i++) {
        memory.setByte(regI + i, regV[i]);
    }
}

void CPU::storeMemoryToRegisters(int reg) {
    for (int i = 0; i <= reg; i++) {
        regV[i] = memory.getByte(regI + i);
    }
}

