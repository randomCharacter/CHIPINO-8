#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DEFAULT_WIDTH 64
#define DEFAULT_HEIGHT 32
#define DEFAULT_SCALE 1

/**
 * CHIP-8 screen realized with Adafruit_SSD1306 compatible display
 */
class Screen {
    private:
        // Display object
        Adafruit_SSD1306 display;
        // Screen width in pixels
        int width;
        // Screen height in pixels
        int height;
        // Screen scale
        int scale;
        // Screen buffer
        bool buf[64][64];
        
    public:
        /**
         * Default constructor.
         * 
         * @param mosi Number of pin screen MOSI is connected to
         * @param clk Number of pin screen CLK is connected to
         * @param dc Number of pin screen DC is connected to
         * @param reset Number of pin screen RESET is connected to
         * @param scale Scale of screen pixels, if not specified DEFAULT_SCALE will be used
         */
        Screen(int mosi, int clk, int dc, int reset, int cs, int scale = DEFAULT_SCALE);

        /**
         * Checks if pixel on given coordinates is turned on,
         *
         * @param x The x coordinate of pixel
         * @param y The y coordinate of pixel
         * @return <code>true</code> if pixel is set to on,
         *         <code>false</code> if it is set to off
         */
        bool isPixelOn(int x, int y);

        /**
         * Turns the pixel with given coordinates on or off.
         *
         * @param x The x coordinate of pixel
         * @param y The y coordinate of pixel
         * @param on If <code>true</code> pixel is turned on,
         *           otherwise it is turned off
         */
        void markPixel(int x, int y, bool on);

        /**
         * Clears the screen.
         */
        void clear();

        /**
         * Displays given text to the screen.
         * 
         * @param text Text to be displayed
         */
        void displayText(char* text);

        /**
         * Displays given number to the screen.
         * 
         * @param number Number to be displayed
         */
        void displayText(int number);

        /**
         * Updates screen image with changes made to buffer
         */
        void show();

        /*
         * Sets the width of the screen.
         * 
         * @param width Value to be assigned to width
         */
        void setWidth(int width);

        /*
         * Sets the height of the screen.
         * 
         * @param height Value to be assigned to height
         */
        void setHeight(int height);

        /*
         * Sets the scale of the screen.
         * 
         * @param scale Value to be assigned to scale
         */
        void setScale(int scale);

        /**
         * Returns width of the screen.
         * 
         * @return The height of the screen
         */
        int getWidth();

        /**
         * Returns height of the screen.
         * 
         * @return The height of the screen
         */
        int getHeight();

        /**
         * Returns scale of the screen.
         * 
         * @return The sclae of the screen
         */
        int getScale();
};

#endif
