#include "64x64.h"

Matrix64x64::Matrix64x64() : lastBlinkTime(0), blinkState(false) {
    // Constructor
}

// Métodos para la luz indicadora de dirección derecha
void Matrix64x64::DirectionIndicatorLight_R(int x, int y, uint16_t color1, Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(DirectionIndicatorLight_RMatrix[i * 64 + j]));
            if (pixel == 1) {
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
            uint8_t pixel = pgm_read_byte(&(DirectionIndicatorLight_RMatrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, currentColor);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

// Métodos para la luz indicadora de dirección izquierda
void Matrix64x64::DirectionIndicatorLight_L(int x, int y, uint16_t color, Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(DirectionIndicatorLight_LMatrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, color);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

void Matrix64x64::DirectionIndicatorLight_L(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= interval) {
        lastBlinkTime = currentTime;
        blinkState = !blinkState;
    }

    uint16_t currentColor = blinkState ? color1 : color2;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(DirectionIndicatorLight_LMatrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, currentColor);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

// Métodos para las luces de estacionamiento
void Matrix64x64::ParkingLights(int x, int y, uint16_t color, Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(ParkingLights_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, color);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

void Matrix64x64::ParkingLights(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= interval) {
        lastBlinkTime = currentTime;
        blinkState = !blinkState;
    }

    uint16_t currentColor = blinkState ? color1 : color2;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(ParkingLights_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, currentColor);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

// Métodos para las luces altas
void Matrix64x64::HighBeams(int x, int y, uint16_t color, Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(HighBeams_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, color);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

void Matrix64x64::HighBeams(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= interval) {
        lastBlinkTime = currentTime;
        blinkState = !blinkState;
    }

    uint16_t currentColor = blinkState ? color1 : color2;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(HighBeams_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, currentColor);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

// Métodos para las luces bajas
void Matrix64x64::LowBeams(int x, int y, uint16_t color, Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(LowBeams_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, color);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

void Matrix64x64::LowBeams(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= interval) {
        lastBlinkTime = currentTime;
        blinkState = !blinkState;
    }

    uint16_t currentColor = blinkState ? color1 : color2;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(LowBeams_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, currentColor);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

// Métodos para las luces de emergencia
void Matrix64x64::HazardLights(int x, int y, uint16_t color, Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(HazardLights_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, color);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

void Matrix64x64::HazardLights(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= interval) {
        lastBlinkTime = currentTime;
        blinkState = !blinkState;
    }

    uint16_t currentColor = blinkState ? color1 : color2;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(HazardLights_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, currentColor);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

// Métodos para las luces de niebla
void Matrix64x64::FogLights(int x, int y, uint16_t color, Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(FogLights_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, color);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}

void Matrix64x64::FogLights(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= interval) {
        lastBlinkTime = currentTime;
        blinkState = !blinkState;
    }

    uint16_t currentColor = blinkState ? color1 : color2;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            uint8_t pixel = pgm_read_byte(&(FogLights_Matrix[i * 64 + j]));
            if (pixel == 1) {
                tft.drawPixel(x + j, y + i, currentColor);
            } else {
                tft.drawPixel(x + j, y + i, ILI9341_BLACK);
            }
        }
    }
}
