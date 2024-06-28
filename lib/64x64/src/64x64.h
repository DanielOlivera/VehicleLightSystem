#ifndef _64x64_H
#define _64x64_H

#include <Adafruit_ILI9341.h> // Solo se incluye en el archivo .h para la declaración de la clase
#include <Adafruit_GFX.h>
#include <Arduino.h>

// Declaración de las matrices en PROGMEM
extern const uint8_t DirectionIndicatorLight_RMatrix[64 * 64] PROGMEM;
extern const uint8_t DirectionIndicatorLight_LMatrix[64 * 64] PROGMEM;
extern const uint8_t ParkingLights_Matrix[64 * 64] PROGMEM;
extern const uint8_t HighBeams_Matrix[64 * 64] PROGMEM;
extern const uint8_t LowBeams_Matrix[64 * 64] PROGMEM;
extern const uint8_t HazardLights_Matrix[64 * 64] PROGMEM;
extern const uint8_t FogLights_Matrix[64 * 64] PROGMEM;

class Matrix64x64 {
public:
    Matrix64x64();
    void DirectionIndicatorLight_R(int x, int y, uint16_t color1, Adafruit_ILI9341 &tft); // Se elimina el parámetro color2 y interval 
    void DirectionIndicatorLight_R(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft); // Se agrega el parámetro interval
    void DirectionIndicatorLight_L(int x, int y, uint16_t color, Adafruit_ILI9341 &tft);
    void DirectionIndicatorLight_L(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft);
    void ParkingLights(int x, int y, uint16_t color, Adafruit_ILI9341 &tft);
    void ParkingLights(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft);
    void HighBeams(int x, int y, uint16_t color, Adafruit_ILI9341 &tft);
    void HighBeams(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft);
    void LowBeams(int x, int y, uint16_t color, Adafruit_ILI9341 &tft);
    void LowBeams(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft);
    void HazardLights(int x, int y, uint16_t color, Adafruit_ILI9341 &tft);
    void HazardLights(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft);
    void FogLights(int x, int y, uint16_t color, Adafruit_ILI9341 &tft);
    void FogLights(int x, int y, uint16_t color1, uint16_t color2, unsigned long interval, Adafruit_ILI9341 &tft);
    
private:   
    unsigned long lastBlinkTime;
    bool blinkState;
};

#endif
