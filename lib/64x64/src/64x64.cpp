#include "64x64.h"

Matrix64x64::Matrix64x64() {
    // Constructor
}

void Matrix64x64::DirectionIndicatorLight(int x, int y, uint16_t color, Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if (DirectionIndicatorLight_R[i * 64 + j] == 1) {
                tft.drawPixel(x + j, y + i, color);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}
