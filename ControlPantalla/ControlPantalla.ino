#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_CLK 13

uint16_t colorTexto = ILI9341_WHITE, colorFondo = ILI9341_BLACK;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  Serial.begin(9600); 
  tft.begin();
  tft.fillScreen(colorFondo);
  tft.setCursor(0, 0);
  tft.setTextColor(colorTexto);  tft.setTextSize(3);
}

void loop(void) {
  tft.println("Hello World!");
  delay(2000);
  BorrarCadena(0, 0, "Hello World!");
  tft.println("Prueba 1");
  delay(5000);
  BorrarCadena(0, 0, "Prueba 1");
}

void BorrarCadena(int x, int y, String texto){
  tft.setTextColor(colorFondo);
  tft.setCursor(x, y);
  tft.println(texto);
  tft.setTextColor(colorTexto);
  tft.setCursor(x, y);
}

void EscribirCadena(int x, int y, String texto){
  tft.setTextColor(colorTexto);
  tft.setCursor(x, y);
  tft.println(texto);
}
