//Include the library
#include <MQUnifiedsensor.h>

/************************Hardware Related Macros************************************/
#define         Board                   ("Arduino Uno")
#define         Pin0                     (A1)  //Analog input 2 of your arduino
#define         Pin1                     (A0)  //Analog input 3 of your arduino
#define         Pin2                     (A2)  //Analog input 4 of your arduino
#define         Pin3                     (A3)  //Analog input 5 of your arduino
#define         Pin4                     (A4)  //Analog input 6 of your arduino
#define         Pin5                     (A5)  //Analog input 7 of your arduino

/***********************Software Related Macros************************************/
#define         RatioMQ2CleanAir          (9.86) //RS / R0 = 9.83 ppm 
#define         RatioMQ3CleanAir          (63) //RS / R0 = 60 ppm 
#define         RatioMQ4CleanAir          (4.4) //RS / R0 = 4.4 ppm 
#define         RatioMQ7CleanAir          (27.5) //RS / R0 = 27.5 ppm  
#define         RatioMQ8CleanAir          (70) //RS / R0 = 70 ppm
#define         RatioMQ135CleanAir        (3.6) //RS / R0 = 3.6 ppm 
#define         ADC_Bit_Resolution        (10) // 10 bit ADC 
#define         Voltage_Resolution        (5) // Volt resolution to calc the voltage
#define         Type                      ("MQ-2")
#define         Type                      ("MQ-3")
#define         Type                      ("MQ-4")
#define         Type                      ("MQ-7")
#define         Type                      ("MQ-8")
#define         Type                      ("MQ-135")

/*****************************Globals***********************************************/
//Declare Sensor
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin0, "MQ-2");
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin1, "MQ-3");
MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin2, "MQ-4");
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin3, "MQ-7");
MQUnifiedsensor MQ8(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin4, "MQ-8");
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin5, "MQ-135");

unsigned long oldTime = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>		// incluye libreria bus SPI
#include <Adafruit_GFX.h>	// incluye libreria para manejo de graficos
#include <Adafruit_ILI9341.h>	// incluye libreria para controlador ILI9341
 
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define TFT_MISO 3
#define TFT_MOSI 2
#define TFT_CLK 13
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
int x = 0, y = 5;

//Estado de interface
//1 - MENU
//2 - Entrenamiento
//3 - Reconocimiento
int estado = 1;
int select = 1;
int btnA = 7, btnB = 6;
int xi = 40, yi = 45;
///////////////////////////////////////////////////Lector SD/////////////////////////////////////////////////////////////////////////////////////////////
#include <SD.h>

File myFile;
String UltimoVal = "";

void setup() {
  Serial.begin(9600);
  iniciarCSV();
  UltimoVal = leerSD();
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
        tft.fillScreen(ILI9341_RED); 		// relleno de pantalla en color rojo
        estado = 2;
      }else if(select == 2){
        tft.fillScreen(ILI9341_RED); 		// relleno de pantalla en color rojo
        estado = 3;
      }
      select = 1;
      delay(500);
    }
  }
  else if(estado == 2){
    iniciaMQ();
    pantallaMQ();
    do{
      entrenarSens();
    }while(digitalRead(btnB) == HIGH);
    estado = 1;
  }
  else{
    
  }
}

