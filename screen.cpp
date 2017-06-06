#include "screen.h"

Screen::Screen(int mosi, int clk, int dc, int reset, int cs, int scale) : display(mosi, clk, dc, reset, cs), scale(scale) {
    width = DEFAULT_WIDTH;
    height = DEFAULT_HEIGHT;
    display.begin(SSD1306_SWITCHCAPVCC);
    display.setTextColor(WHITE);
    clear();
}

void Screen::markPixel(int x, int y, bool on) {
    int color = on == true ? 1 : 0;
    display.fillRect(x * scale, y * scale, scale, scale, color);
    buf[x][y] = on;
}

void Screen::show() {
    display.display();
}

bool Screen::isPixelOn(int x, int y) {
    return buf[x][y];
}

void Screen::clear() {
    memset(buf, 0, sizeof(buf));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.display();
}

void Screen::displayText(char *text) {
    display.print(text);
    display.display();
}

void Screen::displayText(int number) {
    display.print(number);
    display.display();
}

void Screen::setWidth(int width) {
    this->width = width;    
}

void Screen::setHeight(int height) {
    this->height = height;
}

void Screen::setScale(int scale1) {
    scale = scale1;
}

int Screen::getWidth() {
    return width;
}

int Screen::getHeight() {
    return height;
}

int Screen::getScale() {
    return scale;
}

