#include <SPI.h>		// incluye libreria bus SPI
#include <Adafruit_GFX.h>	// incluye libreria para manejo de graficos
#include <Adafruit_ILI9341.h>	// incluye libreria para controlador ILI9341
 
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_CLK 13
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

//Estado de interface
//1 - MENU
//2 - Entrenamiento
//3 - Reconocimiento
int estado = 1;
int select = 1;
int btnA = 7, btnB = 6;
int xi = 40, yi = 45;

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(3);			// establece posicion horizontal
  tft.fillScreen(ILI9341_RED); 		// relleno de pantalla en color rojo
  tft.setTextColor(ILI9341_WHITE);	// color de texto en blanco
  tft.setTextSize(3);			// escala de texto en 2
  Menu(xi+25, yi-10);
  PintaFlecha(xi, yi);

}

void loop() {
  if(estado == 1){
    if (digitalRead(btnA) == HIGH){
      if (select == 1){
        PintaFlecha(xi, yi+80);
        select += 1; 
      }else if (select == 2){
        PintaFlecha(xi, yi);
        select -= 1; 
      }
      delay(500);
    }else if (digitalRead(btnB) == HIGH){
      if(select == 1){
        Serial.println("Opcion de Entrenamiento");
        tft.fillScreen(ILI9341_RED); 		// relleno de pantalla en color rojo
        estado = 2;
      }else if(select == 2){
        Serial.println("Opcion de reconocimiento");
        tft.fillScreen(ILI9341_RED); 		// relleno de pantalla en color rojo
        estado = 3;
      }
      select = 1;
      delay(500);
    }
  }
  else if(estado == 2){

  }
  else{
    
  }
}

int Menu(int x, int y){
  tft.setCursor(x,y);			// ubica cursor
  tft.print("Entrenamiento"); 	// imprime texto
  y += 80;
  tft.setCursor(x,y);			// ubica cursor
  tft.print("Reconocimiento"); 	// imprime texto
}

void borrarFlecha(){
  tft.fillRect(0, 0, xi, tft.height(), ILI9341_RED);
}

void PintaFlecha(int cx, int cy) {
  borrarFlecha();
  int i=35;
    Serial.println("PINTANDO " );
    tft.fillTriangle(cx, cy, cx - i, cy + i/2, cx - i, cy - i/2, tft.color565(0, 1190, 1190));
}