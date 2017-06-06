#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include <Keypad.h>

#define ROWS 4
#define COLS 4
#define NO_KEY_PRESSED 0

// Map of hexa keyboard used by CHIP-8
static const char hexaKeys[ROWS][COLS] = {
    {0x1, 0x2, 0x3, 0xA},
    {0x4, 0x5, 0x6, 0xB},
    {0x7, 0x8, 0x9, 0xC},
    {0xE, 0x0, 0xF, 0xD}
};

/**
 * Class for keyboard controlling and mapping
 */
class Keyboard {
    private:
        Keypad keypad;
        char key = 0;
        
    public:
        /**
         * Default constructor.
         */
        Keyboard(byte rowPins[], byte colPins[]);

        /**
         * Gets currently pressed key.
         * 
         * @return Currently pressed key
         */
        char getKeyPressed();
};

#endif
