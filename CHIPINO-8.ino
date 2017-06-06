#include <SPI.h>
#include <SD.h>

#include "screen.h"
#include "keyboard.h"
#include "memory.h"
#include "speaker.h"
#include "cpu.h"

// Pins display is connected to
#define OLED_MOSI   A4 //D1
#define OLED_CLK    A3 //D0
#define OLED_DC     A2 //DC
#define OLED_CS     A1 //CS
#define OLED_RESET  A5 //RST

// Pins 4x4 keypad is connected to
byte keys1[] = {0, 1, 2, 3};
byte keys2[] = {4, 5, 6, 7};

// Ananlog pin used for RNG
#define RANDOM A0

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Pin that SD card is connected to
#define SD_CS 10

// Pin that buzzer is connected to
#define SOUND_PIN 8

// Byte array representing CHIP-8 memory locations
// for some reason can not be created dinamicly in class
byte mem[MEMORY_SIZE];

// Keyboard object
Keyboard keyboard(keys1, keys2);

// Screen object
Screen screen(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS, 2);

// Memory object
Memory memory(SD_CS, mem);

// Speaker object
Speaker speaker(SOUND_PIN);

// CPU object
CPU cpu(memory, screen, keyboard, speaker, RANDOM);


void setup() {
    char *romName = "WIPEOFF"; // Name of the rom to be loaded
    ///////////////////////////////////
    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);
    ///////////////////////////////////
    screen.displayText("LOADING ROM...\n");
    delay(500);
    if (!memory.initialize()) {
        screen.displayText("Error with SD card!");
    } else {
        if (!memory.loadRom(romName)) {
            screen.displayText("Error loading ROM!");
        } else {
            screen.clear();
        }
    }
}

// Timer interupts
void timer(struct tc_module *const module_inst) {
    cpu.decrementTimers();
}

void loop() {
    cpu.run();
}
