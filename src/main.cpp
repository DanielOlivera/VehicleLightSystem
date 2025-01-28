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
//int obtenerLuminosidad();

int obtenerLuminosidadSensor1();
int obtenerLuminosidadSensor2();

void desplegarLuminosidad(int lux1, int lux2);
void controlarDibujo();
bool testSensores();

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

// Pines del puerto A del MCP23X17
#define MCP23X17_CONTROL_PIN_A1 0  // Pin 0 del MCP23X17
#define MCP23X17_CONTROL_PIN_A2 1  // Pin 1 del MCP23X17
#define MCP23X17_CONTROL_PIN_A3 2  // Pin 2 del MCP23X17
#define MCP23X17_CONTROL_PIN_A4 3  // Pin 3 del MCP23X17
#define MCP23X17_CONTROL_PIN_A5 4  // Pin 4 del MCP23X17
#define MCP23X17_CONTROL_PIN_A6 5  // Pin 5 del MCP23X17
#define MCP23X17_CONTROL_PIN_A7 6  // Pin 6 del MCP23X17
#define MCP23X17_CONTROL_PIN_A8 7  // Pin 7 del MCP23X17

// Pines del puerto B del MCP23X17
#define MCP23X17_OUTPUT_PIN_B0 8  // Pin 8 del MCP23X17 (B0)
#define MCP23X17_OUTPUT_PIN_B1 9  // Pin 9 del MCP23X17 (B1)
#define MCP23X17_OUTPUT_PIN_B2 10 // Pin 10 del MCP23X17 (B2)
#define MCP23X17_OUTPUT_PIN_B3 11 // Pin 11 del MCP23X17 (B3)
#define MCP23X17_OUTPUT_PIN_B4 12 // Pin 12 del MCP23X17 (B4)
#define MCP23X17_OUTPUT_PIN_B5 13 // Pin 13 del MCP23X17 (B5)

//Definición de direcciones I2C para los sensores BH1750
#define SENSOR1_ADDRESS 0x23
#define SENSOR2_ADDRESS 0x5C

// Inicializar componentes
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
BH1750 lightMeter1(SENSOR1_ADDRESS);
BH1750 lightMeter2(SENSOR2_ADDRESS);
//BH1750 lightMeter;
Adafruit_MCP23X17 mcp;
Matrix64x64 matrix;

//Variables globales
int lastLux1 = -1; // Almacena la última lectura de luminosidad para comparación
int lastLux2 = -1; // Almacena la última lectura de luminosidad para comparación
unsigned long lastLuxUpdate = 0;
unsigned long lastBlinkTime = 0;  // Tiempo de la última conmutación
const long blinkInterval = 500;   // Intervalo de parpadeo en milisegundos
bool blinkState = LOW;            // Estado inicial para parpadeo
bool estadoPinB0 = LOW;  // Inicializa el estado del pin B0 del MCP23X17
bool estadoPinB1 = LOW;  // Inicializa el estado del pin B1 del MCP23X17

bool verificarDireccionI2C(uint8_t address) {
  Wire.beginTransmission(address);
  return (Wire.endTransmission() == 0);
}

// Inicializa los sensores BH1750.
void iniciarBH1750() {
  // Verificar y inicializar el primer sensor BH1750
  if (verificarDireccionI2C(SENSOR1_ADDRESS)) {
    if (!lightMeter1.begin(BH1750::CONTINUOUS_LOW_RES_MODE)) {
      tft.setTextColor(ILI9341_RED);
      tft.printf("Error -> sensor 1 BH1750 (0x%02X)\n", SENSOR1_ADDRESS);
    } else {
      tft.setTextColor(ILI9341_GREEN);
      tft.printf("Sensor 1 BH1750 OK: address(0x%02X)\n", SENSOR1_ADDRESS);
    }
  } 
  // Verificar y inicializar el segundo sensor BH1750
  if (verificarDireccionI2C(SENSOR2_ADDRESS)) {
    if (!lightMeter2.begin(BH1750::CONTINUOUS_LOW_RES_MODE)) {
      tft.setTextColor(ILI9341_RED);
      tft.printf("Error -> sensor 2 BH1750 (0x%02X)\n", SENSOR2_ADDRESS);
    } else {
      tft.setTextColor(ILI9341_GREEN);
      tft.printf("Sensor 2 BH1750 OK: address(0x%02X)\n", SENSOR2_ADDRESS);
    }
  } 
  // Limpiar la pantalla TFT después de la confirmación
  delay(5000); // Esperar 2 segundos para que los mensajes sean visibles
  tft.fillScreen(ILI9341_BLACK);
}

