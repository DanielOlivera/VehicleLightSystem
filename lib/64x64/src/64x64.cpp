#include "64x64.h"

Matrix64x64::Matrix64x64() : lastBlinkTime(0), blinkState(false) {
    // Constructor
}

void Matrix64x64::DirectionIndicatorLight(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= interval) {
        lastBlinkTime = currentTime;
        blinkState = !blinkState;
    }

    uint16_t currentColor = blinkState ? color1 : color2;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if (DirectionIndicatorLight_R[i * 64 + j] == 1) {
                tft.drawPixel(x + j, y + i, currentColor);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}