void Menu(int xm, int ym){
  tft.setTextColor(ILI9341_WHITE);	// color de texto en blanco
  tft.setCursor(xm,ym);			// ubica cursor
  tft.print("Entrenamiento"); 	// imprime texto
  ym += 80;
  tft.setCursor(xm,ym);			// ubica cursor
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

void iniciaMQ(){
  tft.setTextSize(2);			// escala de texto en 2
    //init the sensor
  MQ2.init();
  MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ2.setA(574.25); MQ2.setB(-2.222); // Configure the equation to to calculate LPG concentration
  MQ2.setR0(9.659574468);

  MQ3.init(); 
  MQ3.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ3.setA(0.39); MQ3.setB(-1.50); //Configure the equation to calculate Alcohol concentration value
  //MQ3.init(); 
  MQ3.setR0(0.64);

  MQ4.init(); 
  MQ4.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ4.setA(1012.7); MQ4.setB(-2.786); // Configure the equation to to calculate CH4 concentration
  MQ4.setR0(3.86018237);

  MQ7.init(); 
  MQ7.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ7.setA(99.042); MQ7.setB(-1.518); // Configure the equation to calculate CO concentration value
  MQ7.setR0(4);
    
  MQ8.init(); 
  MQ8.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ8.setA(976.97); MQ8.setB(-0.688); // Configure the equation to to calculate H2 concentration
  MQ8.setR0(1);
  
  MQ135.init(); 
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.setA(110.47); MQ135.setB(-2.862); // Configure the equation to to calculate CO2 concentration
  MQ135.setR0(10);

  //Serial.print("Calibrating please wait.");
  float MQ2calcR0 = 0, 
  MQ3calcR0 = 0,
  MQ4calcR0 = 0,
  MQ7calcR0 = 0,
  MQ8calcR0 = 0,
  MQ135calcR0 = 0;
  for(int i = 1; i<=10; i ++){
    //Update the voltage Values
    MQ2.update();
    MQ3.update();
    MQ4.update();
    MQ7.update();
    MQ8.update();
    MQ135.update();

    MQ2calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    MQ3calcR0 += MQ3.calibrate(RatioMQ3CleanAir);
    MQ4calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
    MQ7calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
    MQ8calcR0 += MQ8.calibrate(RatioMQ8CleanAir);
    MQ135calcR0 += MQ135.calibrate(RatioMQ135CleanAir);

  }
  MQ2.setR0(MQ2calcR0/20);
  MQ3.setR0(MQ3calcR0/10);
  MQ4.setR0(MQ4calcR0/20);
  MQ7.setR0(MQ7calcR0/20);
  MQ8.setR0(MQ8calcR0/20);
  MQ135.setR0(MQ135calcR0/20);
 // Serial.println("  done!.");  
  if(isinf(MQ2calcR0) || isinf(MQ3calcR0) || isinf(MQ4calcR0) || isinf(MQ7calcR0) || isinf(MQ8calcR0) || isinf(MQ135calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(MQ2calcR0 == 0 || MQ3calcR0 == 0 || MQ4calcR0 == 0 || MQ7calcR0 == 0 || MQ8calcR0 == 0 || MQ135calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
}

void pantallaMQ(){
      //init ILI9341
    tft.setCursor(x,y);			// ubica cursor
    tft.print("MQ-2"); 	// imprime texto
    tft.setCursor(x+75,y);
    tft.print("LPG: "); 	// imprime texto
    y+=30;
    tft.setCursor(x,y);			// ubica cursor
    tft.print("MQ-3"); 	// imprime texto
    tft.setCursor(x+75,y);
    tft.print("Alcohol: "); 	// imprime texto
    y+=30;
    tft.setCursor(x,y);			// ubica cursor
    tft.print("MQ-4"); 	// imprime texto
    tft.setCursor(x+75,y);
    tft.print("CH4: "); 	// imprime texto
    y+=30;
    tft.setCursor(x,y);			// ubica cursor
    tft.print("MQ-7"); 	// imprime texto
    tft.setCursor(x+75,y);
    tft.print("CO: "); 	// imprime texto
    y+=30;
    tft.setCursor(x,y);			// ubica cursor
    tft.print("MMQ-8"); 	// imprime texto
    tft.setCursor(x+75,y);
    tft.print("H2: "); 	// imprime texto
    y+=30;
    tft.setCursor(x,y);			// ubica cursor
    tft.print("MQ-135"); 	// imprime texto
    x+=75;
    tft.setCursor(x,y);
    tft.print("CO2: "); 	// imprime texto
    x += 103;
    y = 5;
}

void entrenarSens(){
  oldTime = millis();
  while(millis() - oldTime <= (60*1000))
  { 
    // VH 5 Volts
    analogWrite(5, 255); // 255 is DC 5V output
    readAllSensors();
    delay(500);
  }
  // 90s cycle
  oldTime = millis();
  while(millis() - oldTime <= (90*1000))
  {
    // VH 1.4 Volts
    analogWrite(5, 20); // 255 is 100%, 20.4 is aprox 8% of Duty cycle for 90s
    readAllSensors();
    delay(500);
  }
}

void readAllSensors()
{
  delay(3000);
  //Update the voltage Values
  MQ2.update();
  MQ3.update();
  MQ4.update();
  MQ7.update();
  MQ8.update();
  MQ135.update();
  //Read the sensor and print in serial port
  float MQ2Lecture =  MQ2.readSensor();
  float MQ3Lecture =  MQ3.readSensor();
  float MQ4Lecture =  MQ4.readSensor();
  float MQ7Lecture =  MQ7.readSensor();
  float MQ8Lecture =  MQ8.readSensor();
  float MQ135Lecture =  MQ135.readSensor();
  String s = String(MQ2Lecture, 4) + "," + String(MQ3Lecture, 4) + "," + String(MQ4Lecture, 4) + "," + String(MQ7Lecture, 4) + "," + String(MQ8Lecture, 4) + "," + String(MQ135Lecture, 4) + "," + UltimoVal;
  GuardarSD(s);
  tft.setTextColor(ILI9341_BLUE, ILI9341_RED);	// texto azul en fondo rojo
  tft.setCursor(x,y);		// ubica cursor
  tft.print(MQ2Lecture);
  y += 30;
  tft.setCursor(x,y);		// ubica cursor
  tft.print(MQ3Lecture);
  y += 30;
  tft.setCursor(x,y);		// ubica cursor
  tft.print(MQ4Lecture);
  y += 30;
  tft.setCursor(x,y);		// ubica cursor
  tft.print(MQ7Lecture);
  y += 30;
  tft.setCursor(x,y);		// ubica cursor
  tft.print(MQ8Lecture);
  y += 30;
  tft.setCursor(x,y);		// ubica cursor
  tft.print(MQ135Lecture);
  y = 5;
}
void iniciarCSV(){
  Serial.begin(9600);
  if (!SD.begin(4)) {
    return;
  }
  if(!SD.exists("datalog.csv")){
        myFile = SD.open("datalog.csv", FILE_WRITE);
        if (myFile) {
          Serial.println("Archivo nuevo, Escribiendo encabezado(fila 1)");
          myFile.close();
        } else {
        }
  }
}

void GuardarSD(String s){
  myFile = SD.open("datalog.csv", FILE_WRITE);//abrimos  el archivo
  
  if (myFile) { 
    myFile.println(s);
    myFile.close(); //cerramos el archivo  
  } else {
    Serial.println("Error al abrir el archivo para escribir");
  }
  delay(1000);
}

String leerSD(){
  String cadena="";
  myFile = SD.open("datalog.csv",FILE_READ);//abrimos  el archivo 
  if (myFile){
    bool line=false;
    myFile.seek(myFile.size()-1); //Ubicacion en posicion anterior a ultimo caracter
    while (myFile.available()){
      if(line==false){ //Primero leer en reversa para buscar salto de linea
        char caracter=myFile.read();
        if(caracter==',') //Cuando encuentra salto de linea cambia estado
          {
            line=true;
          }else{
            myFile.seek(myFile.position()-2);
          }
      }
      if(line==true){//Empieza a leer normalmente de izquierda a derecha
          char caracter=myFile.read();
          cadena=cadena+caracter;
          if(caracter=='\n'){ //La cadena termina en este caracter
            break;
          }      
      }
    }
    myFile.close(); //cerramos el archivo
    delay(300);            
  } 
  else {
    Serial.println("Error al abrir el archivo para leer");
  }
  return cadena;
}