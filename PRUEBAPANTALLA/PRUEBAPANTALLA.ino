//Include the library
#include <MQUnifiedsensor.h>

/************************Hardware Related Macros************************************/
#define         Board                   ("Arduino Mega")
#define         Pin4                     (2)  //Analog input 6 of your arduino
#define         Pin5                     (3)  //Analog input 7 of your arduino

/***********************Software Related Macros************************************/
#define         RatioMQ2CleanAir          (9.86) //RS / R0 = 9.83 ppm 
#define         RatioMQ3CleanAir          (63) //RS / R0 = 60 ppm 
#define         RatioMQ4CleanAir          (4.4) //RS / R0 = 4.4 ppm 
#define         RatioMQ7CleanAir          (27.5) //RS / R0 = 27.5 ppm  
#define         RatioMQ8CleanAir          (70) //RS / R0 = 70 ppm
#define         RatioMQ135CleanAir        (3.6) //RS / R0 = 3.6 ppm 
#define         ADC_Bit_Resolution        (10) // 10 bit ADC 
#define         Voltage_Resolution        (5) // Volt resolution to calc the voltage

/*****************************Globals***********************************************/
//Declare Sensor
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, A1, "MQ-2");
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin4, "MQ-3");
MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, A3, "MQ-4");
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, A2, "MQ-7");
MQUnifiedsensor MQ8(Board, Voltage_Resolution, ADC_Bit_Resolution, A0, "MQ-8");
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin5, "MQ-135");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

int estado = 1, select = 1;
int btnA = 7, btnB = 6;
///////////////////////////////////////////////////Lector SD/////////////////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>
//#include <SD.h>
#include "SdFat.h"  

SdFat SD;
#define SD_CS_PIN SS
File myFile;

String UltimoVal = "0";

void setup() {
  Serial.begin(9600);
  iniciarCSV();
  lcd.init();                      // Inicializa el LCD
  lcd.backlight();                 // Enciende la luz de fondo del LCD
  delay(300);
  Menu();
  PintaFlecha(0, 0);
  //UltimoVal = leerSD();
  Serial.println(UltimoVal);
}

void loop() {
  if(estado == 1){
    if (digitalRead(btnA) == HIGH){
      if (select == 1){
        PintaFlecha(0, 3);
        select += 1; 
      }else if (select == 2){
        PintaFlecha(0, 0);
        select -= 1; 
      }
      delay(500);
    }else if (digitalRead(btnB) == HIGH){
      if(select == 1){
        estado = 2;
      }else if(select == 2){
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
    Menu();
  }
  else{
    
  }
}

void Menu(){
  lcd.setCursor(3,0);
  lcd.print("Entrenamiento");
  lcd.setCursor(3,3);
  lcd.print("Reconocimiento");
}

void PintaFlecha(int cx, int cy) {
  lcd.setCursor(0,0);
  lcd.print("  ");
  lcd.setCursor(0,3);
  lcd.print("  ");
  lcd.setCursor(cx,cy);
  lcd.print("->");
}

void iniciaMQ(){
  MQ2.init();
  MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ2.setA(574.25); MQ2.setB(-2.222); // Configure the equation to to calculate concentration
  MQ2.setR0(9.659574468);

  MQ3.init(); 
  MQ3.setRegressionMethod(1);
  MQ3.setA(0.39); MQ3.setB(-1.50);
  MQ3.setR0(0.64);

  MQ4.init(); 
  MQ4.setRegressionMethod(1);
  MQ4.setA(1012.7); MQ4.setB(-2.786);
  MQ4.setR0(3.86018237);

  MQ7.init(); 
  MQ7.setRegressionMethod(1);
  MQ7.setA(99.042); MQ7.setB(-1.518);
  MQ7.setR0(4);
    
  MQ8.init(); 
  MQ8.setRegressionMethod(1);
  MQ8.setA(976.97); MQ8.setB(-0.688);
  MQ8.setR0(1);
  
  MQ135.init(); 
  MQ135.setRegressionMethod(1);
  MQ135.setA(110.47); MQ135.setB(-2.862);
  MQ135.setR0(10);

  float MQ2calcR0 = 0, 
  MQ3calcR0 = 0,
  MQ4calcR0 = 0,
  MQ7calcR0 = 0,
  MQ8calcR0 = 0,
  MQ135calcR0 = 0;
  for(int i = 1; i<=10; i ++){
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
}

void pantallaMQ(){
    lcd.setCursor(0,0);
    lcd.print("                   ");
    lcd.setCursor(0,0);
    lcd.print("LPG: ");
    lcd.setCursor(0,1);
    lcd.print("Alcohol: ");
    lcd.setCursor(0,2);
    lcd.print("CH4: ");
    lcd.setCursor(0,3);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print("CO: ");
    lcd.setCursor(10,0);
    lcd.print("H2: ");
    lcd.setCursor(10,2);
    lcd.print("CO2: ");
}

void entrenarSens(){
  unsigned long oldTime = millis();
  while(millis() - oldTime <= (60*1000)){ 
    analogWrite(5, 255);
    readAllSensors();
    delay(500);
  }

  oldTime = millis();
  while(millis() - oldTime <= (90*1000)){
    analogWrite(5, 20);
    readAllSensors();
    delay(500);
  }
}

void readAllSensors()
{
  delay(3000);
  MQ2.update();
  MQ3.update();
  MQ4.update();
  MQ7.update();
  MQ8.update();
  MQ135.update();
  float MQ2Lecture =  MQ2.readSensor();
  float MQ3Lecture =  MQ3.readSensor();
  float MQ4Lecture =  MQ4.readSensor();
  float MQ7Lecture =  MQ7.readSensor();
  float MQ8Lecture =  MQ8.readSensor();
  float MQ135Lecture =  MQ135.readSensor();
  lcd.setCursor(5,0);
  lcd.print(MQ2Lecture);
  lcd.setCursor(9,1);
  lcd.print(MQ3Lecture);
  lcd.setCursor(5,2);
  lcd.print(MQ4Lecture);
  lcd.setCursor(4,3);
  lcd.print(MQ7Lecture);
  lcd.setCursor(13,0);
  lcd.print(MQ8Lecture);
  lcd.setCursor(14,2);
  lcd.print(MQ135Lecture);
  String s = String(MQ2Lecture, 4) + "," + String(MQ3Lecture, 4) + "," + String(MQ4Lecture, 4) + "," + String(MQ7Lecture, 4) + "," + String(MQ8Lecture, 4) + "," + String(MQ135Lecture, 4) + "," + UltimoVal;
  Serial.println("DATO");
  GuardarSD(s);
  Serial.println("LE");
  delay(800);
  
}
void iniciarCSV(){
  Serial.println("Iniciando lector sd");
  if (!SD.begin(53)) {
    Serial.println("ERROR");
    return;
  }
  /*if(!SD.exists("datalog.csv")){
        GuardarSD("LPG,ALCOHOL,CH4,CO,H2,CO2,IDMUESTRA");
        Serial.println("creado");
  }*/
  Serial.println("Inicializado");
}

void GuardarSD(String s){
  myFile = SD.open("datalog.txt", FILE_WRITE);//abrimos  el archivo
  Serial.println(s);
  if (myFile) { 
    myFile.println(s);
    myFile.close();
    Serial.println("Guardado");
  } else {
    Serial.println("Error al abrir el archivo para escribir");
  }
  
  delay(1000);
}

String leerSD(){
  String cadena="";
  myFile = SD.open("datalog.txt",FILE_READ);
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