int obtenerLuminosidadSensor1() {
  return static_cast<int>(lightMeter1.readLightLevel());
}

// Obtiene la luminosidad del sensor BH1750 en la dirección 0x5C.
int obtenerLuminosidadSensor2() {
  return static_cast<int>(lightMeter2.readLightLevel());
}

void desplegarLuminosidad(int lux1, int lux2) {
  tft.fillRect(0, 20, 240, 40, ILI9341_BLACK); // Limpia solo la parte del texto
  tft.setCursor(0, 20); // Posiciona el cursor en la pantalla TFT
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.printf("Luz 1: %d lx", lux1); // Imprime el texto formateado directamente
  tft.setCursor(0, 40); // Posiciona el cursor en la pantalla TFT
  tft.printf("Luz 2: %d lx", lux2); // Imprime el texto formateado directamente
}

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  Wire.begin(D2, D1); // Inicializa I2C
  mcp.begin_I2C();
  mcp.pinMode(MCP23X17_CONTROL_PIN_A1, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_A1, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_A2, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_A2, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_A3, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_A3, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_A4, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_A4, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_A5, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_A5, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_A6, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_A6, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_CONTROL_PIN_A7, INPUT);
  mcp.digitalWrite(MCP23X17_CONTROL_PIN_A7, LOW);  // Establecer el estado inicial a LOW
  mcp.pinMode(MCP23X17_OUTPUT_PIN_B0, OUTPUT);
  mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B0, HIGH); // Estado inicial HIGH
  mcp.pinMode(MCP23X17_OUTPUT_PIN_B1, OUTPUT);
  mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B1, HIGH); // Estado inicial HIGH
  mcp.pinMode(MCP23X17_OUTPUT_PIN_B2, OUTPUT);
  mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B2, HIGH); // Estado inicial HIGH
  mcp.pinMode(MCP23X17_OUTPUT_PIN_B3, OUTPUT);
  mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B3, HIGH); // Estado inicial HIGH
  mcp.pinMode(MCP23X17_OUTPUT_PIN_B4, OUTPUT);
  mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B4, HIGH); // Estado inicial HIGH
  mcp.pinMode(MCP23X17_OUTPUT_PIN_B5, OUTPUT);
  mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B5, HIGH); // Estado inicial HIGH
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  iniciarBH1750();

  // Ejecutar la función de prueba
  if (!testSensores()) {
    tft.println(F("Error en la prueba de sensores"));
    while (true); // Detener la ejecución si la prueba falla
  }

}

void loop() {
  /*if (millis() - lastLuxUpdate >= 100) {  // Actualizar luminosidad cada 2 segundos
    lastLuxUpdate = millis();
    int currentLux1 = obtenerLuminosidadSensor1();
    int currentLux2 = obtenerLuminosidadSensor2();
    //desplegarLuminosidad(currentLux1, currentLux2);
  }*/
  controlarDibujo();
}

void controlarDibujo() {

/***************************************OK***************************************/

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_A1) == HIGH) {
    matrix.DirectionIndicatorLight_L(8, 64, ILI9341_RED, ILI9341_GREEN, 500, tft);
    if (millis() - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = millis();  // Actualiza el tiempo del último parpadeo
      blinkState = !blinkState;  // Alterna el estado
      estadoPinB0 = blinkState;  // Guarda el estado en la variable
      mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B0, blinkState ? LOW : HIGH);
    }

    delay(250); // Retardo para evitar falsos positivos

    int lux1 = obtenerLuminosidadSensor1() > 500 ? 1 : 0; // Obtener el valor de luminosidad del sensor 1
    bool pinB0State = mcp.digitalRead(MCP23X17_OUTPUT_PIN_B0) == LOW; // Estado del pin B0 (invertido)

    if ((estadoPinB0 == 1 && pinB0State == 0 && lux1 == 1) || // ERROR
        (estadoPinB0 == 0 && pinB0State == 1 && lux1 == 0) || // ERROR
        (estadoPinB0 == 0 && pinB0State == 1 && lux1 == 1) || // ERROR
        (estadoPinB0 == 0 && pinB0State == 0 && lux1 == 1) || // ERROR
        (estadoPinB0 == 1 && pinB0State == 1 && lux1 == 0) || // ERROR
        (estadoPinB0 == 1 && pinB0State == 0 && lux1 == 0) || // ERROR
        (estadoPinB0 == 1 && pinB0State == 0 && lux1 == 1)) { // ERROR
      tft.fillScreen(ILI9341_BLACK); // Limpiar la pantalla
      tft.setCursor(0, 0);
      tft.setTextColor(ILI9341_RED);
      tft.setTextSize(2);
      tft.println(F("Error: Estado inconsistente"));
      while (true); // Mantener en este estado hasta que el problema se solucione
    } else if ((estadoPinB0 == 0 && pinB0State == 0 && lux1 == 0) || // OK
               (estadoPinB0 == 1 && pinB0State == 1 && lux1 == 1)) { // OK
      // Estado consistente, continuar con la ejecución normal
    }
  } else {
    matrix.DirectionIndicatorLight_L(8, 64, ILI9341_DARKGREEN, tft);
    mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B0, HIGH);
  }

