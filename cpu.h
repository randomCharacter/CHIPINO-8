#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "memory.h"
#include "screen.h"
#include "keyboard.h"
#include "speaker.h"

// Number of registers
#define NUM_REGISTERS 16
// Memory location that rom starts at
#define PC_START 0x200
// Memory location of begining of stack
#define STACK_START 0x52

// Time to wait before checking for key press/release (ms)
#define KEY_DELAY 30
// Time for decrementing timers (ms)
#define TIMER_DELAY 20

/**
 * Emulates CHIP-8 CPU
 */
class CPU {
    private:
        Memory memory;
        Screen screen;
        Keyboard keyboard;
        Speaker speaker;

        // Pin for for RNG
        int seed;
        
        // Registers
        byte regV[NUM_REGISTERS];
        int regI;
        int regStack;

        // Timers
        byte timerDelay;
        byte timerSound;

        // Program counter
        int pc = PC_START;

        // State of millis() when last time calculated
        int lastMillis;
        
    public:
        /**
         * Default constructor for the class.
         *
         * @param memory The instance of Memory to be used
         * @param screen The instance of Screen
         * @param keyboard The instance of Keyboard to be used
         * @param speaker The instance of Speaker to be used
         * @param seed The number of pin to be used for RNG
         */
        CPU(Memory memory, Screen screen, Keyboard keyboard, Speaker speaker, int seed);

        /**
        * Resets the Arduino.
        */
        void reset();
        
        /**
         * Decrements delay and sound timers.
         */
        void decrementTimers();
        
        /**
         * Reads command from the memory, executes it
         * and increases program counter so that next
         * command can be run.
         */
        void executeNextCommand();
        
        /**
         * Executes given opcode.
         *
         * @param opcode opcode to be executed
         */
        void execute(int opcode);

        /**
         * Runs the code in loop.
         */
        void run();

        // 0x0XXX opcode commands

        /**
         * Clears the screen.
         */
        void clearScreen();

        /**
         * Returns from subroutine.
         */
        void returnFromSubrutine();

        // 0x1XXX opcode commands

        /**
         * Jumps to given address location.
         *
         * @param location Location to be jumped to
         */
        void jumpToAddress(int location);


        // 0x2XXX opcode commands

        /**
         * Calls subroutine at given address location.
         *
         * @param location Location subroutine will be called from
         */
        void callSubroutine(int location);

        // 0x3XXX opcode commands

        /**
         * Skips the next instruction if register is equal to value.
         *
         * @param reg Number of register
         * @param val Value to be compared to
         */
        void skipIfRegisterEqualValue(int reg, int val);

        // 0x4XXX opcode commands

        /**
         * Skips the next instruction if register is not equal to value.
         *
         * @param reg Number of register
         * @param val Value to be compared to
         */
        void skipIfRegisterNotEqualValue(int reg, int val);

        // 0x5XXX opcode commands

        /**
         * Skips the next instruction if both registers have same value.
         *
         * @param reg1 Number of first register
         * @param reg2 Number of second register
         */
        void skipIfRegisterEqualRegister(int reg1, int reg2);

        // 0x6XXX opcode commands

        /**
         * Sets register to given value.
         *
         * @param reg Number of register
         * @param val Value to be set to
         */
        void setRegisterToValue(int reg, int val);

        // 0x7XXX opcode commands

        /**
         * Add given value to the register.
         *
         * @param reg Number of register
         * @param val Value to be added
         */
        void addValueToRegister(int reg, int val);

        // 0x8XXX opcode commands

        /**
         * Sets first register to the value of second.
         *
         * @param reg1 First register, stores value
         * @param reg2 Second register
         */
        void registerMove(int reg1, int reg2);

        /**
         * Calculate value of logical or of two registers.
         * Store result in first.
         *
         * @param reg1 First register, stores value
         * @param reg2 Second register
         */
        void registerOr(int reg1, int reg2);

        /**
         * Calculate value of logical and of two registers.
         * Store result in first.
         *
         * @param reg1 First register, stores value
         * @param reg2 Second register
         */
        void registerAnd(int reg1, int reg2);

        /**
         * Calculate value of logical xor of two registers.
         * Store result in first register.
         *
         * @param reg1 First register, stores value
         * @param reg2 Second register
         */
        void registerXor(int reg1, int reg2);

        /**
         * Add values of second register to the value of first.
         * Stores result in first register.
         * VF is set to 1 when there's a carry,
         * and to 0 when there isn't.
         *
         * @param reg1 First register number, stores value
         * @param reg2 Second register number
         */
        void registerAdd(int reg1, int reg2);

