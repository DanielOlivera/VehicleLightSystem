#include "64x64.h"

Matrix64x64::Matrix64x64() : lastBlinkTime(0), blinkState(false) {
    // Constructor
}


void Matrix64x64::DirectionIndicatorLight_R(int x, int y, uint16_t color1, Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if (DirectionIndicatorLight_RMatrix[i * 64 + j] == 1) {
                tft.drawPixel(x + j, y + i, color1);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

void Matrix64x64::DirectionIndicatorLight_R(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= interval) {
        lastBlinkTime = currentTime;
        blinkState = !blinkState;
    }

    uint16_t currentColor = blinkState ? color1 : color2;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if (DirectionIndicatorLight_RMatrix[i * 64 + j] == 1) {
                tft.drawPixel(x + j, y + i, currentColor);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}
