#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_MCP23X17.h>
#include <BH1750.h>
#include <64x64.h>

// Declaración de funciones
void iniciarBH1750();
int obtenerLuminosidad();
void desplegarLuminosidad(int lux);
void controlarDibujo();

// Definición de pines para la pantalla TFT
#define TFT_CS   D8
#define TFT_DC   D4
#define TFT_MOSI D7
#define TFT_CLK  D5
#define TFT_MISO D6

// Definición del colores oscuros formato RGB565
#define DARKRED 0x8800
#define DARKBLUE 0x0010
#define DARKORANGE 0xBAA0
#define ORANGE 0xFC60

// Pines del MCP23X17
#define MCP23X17_CONTROL_PIN_1 0  // Pin 0 del MCP23X17
#define MCP23X17_CONTROL_PIN_2 1  // Pin 1 del MCP23X17
#define MCP23X17_CONTROL_PIN_3 2  // Pin 2 del MCP23X17
#define MCP23X17_CONTROL_PIN_4 3  // Pin 3 del MCP23X17
#define MCP23X17_CONTROL_PIN_5 4  // Pin 4 del MCP23X17
#define MCP23X17_CONTROL_PIN_6 5  // Pin 5 del MCP23X17
#define MCP23X17_CONTROL_PIN_7 6  // Pin 6 del MCP23X17
#define MCP23X17_CONTROL_PIN_8 7  // Pin 7 del MCP23X17

// Inicializar componentes
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
BH1750 lightMeter;
Adafruit_MCP23X17 mcp;
Matrix64x64 matrix;

//Variables globales
int lastLux = -1; // Almacena la última lectura de luminosidad para comparación
unsigned long lastFlechaUpdate = 0;
unsigned long lastLuxUpdate = 0;
bool estadoFlecha = false;

void iniciarBH1750() {
  Wire.beginTransmission(0x5C);
  if (Wire.endTransmission() == 0 && lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C)) {
    tft.println("BH1750 OK");
  } else {
    tft.println("BH1750 Error");
  }
}
int obtenerLuminosidad() {
  return lightMeter.measurementReady(true) ? static_cast<int>(lightMeter.readLightLevel()) : -1;
}
void desplegarLuminosidad(int lux) {
  if (lux != lastLux) {
    tft.fillRect(0, 20, 240, 40, ILI9341_BLACK); // Limpia solo la parte del texto
    tft.setCursor(0, 20); // Posiciona el cursor en la pantalla TFT
    tft.printf("Luz: %d lx", lux); // Imprime el texto formateado directamente
    lastLux = lux; // Actualiza la última lectura de luminosidad
  }
}

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  Wire.begin(D2, D1); // Inicializa I2C
  mcp.begin_I2C();
  mcp.pinMode(MCP23X17_CONTROL_PIN_1, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_1, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_2, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_2, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_3, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_3, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_4, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_4, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_5, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_5, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_6, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_6, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_7, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_7, LOW);  // Establecer el estado inicial a LOW
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  iniciarBH1750();
}

void loop() {
  if (millis() - lastLuxUpdate >= 250) {  // Actualizar luminosidad cada 2 segundos
    lastLuxUpdate = millis();
    int currentLux = obtenerLuminosidad();
    //desplegarLuminosidad(currentLux);
  }
  controlarDibujo();
}

void controlarDibujo() {
  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_1) == HIGH) {
    matrix.DirectionIndicatorLight_R(168, 64, ILI9341_RED, ILI9341_GREEN, 500, tft);
  } else {
    matrix.DirectionIndicatorLight_R(168, 64, ILI9341_DARKGREEN, tft);
  }
  
  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_2) == HIGH) {
    matrix.DirectionIndicatorLight_L(8, 64, ILI9341_RED, ILI9341_GREEN, 500, tft);  // Parpadeo cada 500 ms entre rojo y verde
  } else {
    matrix.DirectionIndicatorLight_L(8, 64, ILI9341_DARKGREEN, tft);  // Flecha verde oscura sin parpadeo   
  }

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_3) == HIGH) {
    matrix.ParkingLights(88, 64, ORANGE, ILI9341_GREEN, 500, tft);
  } else {
    matrix.ParkingLights(88, 64, DARKORANGE, tft); 
  }

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_4) == HIGH) {
    matrix.HazardLights(88, 138, ILI9341_RED, DARKRED, 500, tft);
  } else {
    matrix.HazardLights(88, 138, DARKRED, tft); 
  }

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_5) == HIGH) {
    matrix.HighBeams(8, 212, ILI9341_BLUE, tft);
  } else {
    matrix.HighBeams(8, 212, DARKBLUE, tft); 
  }

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_6) == HIGH) {
    matrix.LowBeams(88, 212, ILI9341_BLUE, tft);
  } else {
    matrix.LowBeams(88, 212, DARKBLUE, tft); 
  }

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_7) == HIGH) {
    matrix.FogLights(168, 212, ILI9341_BLUE, tft);
  } else {
    matrix.FogLights(168, 212, DARKBLUE, tft); 
  }
}