        /**
         * Subtracts value of second register from the value of first.
         * Stores result in first register.
         * VF is set to 0 when there's a borrow,
         * and 1 when there isn't.
         *
         * @param reg1 First register number, stores value
         * @param reg2 Second register number
         */
        void registerSubN(int reg1, int reg2);

        /**
         * Shifts register right by one.
         * VF is set to the value of the least significant
         * bit of VX before the shift.
         *
         * @param reg Number of register to be shifted
         */
        void registerShiftRight(int reg);

        /**
         * Subtracts value of second register from the value of first.
         * Stores result in first register.
         * VF is set to 1 when there's a borrow,
         * and 0 when there isn't.
         *
         * @param reg1 First register number, stores value
         * @param reg2 Second register number
         */
        void registerSub(int reg1, int reg2);

        /**
         * Shifts register left by one.
         * VF is set to the value of the least significant
         * bit of VX before the shift.
         *
         * @param reg Number of register to be shifted
         */
        void registerShiftLeft(int reg);

        // 0x9XXX opcode commands

        /**
         * Skips the next instruction if value of first register
         * is not equal to the value of second.
         *
         * @param reg1 Number of first register
         * @param reg2 Number of second register
         */
        void skipIfRegisterNotEqualRegister(int reg1, int reg2);

        // 0xAXXX opcode commands

        /**
         * Sets index register to given location.
         *
         * @param location Emulator.Memory location
         */
        void setIToAddress(int location);

        // 0xBXXX opcode commands

        /**
         * Jumps to the given location plus the value
         * of register V0.
         *
         * @param location Base memory location
         */
        void jumpToAddressPlusV0(int location);

        // 0xCXXX opcode commands

        /**
         * Sets register to the result of a bitwise and
         * operation on a random number and given value.
         *
         * @param reg Number of register
         * @param val Given value
         */
        void setRegisterToRandomValue(int reg, int val);

        // 0xDXXX opcode commands

        /**
         * Draws a sprite at coordinate (register 1, register 2) that
         * has a width of 8 pixels and a height given by value.
         * Each row of 8 pixels is read as bit-coded starting from memory location I;.
         * I value does not change after the execution of this instruction.
         * As described above, VF is set to 1 if any screen pixels are flipped
         * from set to unset when the sprite is drawn, and to 0 if that does not happen.
         *
         * @param reg1 Number of register holding first coordinate
         * @param reg2 Number of register holding second coordinate
         * @param val Value of height
         */
        void drawSprite(int reg1, int reg2, int val);

        // 0xEXXX opcode commands

        /**
         * Skips the next instruction if key stored in register is pressed.
         *
         * @param reg Number of register to check for key
         */
        void skipIfKeyPressed(int reg);

        /**
         * Skips the next instruction if key stored in register is not pressed.
         *
         * @param reg Number of register to check for key
         */
        void skipIfKeyNotPressed(int reg);

        // 0xFXXX opcode commands
         
        /**
         * Sets register value to the value of the delay timer.
         *
         * @param reg Number of register
         */
        void setRegisterToDelayTimer(int reg);

        /**
         * Sets register value to the value of the delay timer.
         *
         * @param reg Number of register
         */
        void waitForKey(int reg);

        /**
         * Sets the value of delay timer to the
         * value stored in register.
         *
         * @param reg Number of register
         */
        void setDelayTimer(int reg);

        /**
         * Sets the value of sound timer to the
         * value stored in register.
         *
         * @param reg Number of register
         */
        void setSoundTimer(int reg);

        /**
         * Adds register value to the value of index register.
         * Result is stored in index register.
         *
         * @param reg Number of register
         */
        void addRegisterToI(int reg);

        /**
         * Sets index register to the location of the sprite for the
         * character in register. Characters 0-F (in hexadecimal)
         * are represented by a 4x5 font.
         *
         * @param reg Number of register
         */
        void loadIWithSprite(int reg);

        /**
         * Stores the binary-coded decimal representation of VX,
         * with the most significant of three digits at the address in I,
         * the middle digit at I plus 1,
         * and the least significant digit at I plus 2.
         *
         * @param reg Number of register
         */
        void storeDecimalInMemory(int reg);

        /**
         * Stores values of all V register up to the given one
         * to the memory starting from location in index register.
         *
         * @param reg Number of register
         */
        void storeRegistersInMemory(int reg);

        /**
         * Reads values from location stored in index register
         * and stores it in registers uo to given one.
         *
         * @param reg Number of register
         */
        void storeMemoryToRegisters(int reg);
};

#endif