/***************************************OK***************************************/
  
  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_A2) == HIGH) {
    matrix.DirectionIndicatorLight_R(168, 64, ILI9341_RED, ILI9341_GREEN, 500, tft);
    if (millis() - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = millis();  // Actualiza el tiempo del último parpadeo
      blinkState = !blinkState;  // Alterna el estado
      mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B1, blinkState ? HIGH : LOW);
    }
  } else {
    matrix.DirectionIndicatorLight_R(168, 64, ILI9341_DARKGREEN, tft);
    mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B1, HIGH); 
  }

/***************************************OK***************************************/

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_A3) == HIGH) {
    matrix.HazardLights(88, 64, ILI9341_RED, DARKRED, 500, tft);
    if (millis() - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = millis();  // Actualiza el tiempo del último parpadeo
      blinkState = !blinkState;  // Alterna el estado
      mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B2, blinkState ? LOW : HIGH);
    }
  } else {
    matrix.HazardLights(88, 64, DARKRED, tft);
    mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B2, HIGH);
  }

/***************************************OK***************************************/

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_A4) == HIGH) {
    matrix.Lights(88, 138, ILI9341_GREENYELLOW, tft);
  } else {
    matrix.Lights(88, 138, DARKORANGE, tft); 
  }

/***************************************OK***************************************/

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_A5) == HIGH) {
      matrix.HighBeams(8, 212, ILI9341_BLUE, tft);
      mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B3, LOW);
    } else {
      matrix.HighBeams(8, 212, DARKBLUE, tft); 
      mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B3, HIGH);
    }

/***************************************OK***************************************/

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_A6) == HIGH) {
    matrix.LowBeams(88, 212, ILI9341_BLUE, tft);
    mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B4, LOW);
  } else {
    matrix.LowBeams(88, 212, DARKBLUE, tft); 
    mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B4, HIGH);
  }

/***************************************OK***************************************/

  if (mcp.digitalRead(MCP23X17_CONTROL_PIN_A7) == HIGH) {
    matrix.FogLights(168, 212, ILI9341_BLUE, tft);
    mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B5, LOW);
  } else {
    matrix.FogLights(168, 212, DARKBLUE, tft); 
    mcp.digitalWrite(MCP23X17_OUTPUT_PIN_B5, HIGH);
  }

/***************************************OK***************************************/
}

bool testSensores() {
  // Leer y mostrar la luminosidad del primer sensor durante 5 segundos
  tft.fillRect(0, 20, 240, 20, ILI9341_BLACK); // Limpia solo la parte del texto
  tft.setCursor(0, 20);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Sensor 1: ");
  for (int i = 0; i < 50; i++) {
    int lux1 = obtenerLuminosidadSensor1();
    tft.fillRect(120, 20, 120, 20, ILI9341_BLACK); // Limpia solo la parte del valor
    tft.setCursor(120, 20);
    tft.printf("%d lx", lux1);
    delay(100);
  }

  // Leer y mostrar la luminosidad del segundo sensor durante 5 segundos
  tft.fillRect(0, 40, 240, 20, ILI9341_BLACK); // Limpia solo la parte del texto
  tft.setCursor(0, 40);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Sensor 2: ");
  for (int i = 0; i < 50; i++) {
    int lux2 = obtenerLuminosidadSensor2();
    tft.fillRect(120, 40, 120, 20, ILI9341_BLACK); // Limpia solo la parte del valor
    tft.setCursor(120, 40);
    tft.printf("%d lx", lux2);
    delay(100);
  }
  // Si ambas lecturas son exitosas, retornar true
  tft.fillScreen(ILI9341_BLACK);
  delay(500);
  return true;
}