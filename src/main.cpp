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
void dibujarFlecha(int x, int y, int size, uint16_t color, String direccion);
void parpadearFlecha();
void controlarDibujo();

// Definición de pines para la pantalla TFT
#define TFT_CS   D8
#define TFT_DC   D4
#define TFT_MOSI D7
#define TFT_CLK  D5
#define TFT_MISO D6

// Pines del MCP23X17
#define MCP23X17_CONTROL_PIN_1 0  // Pin 0 del MCP23X17
#define MCP23X17_CONTROL_PIN_2 1  // Pin 1 del MCP23X17
#define MCP23X17_CONTROL_PIN_3 2  // Pin 2 del MCP23X17
#define MCP23X17_CONTROL_PIN_4 3  // Pin 3 del MCP23X17
#define MCP23X17_CONTROL_PIN_5 4  // Pin 4 del MCP23X17
#define MCP23X17_CONTROL_PIN_6 5  // Pin 5 del MCP23X17
#define MCP23X17_CONTROL_PIN_7 6  // Pin 6 del MCP23X17
#define MCP23X17_CONTROL_PIN_8 7  // Pin 7 del MCP23X17

// Definición de pines para el control de parpadeo
//#define CONTROL_PIN_0 D0

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
bool DirectionIndicatorLightDibujada  = false; // Indica si la carita feliz está dibujada

void iniciarBH1750() {
  Wire.beginTransmission(0x5C);
  if (Wire.endTransmission() == 0) {
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C)) {
      tft.println("BH1750 OK");
    } else {
      tft.println("BH1750 Error");
    }
  } else {
    tft.println("BH1750 No Responde");
  }
}

int obtenerLuminosidad() {
  if (lightMeter.measurementReady(true)) {
    return static_cast<int>(lightMeter.readLightLevel());
  }
  return -1;
}

void desplegarLuminosidad(int lux) {
  if (lux != lastLux) {
    char buffer[20]; // Buffer para almacenar el texto de salida
    sprintf(buffer, "Luz: %d lx", lux); // Formatea la salida en el buffer
    tft.fillRect(0, 20, 240, 40, ILI9341_BLACK); // Limpia solo la parte del texto
    tft.setCursor(0, 20);
    tft.print(buffer); // Imprime el texto desde el buffer
    lastLux = lux;
  }
}

void dibujarFlecha(int x, int y, int size, uint16_t color, String direccion) {
  tft.fillRect(x - size, y - size, size * 2, size * 2, ILI9341_BLACK);
  if (direccion == "izq") {
    tft.fillTriangle(x - size / 2, y, x + size / 2, y - size / 2, x + size / 2, y + size / 2, color);
    tft.fillRect(x + size / 2, y - size / 6, size / 2, size / 3, color);
  } else if (direccion == "der") {
    tft.fillTriangle(x + size / 2, y, x - size / 2, y - size / 2, x - size / 2, y + size / 2, color);
    tft.fillRect(x - size, y - size / 6, size / 2, size / 3, color);
  }
}

void parpadearFlecha() {
  if (millis() - lastFlechaUpdate >= 500) {  // Cambiar color cada 500 ms
    lastFlechaUpdate = millis();
    estadoFlecha = !estadoFlecha;  // Alternar el estado de la flecha

    if (estadoFlecha) {
      dibujarFlecha(tft.width() / 2, tft.height() / 2, 50, ILI9341_GREEN, "izq");  // Flecha verde claro
    } else {
      dibujarFlecha(tft.width() / 2, tft.height() / 2, 50, ILI9341_ORANGE, "izq");  // Flecha naranja
    }
  }
}

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  Wire.begin(D2, D1); // Inicializa I2C
  //pinMode(CONTROL_PIN_0, INPUT_PULLDOWN_16); // Configura el pin de control como entrada
  
  mcp.begin_I2C();
  mcp.pinMode(MCP23X17_CONTROL_PIN_1, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_1, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_2, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_2, LOW);  // Establecer el estado inicial a LOW
  
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  iniciarBH1750();
}

void loop() {
  if (millis() - lastLuxUpdate >= 250) {  // Actualizar luminosidad cada 2 segundos
    lastLuxUpdate = millis();
    int currentLux = obtenerLuminosidad();
    desplegarLuminosidad(currentLux);
  }
  controlarDibujo();
}

void controlarDibujo() {
    if (mcp.digitalRead(MCP23X17_CONTROL_PIN_1) == HIGH) {
        if (!DirectionIndicatorLightDibujada) {
            matrix.DirectionIndicatorLight(50, 50, ILI9341_YELLOW, tft);
            DirectionIndicatorLightDibujada = true;
        }
    } else {
        if (DirectionIndicatorLightDibujada) {
            tft.fillRect(50, 50, 64, 64, ILI9341_BLACK);  // Borra la DirectionIndicatorLight_R
            DirectionIndicatorLightDibujada = false;
        }
    }
}
